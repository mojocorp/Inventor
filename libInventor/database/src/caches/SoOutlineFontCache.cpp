#include <Inventor/caches/SoOutlineFontCache.h>
#include <Inventor/caches/SoFontOutline.h>
#include <Inventor/caches/SoGLDisplayList.h>
#include <Inventor/SbVec2s.h>
#include <Inventor/elements/SoCacheElement.h>
#include <Inventor/elements/SoGLTextureEnabledElement.h>
#include <Inventor/elements/SoGLCacheContextElement.h>
#include <Inventor/elements/SoFontNameElement.h>
#include <Inventor/elements/SoFontSizeElement.h>
#include <Inventor/elements/SoComplexityElement.h>
#include <Inventor/elements/SoComplexityTypeElement.h>
#include <Inventor/elements/SoCreaseAngleElement.h>
#include <Inventor/elements/SoModelMatrixElement.h>
#include <Inventor/elements/SoProfileElement.h>
#include <Inventor/elements/SoProfileCoordinateElement.h>
#include <Inventor/elements/SoProjectionMatrixElement.h>
#include <Inventor/elements/SoViewportRegionElement.h>
#include <Inventor/elements/SoViewingMatrixElement.h>
#include <Inventor/nodes/SoShape.h>
#include <Inventor/nodes/SoProfile.h>
#include <Inventor/nodes/SoFont.h>

#include <cmath>
#include <algorithm>

#include "utopia-regular.cpp"

SbBool SoOutlineFontCache::tesselationError = FALSE;
std::vector<SoOutlineFontCache*> SoOutlineFontCache::fonts;
FT_Library SoOutlineFontCache::library = NULL;

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Find an appropriate font, given a state.  A bunch of elements
//    (fontName, fontSize, creaseAngle, complexity and profile) must
//    be enabled in the state...
//
// Use: static, internal

SoOutlineFontCache *
SoOutlineFontCache::getFont(SoState *state, SbBool forRender)
//
////////////////////////////////////////////////////////////////////////
{
    if (library == NULL) {
        // One-time font library initialization
        if (FT_Init_FreeType( &library )) {
#ifdef DEBUG
            SoDebugError::post("SoOutlineFontCache::getFont",
                               "FT_Init_FreeType failed");
#endif
            return NULL;
        }
    }

    SoOutlineFontCache *result = NULL;
    for (size_t i = 0; i < fonts.size() && result == NULL; i++) {
        SoOutlineFontCache *c = fonts[i];
        if (forRender ? c->isRenderValid(state) : c->isValid(state)) {
            result = c; // Loop will terminate...
        }
    }
    // If none match:
    if (result == NULL) {
        result = new SoOutlineFontCache(state);

        // If error:
        if (result->face == 0) {
            delete result;
            return NULL;
        }
    }
    return result;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Figures out if this cache is valid for rendering.
//
// Use: internal

SbBool
SoOutlineFontCache::isRenderValid(SoState *state) const
//
////////////////////////////////////////////////////////////////////////
{
    // Special cache case here:  if we generated side display lists
    // without texture coordinates AND we need texture coordinates,
    // we'll have to regenerate and this cache is invalid:
    if (!sideList.empty()) {
        if (!sidesHaveTexCoords && SoGLTextureEnabledElement::get(state)) {
            return FALSE;
        }
    }

    if (!isValid(state))
        return FALSE;

    if (context != SoGLCacheContextElement::get(state))
        return FALSE;

    return TRUE;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Constructor.  Called by getFont().
//
// Use: private

SoOutlineFontCache::SoOutlineFontCache(SoState *state) :
    SoCache(state), context(-1)
//
////////////////////////////////////////////////////////////////////////
{
    face = NULL;

    // Add element dependencies explicitly here; making 'this' the
    // CacheElement doesn't work if we are being constructed in an
    // action that doesn't have caches.
    const SbName & fontName = SoFontNameElement::get(state);
    addElement(state->getConstElement(SoFontNameElement::getClassStackIndex()));

    // Remember size
    fontSize = SoFontSizeElement::get(state);
    addElement(state->getConstElement(SoFontSizeElement::getClassStackIndex()));

    if (fontName == SoFontNameElement::getDefault() || fontName == "Utopia-Regular") {
        if (FT_New_Memory_Face(library, binary_utopia_regular, BINARY_UTOPIA_REGULAR_SIZE, 0, &face)) {
#ifdef DEBUG
            SoDebugError::post("SoBitmapFontCache::getFont",
                               "Couldn't load embeded font Utopia-Regular!");
#endif
        }
    } else {
        SbFile file;
        if (file.open(SoFont::getFontFileName(fontName), "rb")) {
            buffer.resize(SbFile::size(SoFont::getFontFileName(fontName)));
            file.read(&buffer[0], 1, buffer.size());

            if (FT_New_Memory_Face(library, &buffer[0], (FT_Long)buffer.size(), 0, &face)) {
#ifdef DEBUG
                SoDebugError::post("SoText3::getFont",
                                   "Couldn't find font %s, replacing with Utopia-Regular",
                                   fontName.getString());
#endif
                if (FT_New_Memory_Face(library, binary_utopia_regular, BINARY_UTOPIA_REGULAR_SIZE, 0, &face)) {
#ifdef DEBUG
                    SoDebugError::post("SoText3::getFont",
                                       "Couldn't find font Utopia-Regular!");
#endif
                }
            }
        }
    }

    sidesHaveTexCoords = FALSE;

    // Get profile info:
    const SoNodeList &profiles = SoProfileElement::get(state);
    addElement(state->getConstElement(SoProfileElement::getClassStackIndex()));
    addElement(state->getConstElement(SoProfileCoordinateElement::getClassStackIndex()));
    nProfileVerts = 0;
    if (profiles.getLength() > 0) {
        SoProfile *profileNode = (SoProfile *)profiles[0];
        profileNode->getVertices(state, nProfileVerts, profileVerts);
    } else {
        nProfileVerts = 2;
        profileVerts = new SbVec2f[2];
        profileVerts[0].setValue(0, 0);
        profileVerts[1].setValue(1, 0);
    }

    if (nProfileVerts > 1) {
        cosCreaseAngle = cos(SoCreaseAngleElement::get(state));
        addElement(state->getConstElement(SoCreaseAngleElement::getClassStackIndex()));
        int nSegments = (int) nProfileVerts - 1;

        // Figure out normals for profiles; there are twice as many
        // normals as segments.  The two normals for each segment endpoint
        // may be averaged with the normal for the next segment, depending
        // on whether or not the angle between the segments is greater
        // than the creaseAngle.
        profileNorms.resize(nSegments*2);
        figureSegmentNorms(profileNorms, (int) nProfileVerts, profileVerts,
                           cosCreaseAngle, FALSE);
        // Need to flip all the normals because of the way the profiles
        // are defined:
        for (int i = 0; i < nSegments*2; i++) {
            profileNorms[i] *= -1.0;
        }

        // Figure out S texture coordinates, which run along the profile:
        sTexCoords.resize(nProfileVerts);
        figureSegmentTexCoords(sTexCoords, (int) nProfileVerts,
                               profileVerts, FALSE);
        // And reverse them, so 0 is at the back of the profile:
        float max = sTexCoords[nProfileVerts-1];
        for (int i = 0; i < nProfileVerts; i++) {
            sTexCoords[i] = max - sTexCoords[i];
        }
    }

    fonts.push_back(this);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Destructor
//
// Use: private

SoOutlineFontCache::~SoOutlineFontCache()
//
////////////////////////////////////////////////////////////////////////
{
    if (face) {
        // Free up cached outlines
        {
        std::map<char, SoFontOutline*>::iterator it;
        for (it=outlines.begin(); it!=outlines.end(); it++) {
            delete it->second;
        }
        outlines.clear();
}

        std::map<char, SoGLDisplayList*>::iterator it;
        for (it=frontList.begin(); it!=frontList.end(); it++) {
            it->second->unref(NULL);
        }
        for (it=sideList.begin(); it!=sideList.end(); it++) {
            it->second->unref(NULL);
        }

        if (hasProfile()) {
            delete[] profileVerts;
        }

        // Only destroy the font library font if no other font caches
        // are using the same font identifier:
        SbBool otherUsing = FALSE;
        for (size_t i = 0; i < fonts.size(); i++) {
            SoOutlineFontCache *t = fonts[i];
            if (t != this && t->face == face) otherUsing = TRUE;
        }
        if (!otherUsing) {
            FT_Done_Face(face);
            face = NULL;
        }
        fonts.erase(std::find(fonts.begin(), fonts.end(), this));
    }

    if (fonts.empty()) {
        FT_Done_FreeType(library);
        library = NULL;
    }
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Returns the width of the given string
//
// Use: private

float
SoOutlineFontCache::getWidth(const SbString &string)
//
////////////////////////////////////////////////////////////////////////
{
    float total = 0.0;
    const char *chars = string.getString();

    for (size_t i = 0; i < string.getLength(); i++) {
        const SoFontOutline *outline = getOutline(chars[i]);
        total += outline->getCharAdvance()[0];
    }

    return total;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Returns the 2D bounding box of the given character.
//
// Use: private

void
SoOutlineFontCache::getCharBBox(const char c, SbBox2f &result)
//
////////////////////////////////////////////////////////////////////////
{
    result.makeEmpty();

    if (!face) return;

    SoFontOutline *outline = getOutline(c);

    result = outline->getBoundingBox();
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Given a character, return an outline for the character.  If, for
//    some reason, we can't get the outline, an 'identity' or 'null'
//    outline is returned.
//
// Use: private

SoFontOutline *
SoOutlineFontCache::getOutline(const char c)
//
////////////////////////////////////////////////////////////////////////
{
    if (!face) {
        return SoFontOutline::getNullOutline();
    }
    if (outlines[c] == NULL) {
        outlines[c] = new SoFontOutline(c, face, fontSize);
    }
    return outlines[c];
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Given a character, return the correct amount to advance after
//    drawing that character.  Note:  if we ever wanted to deal with
//    kerning, we'd have to fix this...
//
// Use: private

SbVec2f
SoOutlineFontCache::getCharOffset(const char c)
//
////////////////////////////////////////////////////////////////////////
{
    if (!face) return SbVec2f(0,0);

    return getOutline(c)->getCharAdvance();
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Render the fronts of the given string.  The GL transformation
//    matrix is munged by this routine-- surround it by
//    PushMatrix/PopMatrix.
//
// Use: public, internal

void
SoOutlineFontCache::generateFrontChar(const char c,
                                      GLUtesselator *tobj)
//
////////////////////////////////////////////////////////////////////////
{
    if (!face) return;

    GLdouble v[3];

    tesselationError = FALSE;
    gluTessBeginPolygon( tobj, NULL );

    // Get outline for character
    SoFontOutline *outline = getOutline(c);
    for (size_t i = 0; i < outline->getNumOutlines(); i++) {
        gluTessBeginContour( tobj );
        for (size_t j = 0; j < outline->getNumVerts(i); j++) {
            const SbVec2f &t = outline->getVertex(i,j);
            v[0] = t[0];
            v[1] = t[1];
            v[2] = 0.0;

            // Note: The third argument MUST NOT BE a local variable,
            // since glu just stores the pointer and only calls us
            // back at the gluEndPolygon call.
            gluTessVertex(tobj, v, (GLvoid*)&t);
        }
        gluTessEndContour( tobj );
    }
    gluTessEndPolygon( tobj );

    // If there was an error tesselating the character, just generate
    // a bounding box for the character:
    if (tesselationError) {
        SbBox2f charBBox;
        getCharBBox(c, charBBox);
        if (!charBBox.isEmpty()) {
            SbVec2f boxVerts[4];
            charBBox.getBounds(boxVerts[0], boxVerts[2]);
            boxVerts[1].setValue(boxVerts[2][0], boxVerts[0][1]);
            boxVerts[3].setValue(boxVerts[0][0], boxVerts[2][1]);

            gluTessBeginPolygon( tobj, NULL );
            gluTessBeginContour( tobj );
            for (int i = 0; i < 4; i++) {
                v[0] = boxVerts[i][0];
                v[1] = boxVerts[i][1];
                v[2] = 0.0;
                gluTessVertex(tobj, v, &boxVerts[i]);
            }
            gluTessEndContour( tobj );
            gluTessEndPolygon( tobj );
        }
    }
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Returns TRUE if a display lists exists for given character.
//    Tries to build a display list, if it can.
//
// Use: internal

SbBool
SoOutlineFontCache::hasFrontDisplayList(SoState *state, const char c, GLUtesselator *tobj)
//
////////////////////////////////////////////////////////////////////////
{
    // If we have one, return TRUE
    if (frontList[c])
        return TRUE;

    // If we don't and we can't build one, return FALSE.
    if (SoCacheElement::anyOpen(state))
        return FALSE;

    context = SoGLCacheContextElement::get(state);

    // Build one:
    frontList[c] = new SoGLDisplayList(state, SoGLDisplayList::DISPLAY_LIST);
    frontList[c]->ref();
    frontList[c]->addDependency(state);
    glNewList(frontList[c]->getFirstIndex(), GL_COMPILE);

    generateFrontChar(c, tobj);
    const SbVec2f & t = getOutline(c)->getCharAdvance();
    glTranslatef(t[0], t[1], 0.0);
    frontList[c]->close(state);

    return TRUE;
}
////////////////////////////////////////////////////////////////////////
//
// Description:
//    Returns TRUE if a display lists exists for given character.
//    Tries to build a display list, if it can.
//
// Use: internal

SbBool
SoOutlineFontCache::hasSideDisplayList(SoState *state, const char c, SideCB callbackFunc)
//
////////////////////////////////////////////////////////////////////////
{
    // If we have one, return TRUE
    if (sideList[c])
        return TRUE;

    // If we don't and we can't build one, return FALSE.
    if (SoCacheElement::anyOpen(state))
        return FALSE;

    sidesHaveTexCoords = SoGLTextureEnabledElement::get(state);

    // Build one:
    sideList[c] = new SoGLDisplayList(state, SoGLDisplayList::DISPLAY_LIST);
    sideList[c]->ref();
    sideList[c]->addDependency(state);
    glNewList(sideList[c]->getFirstIndex(), GL_COMPILE);

    glBegin(GL_QUADS);    // Render as independent quads:
    generateSideChar(c, callbackFunc);
    glEnd();

    const SbVec2f & t = getOutline(c)->getCharAdvance();
    glTranslatef(t[0], t[1], 0.0);
    sideList[c]->close(state);

    return TRUE;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Assuming that there are not display lists built for all the
//    characters in given string, render the string.
//
// Use: internal

void
SoOutlineFontCache::renderFront(SoState *state, const SbString &string, GLUtesselator *tobj)
//
////////////////////////////////////////////////////////////////////////
{
    const char *str = string.getString();

    for (size_t i = 0; i < string.getLength(); i++) {
        if (hasFrontDisplayList(state, str[i], tobj)) {
            frontList[str[i]]->call(state);
        }
        else {
            generateFrontChar(str[i], tobj);
            const SbVec2f & t = getOutline(str[i])->getCharAdvance();
            glTranslatef(t[0], t[1], 0.0);
        }
    }
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Assuming that there are not display lists built for all the
//    characters in given string, render the string.
//
// Use: internal

void
SoOutlineFontCache::renderSide(SoState *state, const SbString &string, SideCB callbackFunc)
//
////////////////////////////////////////////////////////////////////////
{
    const char *str = string.getString();

    for (size_t i = 0; i < string.getLength(); i++) {
        if (hasSideDisplayList(state, str[i], callbackFunc)) {
            sideList[str[i]]->call(state);
        }
        else {
            glBegin(GL_QUADS);
            generateSideChar(str[i], callbackFunc);
            glEnd();

            const SbVec2f & t = getOutline(str[i])->getCharAdvance();
            glTranslatef(t[0], t[1], 0.0);
        }
    }
}


////////////////////////////////////////////////////////////////////////
//
// Description:
//   Find the first and last points in the bevel-- that is where the
//   front and back of the character will be.
// Use:
//   internal

void
SoOutlineFontCache::getProfileBounds(float &firstZ, float &lastZ)
//
////////////////////////////////////////////////////////////////////////
{
    if (hasProfile()) {
        firstZ = -profileVerts[0][0];
        lastZ = -profileVerts[nProfileVerts-1][0];
    } else {
        firstZ = lastZ = 0;
    }
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//   Get the 2D bounding box of the bevel.
//
//  Use:
//    internal

void
SoOutlineFontCache::getProfileBBox(SbBox2f &profileBox)
//
////////////////////////////////////////////////////////////////////////
{
    for (int i = 0; i < nProfileVerts; i++) {
        profileBox.extendBy(profileVerts[i]);
    }
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Called by generateSide.  This generates the bevel triangles for
//    one character.
//
// Use: private

void
SoOutlineFontCache::generateSideChar(const char c, SideCB callbackFunc)
//
////////////////////////////////////////////////////////////////////////
{
    if (!hasProfile()) return;

    // Get the outline of the character:
    SoFontOutline *outline = getOutline(c);

    for (size_t i = 0; i < outline->getNumOutlines(); i++) {
        // For each outline:

        size_t nOutline = outline->getNumVerts(i);

        SbVec2f *oVerts = new SbVec2f[nOutline];
        // Copy in verts so figureSegmentNorms can handle them..
        for (int j = 0; j < nOutline; j++) {
            oVerts[j] = outline->getVertex(i, j);
        }

        // First, figure out a set of normals for the outline:
        std::vector<SbVec2f> oNorms(nOutline*2);
        figureSegmentNorms(oNorms, nOutline, oVerts, cosCreaseAngle, TRUE);

        // And appropriate texture coordinates:
        // Figure out T texture coordinates, which run along the
        // outline:
        std::vector<float> tTexCoords(nOutline+1);
        figureSegmentTexCoords(tTexCoords, nOutline, oVerts, TRUE);

        // Now, generate a set of triangles for each segment in the
        // outline.  A bevel of profiles is built at each point in the
        // outline; each profile must be flipped perpendicular to the
        // outline (x coordinate becomes -z), rotated to be the
        // average of the normals of the two adjoining segments at
        // that point, and translated to that point.  Triangles are
        // formed between consecutive bevels.
        // Normals are just taken from the 'pNorms' array, after being
        // rotated the appropriate amount.

        SbVec3f *bevel1 = new SbVec3f[nProfileVerts];
        SbVec3f *bevelN1 = new SbVec3f[(nProfileVerts-1)*2];
        SbVec3f *bevel2 = new SbVec3f[nProfileVerts];
        SbVec3f *bevelN2 = new SbVec3f[(nProfileVerts-1)*2];

        // fill out first bevel:
        fillBevel(bevel1, (int) nProfileVerts, profileVerts,
                  outline->getVertex(i,0),
                  oNorms[(nOutline-1)*2+1], oNorms[0*2]);

        SbVec3f *s1 = bevel1;
        SbVec3f *s2 = bevel2;

        for (int j = 0; j < nOutline; j++) {
            // New normals are calculated for both ends of this
            // segment, since the normals may or may not be shared
            // with the previous segment.
            fillBevelN(bevelN1, profileNorms, oNorms[j*2]);

            int j2 = (j+1)%nOutline;
            // fill out second bevel:
            fillBevel(s2, (int) nProfileVerts, profileVerts,
                      outline->getVertex(i,j2),
                      oNorms[j*2+1], oNorms[j2*2]);
            fillBevelN(bevelN2, profileNorms, oNorms[j*2+1]);

            // And generate triangles between the two bevels:
            (*callbackFunc)((int) nProfileVerts, s1, bevelN1, s2, bevelN2,
                            &sTexCoords[0], &tTexCoords[j]);

            // Swap bevel1/2 (avoids some recomputation)
            SbVec3f *t;
            t = s1; s1 = s2; s2 = t;
        }
        delete [] bevelN2;
        delete [] bevel2;
        delete [] bevelN1;
        delete [] bevel1;
        delete [] oVerts;
    }
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Given a set of line segments, this figures out the normal at
//    each point in each segment.  It uses the creaseAngle passed in
//    to decide whether or not adjacent segments share normals.  The
//    isClosed flag is used to determine whether or not the first and
//    last points should be considered another segment.
//
//
// Use: private

void
SoOutlineFontCache::figureSegmentNorms(std::vector<SbVec2f> & norms, int nPoints,
                            const SbVec2f *points,  float cosCreaseAngle,
                            SbBool isClosed)
//
////////////////////////////////////////////////////////////////////////
{
    int nSegments = isClosed ? nPoints : nPoints-1;

    // First, we'll just make all the normals perpendicular to their
    // segments:
    for (int i = 0; i < nSegments; i++) {
        SbVec2f n;
        // This is 2D perpendicular, assuming profile is increasing in
        // X (which becomes 'decreasing in Z' when we actually use
        // it...) (note: if a profile isn't increasing in X, the
        // character will be inside-out, with the front face drawn
        // behind the back face, etc).
        SbVec2f v = points[(i+1)%nPoints] - points[i];
        n[0] =  v[1];
        n[1] = -v[0];
        n.normalize();

        norms[i*2+0] = n;
        norms[i*2+1] = n;
    }

    // Now, figure out if the angle between any two segments is small
    // enough to average two of their normals.
    for (int i = 0; i < (isClosed ? nSegments : nSegments-1); i++) {
        SbVec2f seg1 = points[(i+1)%nPoints] - points[i];
        seg1.normalize();
        SbVec2f seg2 = points[(i+2)%nPoints] - points[(i+1)%nPoints];
        seg2.normalize();

        float dp = seg2.dot(seg1);
        if (dp > cosCreaseAngle) {
            // Average the second normal for this segment, and the
            // first normal for the next segment:
            SbVec2f average = norms[i*2+1] + norms[((i+1)%nPoints)*2];
            average.normalize();
            norms[i*2+1] = average;
            norms[((i+1)%nPoints)*2] = average;
        }
    }
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Given a set of line segments, this figures out the texture
//    coordinates for each vertex.  If the isClosed flag is TRUE,
//    an extra texture coordinate is calculated, AND the points are
//    traversed in reverse order.
//
// Use: private

void
SoOutlineFontCache::figureSegmentTexCoords(std::vector<float> & texCoords, int nPoints,
                            const SbVec2f *points, SbBool isClosed)
//
////////////////////////////////////////////////////////////////////////
{
    float total = 0.0;

    if (isClosed) {
        for (int i = nPoints; i >= 0; i--) {
            texCoords[i] = total / getHeight();
            if (i > 0) {
                total += (points[i%nPoints] - points[i-1]).length();
            }
        }
    } else {
        for (int i = 0; i < nPoints; i++) {
            texCoords[i] = total / getHeight();
            if (i+1 < nPoints) {
                total += (points[i+1] - points[i]).length();
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Given a set of segments that make up a profile or bevel along
//    which we'll extrude the front face of the text, this routine
//    transforms the bevel from its default orientation (in the x-y
//    plane) to its correct position for a particular point on the
//    text outline, rotated and translated into position.  The
//    translation is the point on the outline, and the two normals
//    passed in are the normals for the segments adjoining that point.
//
// Use: private

void
SoOutlineFontCache::fillBevel(SbVec3f *result, int nPoints,
          const SbVec2f *points,
          const SbVec2f &translation,
          const SbVec2f &n1, const SbVec2f &n2)
//
////////////////////////////////////////////////////////////////////////
{
    // First, figure out a rotation for this bevel:
    SbVec2f n = n1+n2;
    n.normalize();

    // Now, for each point:
    for (int i = 0; i < nPoints; i++) {
        // This is really the 2D rotation formula,
        // x = x' cos(angle) - y' sin(angle)
        // y = x' sin(angle) + y' cos(angle)
        // Because of the geometry, cos(angle) is n[1] and sin(angle)
        // is -n[0], and x' is zero (the bevel always goes straight
        // back).
        result[i][0] = points[i][1] * n[0] + translation[0];
        result[i][1] = points[i][1] * n[1] + translation[1];
        result[i][2] = -points[i][0];
    }
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Given a set of normals for a profile, this rotates the normals
//    from their default position (int the x-y plane) to the correct
//    orientation for a particular point on the texts outline.  The
//    normal passed in is the normal for one end of one of the
//    outline's segments.

void
SoOutlineFontCache::fillBevelN(SbVec3f *result,
          const std::vector<SbVec2f> & norms,
          const SbVec2f &n)
//
////////////////////////////////////////////////////////////////////////
{
    // Now, for each point:
    for (size_t i = 0; i < norms.size(); i++) {
        // This is really the 2D rotation formula,
        // x = x' cos(angle) - y' sin(angle)
        // y = x' sin(angle) + y' cos(angle)
        // Because of the geometry, cos(angle) is n[1] and sin(angle)
        // is -n[0], and x' is zero (the bevel always goes straight
        // back).
        result[i][0] = norms[i][1] * n[0];
        result[i][1] = norms[i][1] * n[1];
        result[i][2] = -norms[i][0];
    }
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Called by the GLU tesselator when there is an error
//
// Use: static, private
//
////////////////////////////////////////////////////////////////////////

#ifdef DEBUG
void
SoOutlineFontCache::errorCB(GLenum whichErr)
{
    SoDebugError::post("SoText3::errorCB", "%s", gluErrorString(whichErr));
    tesselationError = TRUE;
}
#else  /* DEBUG */
void
SoOutlineFontCache::errorCB(GLenum)
{
    tesselationError = TRUE;
}
#endif /* DEBUG */
