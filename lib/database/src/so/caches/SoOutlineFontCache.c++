/*
 *
 *  Copyright (C) 2000 Silicon Graphics, Inc.  All Rights Reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  Further, this software is distributed without any warranty that it is
 *  free of the rightful claim of any third person regarding infringement
 *  or the like.  Any license provided herein, whether implied or
 *  otherwise, applies only to this software file.  Patent licenses, if
 *  any, provided herein do not apply to combinations of this program with
 *  other software, or any other product whatsoever.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *  Contact information: Silicon Graphics, Inc., 1600 Amphitheatre Pkwy,
 *  Mountain View, CA  94043, or:
 *
 *  http://www.sgi.com
 *
 *  For further information regarding this notice, see:
 *
 *  http://oss.sgi.com/projects/GenInfo/NoticeExplan/
 *
 */

/*
 * Copyright (C) 1990,91   Silicon Graphics, Inc.
 *
 _______________________________________________________________________
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 |
 |   $Revision: 1.10 $
 |
 |   Classes:
 |      SoFontOutline
 |      SoOutlineFontCache
 |
 |
 |   Author(s)          : Gavin Bell
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

#include <cstring>
#include <cmath>
#include <algorithm>
#include <machine.h>
#include <Inventor/caches/SoOutlineFontCache.h>
#include <Inventor/elements/SoCacheElement.h>
#include <Inventor/elements/SoCreaseAngleElement.h>
#include <Inventor/elements/SoFontSizeElement.h>
#include <Inventor/elements/SoGLDisplayList.h>
#include <Inventor/elements/SoGLCacheContextElement.h>
#include <Inventor/elements/SoGLTextureEnabledElement.h>
#include <Inventor/elements/SoProfileElement.h>
#include <Inventor/elements/SoProfileCoordinateElement.h>
#include <Inventor/nodes/SoProfile.h>
#include <GL/glu.h>

#include "ft2build.h"
#include FT_FREETYPE_H
#include FT_OUTLINE_H

SbBool SoOutlineFontCache::tesselationError = FALSE;
std::vector<SoOutlineFontCache*> SoOutlineFontCache::fonts;

// First, a more convenient structure for outlines:
class SoFontOutline {

  public:
    // Constructor, takes a pointer to the font-library outline
    // structure and the font's size:
    SoFontOutline(wchar_t ch, FT_Face face, float fontSize);
    // Destructor
    ~SoFontOutline();

    // Query routines:
    size_t	    getNumOutlines() const { return verts.size(); }
    size_t	    getNumVerts(size_t i) const { return verts[i].size(); }
    const SbVec2f & getVertex(size_t i, size_t j) const { return verts[i][j]; }
    const SbVec2f & getCharAdvance() const { return charAdvance; }
    const SbBox2f & getBoundingBox() const { return bbox;}

  private:
    static int moveTo(FT_Vector *to, SoFontOutline* fo);
    static int lineTo(FT_Vector *to, SoFontOutline* fo);
    static int conicTo(FT_Vector *control, FT_Vector *to, SoFontOutline* fo);
    static int cubicTo(FT_Vector *control1, FT_Vector *control2, FT_Vector *to, SoFontOutline* fo);

    // This basically mimics the FLoutline structure, with the
    // exception that the font size is part of the outline:
    float scale;
    std::vector< std::vector<SbVec2f> > verts;
    SbVec2f charAdvance;
    SbBox2f bbox;
};

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
    SoFontCache(state), context(-1)
//
////////////////////////////////////////////////////////////////////////
{
    // Add element dependencies explicitly here; making 'this' the
    // CacheElement doesn't work if we are being constructed in an
    // action that doesn't have caches.

    // Remember size
    fontSize = SoFontSizeElement::get(state);

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

    // compute profile bbox.
    for (int i = 0; i < nProfileVerts; i++) {
        profileBBox.extendBy(profileVerts[i]);
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
            std::map<wchar_t, SoFontOutline*>::iterator it;
            for (it=outlines.begin(); it!=outlines.end(); ++it) {
                delete it->second;
            }
            outlines.clear();
        }

        std::map<wchar_t, SoGLDisplayList*>::iterator it;
        for (it=frontList.begin(); it!=frontList.end(); ++it) {
            it->second->unref(NULL);
        }
        for (it=sideList.begin(); it!=sideList.end(); ++it) {
            it->second->unref(NULL);
        }

        if (hasProfile()) {
            delete[] profileVerts;
        }

        // Only destroy the font library font if no other font caches
        // are using the same font identifier:
        SbBool otherUsing = FALSE;
        for (size_t i = 0; i < fonts.size(); ++i) {
            SoOutlineFontCache *t = fonts[i];
            if (t != this && t->face == face) otherUsing = TRUE;
        }
        if (!otherUsing) {
            FT_Done_Face(face);
            face = NULL;
        }
        fonts.erase(std::find(fonts.begin(), fonts.end(), this));
    }
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Returns the width of the given string
//
// Use: private

float
SoOutlineFontCache::getWidth(const std::wstring &string)
//
////////////////////////////////////////////////////////////////////////
{
    float total = 0.0;
    for (size_t i = 0; i < string.size(); i++) {
        const SoFontOutline *outline = getOutline(string[i]);
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

SbBox2f
SoOutlineFontCache::getCharBBox(const wchar_t c)
//
////////////////////////////////////////////////////////////////////////
{
    return getOutline(c)->getBoundingBox();
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
SoOutlineFontCache::getOutline(const wchar_t c)
//
////////////////////////////////////////////////////////////////////////
{
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
SoOutlineFontCache::getCharOffset(const wchar_t c)
//
////////////////////////////////////////////////////////////////////////
{
    if (!face)
        return SbVec2f(0,0);

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
SoOutlineFontCache::generateFrontChar(const wchar_t c,
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
        SbBox2f charBBox = getCharBBox(c);
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
SoOutlineFontCache::hasFrontDisplayList(SoState *state, const wchar_t c, GLUtesselator *tobj)
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
    SoGLDisplayList *displaylist = new SoGLDisplayList(state, SoGLDisplayList::DISPLAY_LIST);
    frontList[c] = displaylist;
    displaylist->ref();
    displaylist->addDependency(state);
    glNewList(displaylist->getFirstIndex(), GL_COMPILE);

    generateFrontChar(c, tobj);
    const SbVec2f & t = getOutline(c)->getCharAdvance();
    glTranslatef(t[0], t[1], 0.0);
    displaylist->close(state);

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
SoOutlineFontCache::hasSideDisplayList(SoState *state, const wchar_t c, SideCB callbackFunc)
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
    SoGLDisplayList *displaylist = new SoGLDisplayList(state, SoGLDisplayList::DISPLAY_LIST);
    sideList[c] = displaylist;
    displaylist->ref();
    displaylist->addDependency(state);
    glNewList(displaylist->getFirstIndex(), GL_COMPILE);

    glBegin(GL_QUADS);    // Render as independent quads:
    generateSideChar(c, callbackFunc);
    glEnd();

    const SbVec2f & t = getOutline(c)->getCharAdvance();
    glTranslatef(t[0], t[1], 0.0);
    displaylist->close(state);

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
SoOutlineFontCache::renderFront(SoState *state, const SbString &string)
//
////////////////////////////////////////////////////////////////////////
{
    static GLUtesselator *tobj = NULL;
    if (tobj == NULL) {
        tobj = (GLUtesselator *)gluNewTess();
        gluTessCallback(tobj, (GLenum)GLU_TESS_BEGIN, (void (*)())glBegin);
        gluTessCallback(tobj, (GLenum)GLU_TESS_END, (void (*)())glEnd);
        gluTessCallback(tobj, (GLenum)GLU_TESS_VERTEX, (void (*)())glVertex2fv);
        gluTessCallback(tobj, (GLenum)GLU_TESS_ERROR, (void (*)())SoOutlineFontCache::errorCB);
    }

    const std::wstring str = string.toStdWString();
    for (size_t i = 0; i < str.size(); i++) {
        if (hasFrontDisplayList(state, str[i], tobj)) {
            frontList[str[i]]->call(state);
        } else {
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
    const std::wstring str = string.toStdWString();
    for (size_t i = 0; i < str.size(); i++) {
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
//   Copy info from the font library into a more convenient form.
//
// Use: internal

SoFontOutline::SoFontOutline(wchar_t ch, FT_Face face, float size)
//
////////////////////////////////////////////////////////////////////////
{
    FT_UInt index = FT_Get_Char_Index(face, ch);
    if(FT_Load_Glyph(face, index, FT_LOAD_IGNORE_TRANSFORM | FT_LOAD_NO_SCALE)) {
#ifdef DEBUG
        SoDebugError::post("SoFontOutline",
                           "FT_Load_Glyph failed");
#endif
    }

    scale = size / (float)face->units_per_EM;

    if (index == 0) {
        verts.resize(1);
        verts[0].push_back(SbVec2f(0.0f, 0.0f)*size);
        verts[0].push_back(SbVec2f(1.0f, 0.0f)*size);
        verts[0].push_back(SbVec2f(1.0f, 1.0f)*size);
        verts[0].push_back(SbVec2f(0.0f, 1.0f)*size);

        charAdvance = SbVec2f(1.0f, 0.0f);
    } else {
        static FT_Outline_Funcs ft2_outline_funcs = {
            (FT_Outline_MoveTo_Func)moveTo,
            (FT_Outline_LineTo_Func)lineTo,
            (FT_Outline_ConicTo_Func)conicTo,
            (FT_Outline_CubicTo_Func)cubicTo,
            0,0
        };

        FT_Outline *outline = &face->glyph->outline;

        verts.reserve(outline->n_contours);

        charAdvance = SbVec2f(face->glyph->advance.x,
                              face->glyph->advance.y)*scale;

        FT_Outline_Decompose (outline, &ft2_outline_funcs, this);

        // reverse the contours if necessary
        if (!(outline->flags & FT_OUTLINE_REVERSE_FILL)) {
            for (size_t ctr=0; ctr<verts.size(); ctr++) {
                std::reverse(verts[ctr].begin(), verts[ctr].end());
            }
        }
        for (size_t ctr=0; ctr<verts.size(); ctr++) {
            if (verts[ctr].front().equals(verts[ctr].back(), 10E-6)) {
                verts[ctr].pop_back();
            }
        }
    }

    bbox.makeEmpty();
    for (size_t ctr=0; ctr<verts.size(); ctr++) {
        for (size_t v=0; v<verts[ctr].size(); v++) {
            bbox.extendBy(verts[ctr][v]);
        }
    }
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//   Destructor; free up outline storage
//
// Use: internal

SoFontOutline::~SoFontOutline()
//
////////////////////////////////////////////////////////////////////////
{

}

/*
 * This function injects a new contour in the render pool.
 */
int
SoFontOutline::moveTo (FT_Vector *to, SoFontOutline* fo)
{
    fo->verts.push_back(std::vector<SbVec2f>());
    fo->verts.back().push_back(SbVec2f(to->x, to->y)*fo->scale);

    return 0;
}

/*
 * This function injects a new line segment in the render pool and
 * adjusts the faces list accordingly.
 */
int
SoFontOutline::lineTo (FT_Vector *to, SoFontOutline* fo)
{
    std::vector<SbVec2f> & ctr = fo->verts.back();

    ctr.push_back(SbVec2f(to->x, to->y)*fo->scale);

    return 0;
}

//
// Injects a new conic Bezier arc and adjusts the face list
// accordingly.
//
int
SoFontOutline::conicTo (FT_Vector *control, FT_Vector *to, SoFontOutline* fo)
{
    std::vector<SbVec2f> & ctr = fo->verts.back();

    SbVec2f p1 = ctr.back();
    SbVec2f p2 = SbVec2f(control->x, control->y)*fo->scale;
    SbVec2f p3 = SbVec2f(to->x, to->y)*fo->scale;

    int num_steps = 15;
    for(int i=1; i<num_steps; i++) {
        float mu = i/(float)num_steps;
        float mu2 = mu * mu;
        float mum1 = 1 - mu;
        float mum12 = mum1 * mum1;

        ctr.push_back(mum12 * p1 + 2*mu*mum1 * p2 + mu2 * p3);
    }
    return 0;
}

/*
 * Injects a new cubic Bezier arc and adjusts the face list
 * accordingly.
 */
int
SoFontOutline::cubicTo (FT_Vector *control1,
                         FT_Vector *control2,
                         FT_Vector *to,
                         SoFontOutline* fo)
{
    std::vector<SbVec2f> & ctr = fo->verts.back();

    SbVec2f p1 = ctr.back();
    SbVec2f p2 = SbVec2f(control1->x, control1->y)*fo->scale;
    SbVec2f p3 = SbVec2f(control2->x, control2->y)*fo->scale;
    SbVec2f p4 = SbVec2f(to->x, to->y)*fo->scale;

    int num_steps = 15;
    for (int i=1; i<num_steps; i++) {
        float mu = i/(float)num_steps;
        float mum1 = 1 - mu;
        float mum13 = mum1 * mum1 * mum1;
        float mu3 = mu * mu * mu;
        ctr.push_back(mum13*p1 + 3*mu*mum1*mum1*p2 + 3*mu*mu*mum1*p3 + mu3*p4);
    }

    return 0;
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
//    Called by generateSide.  This generates the bevel triangles for
//    one character.
//
// Use: private

void
SoOutlineFontCache::generateSideChar(const wchar_t c, SideCB callbackFunc)
//
////////////////////////////////////////////////////////////////////////
{
    if (!hasProfile())
        return;

    // Get the outline of the character:
    SoFontOutline *outline = getOutline(c);

    for (size_t i = 0; i < outline->getNumOutlines(); i++) {
        // For each outline:

        const size_t nOutline = outline->getNumVerts(i);

        std::vector<SbVec2f> oVerts(nOutline);
        // Copy in verts so figureSegmentNorms can handle them..
        for (size_t j = 0; j < nOutline; j++) {
            oVerts[j] = outline->getVertex(i, j);
        }

        // First, figure out a set of normals for the outline:
        std::vector<SbVec2f> oNorms(nOutline*2);
        figureSegmentNorms(oNorms, nOutline, oVerts.data(), cosCreaseAngle, TRUE);

        // And appropriate texture coordinates:
        // Figure out T texture coordinates, which run along the
        // outline:
        std::vector<float> tTexCoords(nOutline+1);
        figureSegmentTexCoords(tTexCoords, nOutline, oVerts.data(), TRUE);

        // Now, generate a set of triangles for each segment in the
        // outline.  A bevel of profiles is built at each point in the
        // outline; each profile must be flipped perpendicular to the
        // outline (x coordinate becomes -z), rotated to be the
        // average of the normals of the two adjoining segments at
        // that point, and translated to that point.  Triangles are
        // formed between consecutive bevels.
        // Normals are just taken from the 'pNorms' array, after being
        // rotated the appropriate amount.

        std::vector<SbVec3f> bevel1(nProfileVerts);
        std::vector<SbVec3f> bevelN1((nProfileVerts-1)*2);
        std::vector<SbVec3f> bevel2(nProfileVerts);
        std::vector<SbVec3f> bevelN2((nProfileVerts-1)*2);

        // fill out first bevel:
        fillBevel(bevel1.data(), (int) nProfileVerts, profileVerts,
                  outline->getVertex(i,0),
                  oNorms[(nOutline-1)*2+1], oNorms[0*2]);

        SbVec3f *s1 = bevel1.data();
        SbVec3f *s2 = bevel2.data();

        for (size_t j = 0; j < nOutline; j++) {
            // New normals are calculated for both ends of this
            // segment, since the normals may or may not be shared
            // with the previous segment.
            fillBevelN(bevelN1.data(), profileNorms, oNorms[j*2]);

            int j2 = (j+1)%nOutline;
            // fill out second bevel:
            fillBevel(s2, (int) nProfileVerts, profileVerts,
                      outline->getVertex(i,j2),
                      oNorms[j*2+1], oNorms[j2*2]);
            fillBevelN(bevelN2.data(), profileNorms, oNorms[j*2+1]);

            // And generate triangles between the two bevels:
            (*callbackFunc)((int) nProfileVerts, s1, bevelN1.data(), s2, bevelN2.data(),
                            &sTexCoords[0], &tTexCoords[j]);

            // Swap bevel1/2 (avoids some recomputation)
            SbVec3f *t;
            t = s1; s1 = s2; s2 = t;
        }
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
    const int nSegments = isClosed ? nPoints : nPoints-1;

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
