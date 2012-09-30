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
 |   $Revision: 1.6 $
 |
 |   Classes:
 |      SoText2
 |
 |   Author(s)          : Gavin Bell
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

#include <Inventor/misc/SoGL.h>
#include <Inventor/SbBox3f.h>
#include <Inventor/SbMatrix.h>
#include <Inventor/SbViewportRegion.h>
#include <Inventor/SoPickedPoint.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoRayPickAction.h>
#include <Inventor/bundles/SoMaterialBundle.h>
#include <Inventor/details/SoTextDetail.h>
#include <Inventor/elements/SoGLLazyElement.h>
#include <Inventor/elements/SoFontSizeElement.h>
#include <Inventor/elements/SoGLCacheContextElement.h>
#include <Inventor/elements/SoGLTextureEnabledElement.h>
#include <Inventor/elements/SoModelMatrixElement.h>
#include <Inventor/elements/SoProjectionMatrixElement.h>
#include <Inventor/elements/SoViewingMatrixElement.h>
#include <Inventor/elements/SoViewportRegionElement.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/nodes/SoText2.h>
#include <Inventor/caches/SoBitmapFontCache.h>

SO_NODE_SOURCE(SoText2);

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Constructor
//
// Use: public

SoText2::SoText2()
//
////////////////////////////////////////////////////////////////////////
{
    SO_NODE_CONSTRUCTOR(SoText2);

    SO_NODE_ADD_FIELD(string,	(""));
    SO_NODE_ADD_FIELD(spacing,	(1.));
    SO_NODE_ADD_FIELD(justification,	(LEFT));

    // Set up static info for enumerated type field
    SO_NODE_DEFINE_ENUM_VALUE(Justification, LEFT);
    SO_NODE_DEFINE_ENUM_VALUE(Justification, RIGHT);
    SO_NODE_DEFINE_ENUM_VALUE(Justification, CENTER);

    // Set up info in enumerated type field
    SO_NODE_SET_SF_ENUM_TYPE(justification, Justification);

    isBuiltIn = TRUE;
    fontCache = NULL;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Destructor
//
// Use: private

SoText2::~SoText2()
//
////////////////////////////////////////////////////////////////////////
{
    if (fontCache != NULL) fontCache->unref();
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    This initializes the SoText2 class.
//
// Use: internal

void
SoText2::initClass()
//
////////////////////////////////////////////////////////////////////////
{
    SO__NODE_INIT_CLASS(SoText2, "Text2", SoShape);

    // Font size/name are enabled by SoFont node
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Static helper routine; given a vector in object space, a
//    transformation matrix to screen (normalized-device coordinate)
//    space, and a viewportRegion, returns the 2D pixel coordinates of
//    the vector (relative to the origin of the viewport).  This
//    actually returns a 3D vector; the z value is just the NDC z value.
//
// Use: internal, static

static SbVec3f
fromObjectSpace(const SbVec3f &vector, const SbMatrix &matrix,
                const SbViewportRegion &vpr)
//
////////////////////////////////////////////////////////////////////////
{
    // First, transform to NDC (-1 to 1 in viewport)
    SbVec3f ndc;
    matrix.multVecMatrix(vector, ndc);

    // And do the viewport transformation:
    SbVec2s vpSize = vpr.getViewportSizePixels();
    SbVec3f result;
    result[0] = (ndc[0]+1.0f)*vpSize[0]/2.0f;
    result[1] = (ndc[1]+1.0f)*vpSize[1]/2.0f;
    // Leave the z coordinate alone
    result[2] = ndc[2];

    return result;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Static helper routine; given 2D pixel coordinates, a
//    transformation matrix to object space, and a viewportRegion,
//    return the object-space point (assuming the 2D pixel is at z
//    coordinate 0).
//
// Use: internal, static

static SbVec3f
toObjectSpace(const SbVec3f &pixel, const SbMatrix &matrix,
              const SbViewportRegion &vpr)
//
////////////////////////////////////////////////////////////////////////
{
    // Viewport transformation, to normalized device coordinates:
    SbVec2s vpSize = vpr.getViewportSizePixels();
    SbVec3f ndc;
    ndc[0] = pixel[0]*2.0f/vpSize[0] - 1.0f;
    ndc[1] = pixel[1]*2.0f/vpSize[1] - 1.0f;
    ndc[2] = pixel[2];

    SbVec3f result;
    matrix.multVecMatrix(ndc, result);

    return result;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    render the text with the current font
//
// Use: protected

void
SoText2::GLRender(SoGLRenderAction *action)
//
////////////////////////////////////////////////////////////////////////
{
    if (!shouldGLRender(action)) return;

    SoState *state = action->getState();

    state->push();

    // Get a font cache we can pull stuff out of
    if (fontCache != NULL) {
        if (!fontCache->isRenderValid(state)) {
            fontCache->unref(state);
            fontCache = NULL;
        }
    }
    if (fontCache == NULL) {
        fontCache = SoBitmapFontCache::getFont(state, TRUE);
        if (fontCache == NULL) {
            state->pop();
            return;
        }
    }

    // Turn off lighting
    SoGLLazyElement::setLightModel(state, SoGLLazyElement::BASE_COLOR);
    // Turn off texturing
    SoGLTextureEnabledElement::set(state, FALSE);

    // Send first color
    SoMaterialBundle mb(action);
    mb.sendFirst();
    
    const SbColor &	color = SoGLLazyElement::getDiffuse(state, 0);

    glPushAttrib(GL_PIXEL_MODE_BIT | GL_COLOR_BUFFER_BIT);

    glPixelTransferf(GL_RED_SCALE,   color[0]);
    glPixelTransferf(GL_GREEN_SCALE, color[1]);
    glPixelTransferf(GL_BLUE_SCALE,  color[2]);

    // Special-case left-justified, single-line text, which we know
    // starts at (0,0,0) in object space, so we can help caching by
    // avoiding getting the projection/view/model matrices:
    if (string.getNum() == 1 && justification.getValue() == LEFT) {
        glRasterPos3f(0,0,0);
        fontCache->drawString(state, string[0]);
    }
    // General case:
    else {
        SbMatrix objToScreen;
        objToScreen = SoProjectionMatrixElement::get(state);
        objToScreen = objToScreen.multLeft(SoViewingMatrixElement::get(state));
        objToScreen = objToScreen.multLeft(SoModelMatrixElement::get(state));

        SbMatrix screenToObj = objToScreen.inverse();

        SbViewportRegion vpr = SoViewportRegionElement::get(state);

        // The origin of the text on the screen is the object-space point
        // 0,0,0:
        SbVec3f screenOrigin = fromObjectSpace(SbVec3f(0,0,0), objToScreen, vpr);

        for (int line = 0; line < string.getNum(); line++) {
            // Starting position of string, based on justification:
            SbVec3f charPosition = getPixelStringOffset(line) + screenOrigin;

            const SbString &str = string[line];

            // Transform the screen-space starting position into object
            // space, and feed that back to the glRasterPos command (which
            // will turn around and transform it back into screen-space,
            // but oh well).
            SbVec3f lineOrigin = toObjectSpace(charPosition, screenToObj, vpr);
            glRasterPos3fv(&lineOrigin[0]);

            fontCache->drawString(state, str);
        }
        // Don't auto-cache above, since dependent on camera:
        SoGLCacheContextElement::shouldAutoCache(state, SoGLCacheContextElement::DONT_AUTO_CACHE);
    }
    glPopAttrib();
    state->pop();
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Implements ray picking
//
// Use: protected

void
SoText2::rayPick(SoRayPickAction *action)
//
////////////////////////////////////////////////////////////////////////
{
    // First see if the object is pickable
    if (! shouldRayPick(action))
        return;

    SoState *state = action->getState();

    state->push();

    // Get a font cache we can pull stuff out of
    if (fontCache != NULL) {
        if (!fontCache->isValid(state)) {
            fontCache->unref();
            fontCache = NULL;
        }
    }
    if (fontCache == NULL) {
        fontCache = SoBitmapFontCache::getFont(state, FALSE);
        if (fontCache == NULL) {
            state->pop();
            return;
        }
    }

    // Tell the action about our current object space
    action->setObjectSpace();

    SbMatrix objToScreen;
    objToScreen = SoProjectionMatrixElement::get(state);
    objToScreen = objToScreen.multLeft(SoViewingMatrixElement::get(state));
    objToScreen = objToScreen.multLeft(SoModelMatrixElement::get(state));

    SbMatrix screenToObj = objToScreen.inverse();

    SbViewportRegion vpr = SoViewportRegionElement::get(state);

    // The origin of the text on the screen is the object-space point
    // 0,0,0:
    SbVec3f screenOrigin = fromObjectSpace(SbVec3f(0,0,0), objToScreen, vpr);

    for (int line = 0; line < string.getNum(); line++) {

        // Intersect against each line of text's bounding box:
        SbBox3f lineBbox, charBbox;

        // Starting position of string, based on justification:
        SbVec3f charPosition = getPixelStringOffset(line) + screenOrigin;

        const SbString &str = string[line];
        const char *chars = str.getString();

        SbVec3f p0, p1, p2, p3;
        size_t chr;
        for (size_t chr = 0; chr < str.getLength(); chr++) {
            fontCache->getCharBbox(chars[chr], charBbox);

            if (!charBbox.isEmpty()) {

                SbVec3f charMin = charBbox.getMin() + charPosition;
                SbVec3f charMax = charBbox.getMax() + charPosition;

                // Extend the line's bounding box by this character's
                // bounding box (both boxes are in screen-space):
                lineBbox.extendBy(charMin);
                lineBbox.extendBy(charMax);

                // Advance to next character...
                charPosition += fontCache->getCharOffset(chars[chr]);
            }
        }
        // And transform line's box into object space:
        SbVec3f min = lineBbox.getMin();
        SbVec3f max = lineBbox.getMax();

        SbVec3f t;
        t.setValue(min[0], min[1], screenOrigin[2]);
        p0 = toObjectSpace(t, screenToObj, vpr);
        t.setValue(max[0], min[1], screenOrigin[2]);
        p1 = toObjectSpace(t, screenToObj, vpr);
        t.setValue(min[0], max[1], screenOrigin[2]);
        p2 = toObjectSpace(t, screenToObj, vpr);
        t.setValue(max[0], max[1], screenOrigin[2]);
        p3 = toObjectSpace(t, screenToObj, vpr);

        // intersect the two triangles:
        SbVec3f point;
        // Info we get back from the pick that we don't need:
        SbVec3f junk1; SbBool junk2;
        if (action->intersect(p0, p1, p2, point, junk1, junk2) ||
                action->intersect(p2, p1, p3, point, junk1, junk2)) {

            SoPickedPoint *pp = action->addIntersection(point);
            if (pp) {
                SoTextDetail *detail = new SoTextDetail();
                detail->setStringIndex(line);

                // Figure out which character was hit:
                // Transform picked point into screen space:
                SbVec3f screenPoint =
                        fromObjectSpace(pp->getObjectPoint(), objToScreen,
                                        vpr);
                // Figure out which character that corresponds to, by
                // adding on the x-offset of each character until we
                // go past the picked point:
                charPosition = getPixelStringOffset(line) + screenOrigin;
                for (chr = 0; chr < str.getLength(); chr++) {
                    charPosition += fontCache->getCharOffset(chars[chr]);
                    // Assuming left-to-right drawing of characters:
                    if (charPosition[0] >= screenPoint[0]) break;
                }

                detail->setCharacterIndex(chr);

                pp->setDetail(detail, this);
                pp->setMaterialIndex(0);

                // We'll define normal to be object-space 0,0,1:
                pp->setObjectNormal(SbVec3f(0,0,1));
                // And texture coordinates to be zero:
                pp->setObjectTextureCoords(SbVec4f(0,0,0,0));
            }
        }
    }
    state->pop();
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Generates primitives - no-op.
//
// Use: protected

void
SoText2::generatePrimitives(SoAction *)
//
////////////////////////////////////////////////////////////////////////
{
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Computes bounding box of text. Since 2D text is screen-aligned,
//    we need to look at the current view volume and viewport region.
//
// Use: protected

void
SoText2::computeBBox(SoAction *action, SbBox3f &box, SbVec3f &center)
//
////////////////////////////////////////////////////////////////////////
{
    SoState *state = action->getState();

    state->push();

    // Get a font cache we can pull stuff out of
    if (fontCache != NULL) {
        if (!fontCache->isValid(state)) {
            fontCache->unref();
            fontCache = NULL;
        }
    }
    if (fontCache == NULL) {
        fontCache = SoBitmapFontCache::getFont(state, FALSE);
        if (fontCache == NULL) {
            state->pop();
            return;
        }
    }

    SbMatrix objToScreen;
    objToScreen = SoProjectionMatrixElement::get(state);
    objToScreen = objToScreen.multLeft(SoViewingMatrixElement::get(state));
    objToScreen = objToScreen.multLeft(SoModelMatrixElement::get(state));

    SbMatrix screenToObj = objToScreen.inverse();

    SbViewportRegion vpr = SoViewportRegionElement::get(state);

    // The origin of the text on the screen is the object-space point
    // 0,0,0:
    SbVec3f screenOrigin = fromObjectSpace(SbVec3f(0,0,0), objToScreen, vpr);

    // Figure out the screen-space bounding box of the characters:
    SbBox3f screenBbox, charBbox;

    for (int line = 0; line < string.getNum(); line++) {
        // Starting position of string, based on justification:
        SbVec3f charPosition = getPixelStringOffset(line) + screenOrigin;

        const SbString &str = string[line];
        const char *chars = str.getString();

        for (size_t chr = 0; chr < str.getLength(); chr++) {
            fontCache->getCharBbox(chars[chr], charBbox);
            if (!charBbox.isEmpty()) {
                SbVec3f min = charBbox.getMin() + charPosition;
                SbVec3f max = charBbox.getMax() + charPosition;
                screenBbox.extendBy(min);
                screenBbox.extendBy(max);
            }

            // And advance...
            charPosition += fontCache->getCharOffset(chars[chr]);
        }
    }
    // Ok, screenBbox now contains the pixel-space extent of the
    // characters.  We'll transform the bounds of that box back into
    // object space and extend the object-space bounding box:
    
    if (!screenBbox.isEmpty()) {
        // Do each of the 4 corners of the screen-space box:
        const SbVec3f &min = screenBbox.getMin();
        const SbVec3f &max = screenBbox.getMax();
        SbVec3f objectPoint, temp;

        temp.setValue(min[0], min[1], screenOrigin[2]);
        objectPoint = toObjectSpace(temp, screenToObj, vpr);
        box.extendBy(objectPoint);

        temp.setValue(max[0], max[1], screenOrigin[2]);
        objectPoint = toObjectSpace(temp, screenToObj, vpr);
        box.extendBy(objectPoint);

        temp.setValue(min[0], max[1], screenOrigin[2]);
        objectPoint = toObjectSpace(temp, screenToObj, vpr);
        box.extendBy(objectPoint);

        temp.setValue(max[0], min[1], screenOrigin[2]);
        objectPoint = toObjectSpace(temp, screenToObj, vpr);
        box.extendBy(objectPoint);

        // Set the center to be the origin, which is the natural "center"
        // of the text, regardless of justification
        center.setValue(0.0, 0.0, 0.0);
    }

    state->pop();
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Returns the number of pixels the given line is offset from the
//    text origin.
//
// Use: private

SbVec3f
SoText2::getPixelStringOffset(int line)
//
////////////////////////////////////////////////////////////////////////
{
    SbVec3f result(0,0,0);
    
    if (justification.getValue() == RIGHT) {
        float width = fontCache->getWidth(string[line]);
        result[0] = -width;
    }
    if (justification.getValue() == CENTER) {
        float width = fontCache->getWidth(string[line]);
        result[0] = -width/2.0f;
    }
    result[1] = -line*fontCache->getHeight()*spacing.getValue()*2;

    return result;
}
