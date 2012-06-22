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
FT_Library SoOutlineFontCache::context = NULL;

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
    if (context == NULL) {
        // One-time font library initialization
        if (FT_Init_FreeType( &context )) {
#ifdef DEBUG
            SoDebugError::post("SoOutlineFontCache::getFont",
                               "FT_Init_FreeType failed");
#endif
            return NULL;
        }
    }
    else if (context == NULL) return NULL;

    SoOutlineFontCache *result = NULL;
    for (int i = 0; i < fonts.size() && result == NULL; i++) {
        SoOutlineFontCache *c = fonts[i];
        if (forRender ? c->isRenderValid(state) : c->isValid(state)) {
            result = c; // Loop will terminate...
            result->ref(); // Increment ref count
        }
    }
    // If none match:
    if (result == NULL) {
        result = new SoOutlineFontCache(state);

        // If error:
        if (result->fontId == 0) {
            delete result;
            return NULL;
        }
    }
    return result;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Sees if this font is valid.  If it is valid, it also makes it
//    current.
//
// Use: public

SbBool
SoOutlineFontCache::isValid(SoState *state) const
//
////////////////////////////////////////////////////////////////////////
{
    return SoCache::isValid(state);
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
    if (sideList) {
        if (!sidesHaveTexCoords &&
            SoGLTextureEnabledElement::get(state)) {
            return FALSE;
        }
    }

    if (!isValid(state)) return FALSE;

    if (frontList &&
        frontList->getContext() != SoGLCacheContextElement::get(state))
        return FALSE;
    if (sideList &&
        sideList->getContext() != SoGLCacheContextElement::get(state))
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
        SoCache(state)
//
////////////////////////////////////////////////////////////////////////
{
    ref();

    frontList = sideList = NULL;

    // Add element dependencies explicitly here; making 'this' the
    // CacheElement doesn't work if we are being constructed in an
    // action that doesn't have caches.
    SbName font = SoFontNameElement::get(state);
    addElement(state->getConstElement(
        SoFontNameElement::getClassStackIndex()));
    if (font == SoFontNameElement::getDefault()) {
        font = SbName("Utopia-Regular");
    }

    float uems;

    // Remember size
    fontSize = SoFontSizeElement::get(state);
    addElement(state->getConstElement(
        SoFontSizeElement::getClassStackIndex()));

    // Figure out complexity...
    float complexity = SoComplexityElement::get(state);
    addElement(state->getConstElement(
        SoComplexityElement::getClassStackIndex()));
    addElement(state->getConstElement(
        SoComplexityTypeElement::getClassStackIndex()));

    switch (SoComplexityTypeElement::get(state)) {
      case SoComplexityTypeElement::OBJECT_SPACE:
        {
            // Two ramps-- complexity of zero  == 250/1000 of an em
            //             complexity of .5    == 20/1000 of an em
            //             complexity of 1     == 1/1000 of an em
            const float ZERO = 250;
            const float HALF = 20;
            const float ONE = 1;
            if (complexity > 0.5f) uems = (2.0f-complexity*2.0f)*(HALF-ONE)+ONE;
            else uems = (1.0f-complexity*2.0f)*(ZERO-HALF)+HALF;
        }
        break;

      case SoComplexityTypeElement::SCREEN_SPACE:
        {
            SbVec3f p(fontSize, fontSize, fontSize);
            SbVec2s rectSize;

            SoShape::getScreenSize(state, SbBox3f(-p, p), rectSize);
            float maxSize =
                (rectSize[0] > rectSize[1] ? rectSize[0] : rectSize[1]);
            uems = 250.0f / (1.0f + 0.25f * maxSize * complexity *
                            complexity);

            // We have to manually add the dependency on the
            // projection, view and model matrix elements (these are
            // gotten in the SoShape::getScreenSize routine), and the
            // ViewportRegionElement:
            addElement(state->getConstElement(
                SoProjectionMatrixElement::getClassStackIndex()));
            addElement(state->getConstElement(
                SoViewingMatrixElement::getClassStackIndex()));
            addElement(state->getConstElement(
                SoModelMatrixElement::getClassStackIndex()));
            addElement(state->getConstElement(
                SoViewportRegionElement::getClassStackIndex()));
        }
        break;

      case SoComplexityTypeElement::BOUNDING_BOX:
        {
            uems = 20;
        }
        break;
    }
    //TODO flSetHint(FL_HINT_TOLERANCE, uems);

    // If error creating font:
    if (FT_New_Face( context, SoFontNameElement::getFontFileName(font).getString(), 0, &fontId )) {
#ifdef DEBUG
            SoDebugError::post("SoText3::getFont",
                      "Couldn't find font %s, replacing with Utopia-Regular",
                       font.getString());
#endif
        if (FT_New_Memory_Face(context, binary_utopia_regular, BINARY_UTOPIA_REGULAR_SIZE, 0, &fontId)) {
#ifdef DEBUG
            SoDebugError::post("SoText3::getFont",
                               "Couldn't find font Utopia-Regular!");
#endif
        }
    }

    FT_Set_Char_Size( fontId, fontSize * 64, 0, 72, 72);

    int numChars = 256;  // ??? NEED TO REALLY KNOW HOW MANY CHARACTERS IN
                     // FONT!
    sidesHaveTexCoords = FALSE;

    frontFlags = new SbBool[numChars];
    sideFlags = new SbBool[numChars];
    outlines.resize(numChars);
    for (int i = 0; i < numChars; i++) {
        frontFlags[i] = sideFlags[i] = FALSE;
        outlines[i] = NULL;
    }

    // Get profile info:
    const SoNodeList &profiles = SoProfileElement::get(state);
    addElement(state->getConstElement(
        SoProfileElement::getClassStackIndex()));
    addElement(state->getConstElement(
        SoProfileCoordinateElement::getClassStackIndex()));
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
        addElement(state->getConstElement(
            SoCreaseAngleElement::getClassStackIndex()));
        int nSegments = (int) nProfileVerts - 1;

        // Figure out normals for profiles; there are twice as many
        // normals as segments.  The two normals for each segment endpoint
        // may be averaged with the normal for the next segment, depending
        // on whether or not the angle between the segments is greater
        // than the creaseAngle.
        profileNorms = new SbVec2f[nSegments*2];
        figureSegmentNorms(profileNorms, (int) nProfileVerts, profileVerts,
                           cosCreaseAngle, FALSE);
        // Need to flip all the normals because of the way the profiles
        // are defined:
        for (int i = 0; i < nSegments*2; i++) {
            profileNorms[i] *= -1.0;
        }

        // Figure out S texture coordinates, which run along the profile:
        sTexCoords = new float[nProfileVerts];
        figureSegmentTexCoords(sTexCoords, (int) nProfileVerts,
                               profileVerts, FALSE);
        // And reverse them, so 0 is at the back of the profile:
        float max = sTexCoords[nProfileVerts-1];
        for (int i = 0; i < nProfileVerts; i++) {
            sTexCoords[i] = max - sTexCoords[i];
        }
    } else {
        profileNorms = NULL;
        sTexCoords = NULL;
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
    if (fontId) {

        delete[] frontFlags;
        delete[] sideFlags;

        // Free up cached outlines
        for (int i = 0; i < outlines.size(); i++) {
            delete outlines[i];
        }
        outlines.clear();

        if (hasProfile()) {
            delete[] profileVerts;
            delete[] sTexCoords;
            delete[] profileNorms;
        }

        // Only destroy the font library font if no other font caches
        // are using the same font identifier:
        SbBool otherUsing = FALSE;
        for (int i = 0; i < fonts.size(); i++) {
            SoOutlineFontCache *t = fonts[i];
            if (t != this && t->fontId == fontId) otherUsing = TRUE;
        }
        if (!otherUsing) {
            FT_Done_Face(fontId);
        }
        fonts.erase(std::find(fonts.begin(), fonts.end(), this));
    }
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Destroy this cache.  Called by unref(); frees up OpenGL display
//    lists.
//
// Use: protected, virtual

void
SoOutlineFontCache::destroy(SoState *)
//
////////////////////////////////////////////////////////////////////////
{
    // Pass in NULL to unref because this cache may be destroyed
    // from an action _other_ than GLRender:
    if (frontList) {
        frontList->unref(NULL);
        frontList = NULL;
    }
    if (sideList) {
        sideList->unref(NULL);
        sideList = NULL;
    }

    SoCache::destroy(NULL);

    FT_Done_FreeType(context);

    context = NULL;
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

    for (int i = 0; i < string.getLength(); i++) {
        SoFontOutline *outline = getOutline(chars[i]);
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

    if (!fontId) return;

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
    if (!fontId) {
        return SoFontOutline::getNullOutline();
    }
    if (outlines[c] == NULL) {
        if(FT_Load_Char(fontId, c, FT_LOAD_NO_BITMAP | FT_LOAD_NO_HINTING | FT_LOAD_IGNORE_TRANSFORM)) {
    #ifdef DEBUG
            SoDebugError::post("SoOutlineFontCache::getOutline",
                               "FT_Load_Char failed");
    #endif
        }

        outlines[c] = new SoFontOutline(fontId->glyph, fontSize);

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
    if (!fontId) return SbVec2f(0,0);

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
    if (!fontId) return;

    GLdouble v[3];

    tesselationError = FALSE;
    gluBeginPolygon(tobj);

    // Get outline for character
    SoFontOutline *outline = getOutline(c);
    int i;
    for (i = 0; i < outline->getNumOutlines(); i++) {

        // It would be nice if the font manager told us the type of
        // each outline...
        gluNextContour(tobj, (GLenum)GLU_UNKNOWN);

        for (int j = 0; j < outline->getNumVerts(i); j++) {
            const SbVec2f &t = outline->getVertex(i,j);
            v[0] = t[0];
            v[1] = t[1];
            v[2] = 0.0;

            // Note: The third argument MUST NOT BE a local variable,
            // since glu just stores the pointer and only calls us
            // back at the gluEndPolygon call.
            gluTessVertex(tobj, v, (GLvoid*)&t);
        }
    }
    gluEndPolygon(tobj);

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

            gluBeginPolygon(tobj);
            for (i = 0; i < 4; i++) {
                v[0] = boxVerts[i][0];
                v[1] = boxVerts[i][1];
                v[2] = 0.0;
                gluTessVertex(tobj, v, &boxVerts[i]);
            }
            gluEndPolygon(tobj);
        }
    }
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Sets up for GL rendering.
//
// Use: internal

void
SoOutlineFontCache::setupToRenderFront(SoState *state)
//
////////////////////////////////////////////////////////////////////////
{
    otherOpen = SoCacheElement::anyOpen(state);
    if (!otherOpen && !frontList) {
        frontList = new SoGLDisplayList(state,
                                        SoGLDisplayList::DISPLAY_LIST,
                                        outlines.size());
        frontList->ref();
    }
    if (frontList) {
        // Set correct list base
        glListBase(frontList->getFirstIndex());
        frontList->addDependency(state);
    }
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Sets up for GL rendering.
//
// Use: internal

void
SoOutlineFontCache::setupToRenderSide(SoState *state, SbBool willTexture)
//
////////////////////////////////////////////////////////////////////////
{
    otherOpen = SoCacheElement::anyOpen(state);
    if (!otherOpen && !sideList) {
        sideList = new SoGLDisplayList(state,
                                        SoGLDisplayList::DISPLAY_LIST,
                                        outlines.size());
        sideList->ref();
        sidesHaveTexCoords = willTexture;
    }
    if (sideList) {
        // Set correct list base
        glListBase(sideList->getFirstIndex());
        sideList->addDependency(state);
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
SoOutlineFontCache::hasFrontDisplayList(const char c,
                                        GLUtesselator *tobj)
//
////////////////////////////////////////////////////////////////////////
{
    // If we have one, return TRUE
    if (frontFlags[c] == TRUE) return TRUE;

    // If we don't and we can't build one, return FALSE.
    if (otherOpen) return FALSE;

    // Build one:
    glNewList(frontList->getFirstIndex()+c, GL_COMPILE);
    generateFrontChar(c, tobj);
    const SbVec2f & t = getOutline(c)->getCharAdvance();
    glTranslatef(t[0], t[1], 0.0);
    glEndList();
    frontFlags[c] = TRUE;

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
SoOutlineFontCache::hasSideDisplayList(const char c,
                                       SideCB callbackFunc)
//
////////////////////////////////////////////////////////////////////////
{
    // If we have one, return TRUE
    if (sideFlags[c] == TRUE) return TRUE;

    // If we don't and we can't build one, return FALSE.
    if (otherOpen) return FALSE;

    // Build one:
    glNewList(sideList->getFirstIndex()+c, GL_COMPILE);

    glBegin(GL_QUADS);    // Render as independent quads:
    generateSideChar(c, callbackFunc);
    glEnd();

    const SbVec2f & t = getOutline(c)->getCharAdvance();
    glTranslatef(t[0], t[1], 0.0);
    glEndList();
    sideFlags[c] = TRUE;

    return TRUE;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Assuming that there are display lists built for all the
//    characters in given string, render them using the GL's CallLists
//    routine.
//
// Use: internal

void
SoOutlineFontCache::callFrontLists(const SbString &string)
//
////////////////////////////////////////////////////////////////////////
{
    const char *str = string.getString();

    glCallLists(string.getLength(), GL_UNSIGNED_BYTE, str);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Assuming that there are display lists built for all the
//    characters in given string, render them using the GL's CallLists
//    routine.
//
// Use: internal

void
SoOutlineFontCache::callSideLists(const SbString &string)
//
////////////////////////////////////////////////////////////////////////
{
    const char *str = string.getString();

    glCallLists(string.getLength(), GL_UNSIGNED_BYTE, str);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Assuming that there are not display lists built for all the
//    characters in given string, render the string.
//
// Use: internal

void
SoOutlineFontCache::renderFront(const SbString &string,
                                GLUtesselator *tobj)
//
////////////////////////////////////////////////////////////////////////
{
    const char *str = string.getString();

    for (int i = 0; i < string.getLength(); i++) {
        if (frontFlags[str[i]]) {
            glCallList(frontList->getFirstIndex()+str[i]);
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
SoOutlineFontCache::renderSide(const SbString &string,
                               SideCB callbackFunc)
//
////////////////////////////////////////////////////////////////////////
{
    const char *str = string.getString();

    for (int i = 0; i < string.getLength(); i++) {
        if (sideFlags[str[i]]) {
            glCallList(sideList->getFirstIndex()+str[i]);
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

    for (int i = 0; i < outline->getNumOutlines(); i++) {
        // For each outline:

        int nOutline = outline->getNumVerts(i);

        SbVec2f *oVerts = new SbVec2f[nOutline];
        // Copy in verts so figureSegmentNorms can handle them..
        int j;
        for (j = 0; j < nOutline; j++) {
            oVerts[j] = outline->getVertex(i, j);
        }

        // First, figure out a set of normals for the outline:
        SbVec2f *oNorms = new SbVec2f[nOutline*2];
        figureSegmentNorms(oNorms, nOutline, oVerts, cosCreaseAngle, TRUE);

        // And appropriate texture coordinates:
        // Figure out T texture coordinates, which run along the
        // outline:
        float *tTexCoords = new float[nOutline+1];
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

        for (j = 0; j < nOutline; j++) {
            // New normals are calculated for both ends of this
            // segment, since the normals may or may not be shared
            // with the previous segment.
            fillBevelN(bevelN1, (int)(nProfileVerts-1)*2, profileNorms,
                       oNorms[j*2]);

            int j2 = (j+1)%nOutline;
            // fill out second bevel:
            fillBevel(s2, (int) nProfileVerts, profileVerts,
                      outline->getVertex(i,j2),
                      oNorms[j*2+1], oNorms[j2*2]);
            fillBevelN(bevelN2, (int)(nProfileVerts-1)*2, profileNorms,
                       oNorms[j*2+1]);

            // And generate triangles between the two bevels:
            (*callbackFunc)((int) nProfileVerts, s1, bevelN1, s2, bevelN2,
                             sTexCoords, &tTexCoords[j]);

            // Swap bevel1/2 (avoids some recomputation)
            SbVec3f *t;
            t = s1; s1 = s2; s2 = t;
        }
        delete [] bevelN2;
        delete [] bevel2;
        delete [] bevelN1;
        delete [] bevel1;
        delete [] tTexCoords;
        delete [] oNorms;
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
SoOutlineFontCache::figureSegmentNorms(SbVec2f *norms, int nPoints,
                            const SbVec2f *points,  float cosCreaseAngle,
                            SbBool isClosed)
//
////////////////////////////////////////////////////////////////////////
{
    int nSegments;

    if (isClosed) nSegments = nPoints;
    else nSegments = nPoints-1;

    // First, we'll just make all the normals perpendicular to their
    // segments:
    int i;
    for (i = 0; i < nSegments; i++) {
        SbVec2f n;
        // This is 2D perpendicular, assuming profile is increasing in
        // X (which becomes 'decreasing in Z' when we actually use
        // it...) (note: if a profile isn't increasing in X, the
        // character will be inside-out, with the front face drawn
        // behind the back face, etc).
        SbVec2f v = points[(i+1)%nPoints] - points[i];
        n[0] = v[1];
        n[1] = -v[0];
        n.normalize();

        norms[i*2] = n;
        norms[i*2+1] = n;
    }
    // Now, figure out if the angle between any two segments is small
    // enough to average two of their normals.
    for (i = 0; i < (isClosed ? nSegments : nSegments-1); i++) {
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
SoOutlineFontCache::figureSegmentTexCoords(float *texCoords, int nPoints,
                            const SbVec2f *points, SbBool isClosed)
//
////////////////////////////////////////////////////////////////////////
{
    float total = 0.0;

    int i;

    if (isClosed) {
        for (i = nPoints; i >= 0; i--) {
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
SoOutlineFontCache::fillBevelN(SbVec3f *result, int nNorms,
          const SbVec2f *norms,
          const SbVec2f &n)
//
////////////////////////////////////////////////////////////////////////
{
    // Now, for each point:
    for (int i = 0; i < nNorms; i++) {
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
