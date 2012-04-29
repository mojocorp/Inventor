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
 |      SoText3
 |
 |
 |   Author(s)          : Gavin Bell
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

#include <Inventor/misc/SoGL.h>
#include <Inventor/SbBox2f.h>
#include <Inventor/SbBox3f.h>
#include <Inventor/SoPrimitiveVertex.h>
#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoRayPickAction.h>
#include <Inventor/bundles/SoMaterialBundle.h>
#include <Inventor/details/SoTextDetail.h>
#include <Inventor/elements/SoGLTextureCoordinateElement.h>
#include <Inventor/elements/SoGLTextureEnabledElement.h>
#include <Inventor/elements/SoMaterialBindingElement.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/misc/SoState.h>
#include <Inventor/nodes/SoText3.h>
#include <Inventor/caches/SoOutlineFontCache.h>

// Static stuff is used while generating primitives:
SoText3 *SoText3::currentGeneratingNode = NULL;
SoPrimitiveVertex *SoText3::genPrimVerts[3];
SbVec3f SoText3::genTranslate;
int SoText3::genWhichVertex = -1;
uint32_t SoText3::genPrimType;
SoAction *SoText3::genAction = NULL;
SbBool SoText3::genBack = FALSE;
SbBool SoText3::genTexCoord = TRUE;
const SoTextureCoordinateElement *SoText3::tce = NULL;

SO_NODE_SOURCE(SoText3);

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Constructor
//
// Use: public

SoText3::SoText3()
//
////////////////////////////////////////////////////////////////////////
{
    SO_NODE_CONSTRUCTOR(SoText3);

    SO_NODE_ADD_FIELD(string,	(""));
    SO_NODE_ADD_FIELD(spacing,	(1.0));
    SO_NODE_ADD_FIELD(justification,	(LEFT));
    SO_NODE_ADD_FIELD(parts,		(FRONT));

    // Set up static info for enumerated type field
    SO_NODE_DEFINE_ENUM_VALUE(Justification,	LEFT);
    SO_NODE_DEFINE_ENUM_VALUE(Justification,	RIGHT);
    SO_NODE_DEFINE_ENUM_VALUE(Justification,	CENTER);

    // Set up static info for enumerated type field
    SO_NODE_DEFINE_ENUM_VALUE(Part, SIDES);
    SO_NODE_DEFINE_ENUM_VALUE(Part, FRONT);
    SO_NODE_DEFINE_ENUM_VALUE(Part, BACK);
    SO_NODE_DEFINE_ENUM_VALUE(Part, ALL);

    // Set up info in enumerated type field
    SO_NODE_SET_SF_ENUM_TYPE(justification, Justification);
    SO_NODE_SET_SF_ENUM_TYPE(parts, Part);

    isBuiltIn = TRUE;
    myFont = NULL;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Destructor
//
// Use: private

SoText3::~SoText3()
//
////////////////////////////////////////////////////////////////////////
{
    if (myFont != NULL) myFont->unref();
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    This initializes the SoText3 class.
//
// Use: internal

void
SoText3::initClass()
//
////////////////////////////////////////////////////////////////////////
{
    SO__NODE_INIT_CLASS(SoText3, "Text3", SoShape);

    // Font name and size elements are enabled by the SoFont node
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Figure out the bounds of a character.
//
// Use: extender

SbBox3f
SoText3::getCharacterBounds(SoState *state, int stringIndex, int charIndex)
//
////////////////////////////////////////////////////////////////////////
{
    SbBox3f result;

    if (!setupFontCache(state))
        return result;  // Empty bbox
    
#ifdef DEBUG
    if (stringIndex >= string.getNum()) {
        SoDebugError::post("SoText3::getCharacterBounds",
                           "stringIndex (%d) out of range (max %d)",
                           stringIndex, string.getNum());
    }
    if (charIndex >= string[stringIndex].getLength()) {
        SoDebugError::post("SoText3::getCharacterBounds",
                           "charIndex (%d) out of range (max %d)",
                           charIndex,
                           string[stringIndex].getLength());
    }
#endif

    float frontZ, backZ;
    myFont->getProfileBounds(frontZ, backZ);
    
    float height = myFont->getHeight();

    const char *chars = string[stringIndex].getString();
    float width = (myFont->getCharOffset(chars[charIndex]))[0];
    
    // Figure out where origin of character is:
    SbVec2f charPosition = getStringOffset(stringIndex);
    for (int i = 0; i < charIndex; i++) {
        charPosition += myFont->getCharOffset(chars[i]);
    }
    
    // Ok, have width, height, depth and starting position of text,
    // can create the bounds box:
    if (parts.getValue() & (FRONT|SIDES)) {
        result.extendBy(SbVec3f(charPosition[0],
                                charPosition[1],
                                frontZ));
        result.extendBy(SbVec3f(charPosition[0]+width,
                                charPosition[1],
                                frontZ));
        result.extendBy(SbVec3f(charPosition[0],
                                charPosition[1]+height,
                                frontZ));
        result.extendBy(SbVec3f(charPosition[0]+width,
                                charPosition[1]+height,
                                frontZ));
    }
    if (parts.getValue() & (BACK|SIDES)) {
        result.extendBy(SbVec3f(charPosition[0],
                                charPosition[1],
                                backZ));
        result.extendBy(SbVec3f(charPosition[0]+width,
                                charPosition[1],
                                backZ));
        result.extendBy(SbVec3f(charPosition[0],
                                charPosition[1]+height,
                                backZ));
        result.extendBy(SbVec3f(charPosition[0]+width,
                                charPosition[1]+height,
                                backZ));
    }
    return result;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Performs GL rendering of a Text3.
//
// Use: extender

void
SoText3::GLRender(SoGLRenderAction *action)
//
////////////////////////////////////////////////////////////////////////
{
    static GLUtesselator *tobj = NULL;

    // First see if the object is visible and should be rendered now
    if (! shouldGLRender(action))
        return;

    SoState *state = action->getState();

    if (!setupFontCache(state, TRUE))
        return;

    SoMaterialBindingElement::Binding mbe = SoMaterialBindingElement::get(state);
    SbBool materialPerPart = (mbe == SoMaterialBindingElement::PER_PART_INDEXED ||
                              mbe == SoMaterialBindingElement::PER_PART);

    SoMaterialBundle mb(action);
    if (!materialPerPart) {
        // Make sure the fist current material is sent to GL
        mb.sendFirst();
    }

    float firstZ, lastZ;
    myFont->getProfileBounds(firstZ, lastZ);

    if (tobj == NULL) {
        tobj = gluNewTess();
        gluTessCallback(tobj, (GLenum)GLU_BEGIN,  (void (CALLBACK*)())glBegin);
        gluTessCallback(tobj, (GLenum)GLU_END,    (void (CALLBACK*)())glEnd);
        gluTessCallback(tobj, (GLenum)GLU_VERTEX, (void (CALLBACK*)())glVertex2fv);
        gluTessCallback(tobj, (GLenum)GLU_ERROR,  (void (CALLBACK*)())SoOutlineFontCache::errorCB);
    }

    // See if texturing is enabled
    genTexCoord = SoGLTextureEnabledElement::get(action->getState());

    if ((parts.getValue() & SIDES) && (myFont->hasProfile())) {
        if (materialPerPart) mb.send(1, FALSE);

        myFont->setupToRenderSide(state, genTexCoord);
        for (int line = 0; line < string.getNum(); line++) {
            glPushMatrix();
            SbVec2f p = getStringOffset(line);
            if (p[0] != 0.0 || p[1] != 0.0)
                glTranslatef(p[0], p[1], 0.0);
            renderSide(action, line);
            glPopMatrix();
        }
    }
    if (parts.getValue() & BACK) {
        if (materialPerPart) mb.send(2, FALSE);

        if (lastZ != 0.0) {
            glTranslatef(0, 0, lastZ);
        }
        glNormal3f(0, 0, -1);
        glFrontFace(GL_CW);

        myFont->setupToRenderFront(state);

        if (genTexCoord) {
            glPushAttrib(GL_TEXTURE_BIT);
            glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
            glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
            GLfloat params[4];
            params[0] = -1.0f/myFont->getHeight();
            params[1] = params[2] = params[3] = 0.0;
            glTexGenfv(GL_S, GL_OBJECT_PLANE, params);
            params[1] = -params[0];
            params[0] = 0.0;
            glTexGenfv(GL_T, GL_OBJECT_PLANE, params);

            glEnable(GL_TEXTURE_GEN_S);
            glEnable(GL_TEXTURE_GEN_T);
        }

        for (int line = 0; line < string.getNum(); line++) {
            if (string[line].getLength() <= 0) continue;

            glPushMatrix();
            SbVec2f p = getStringOffset(line);
            if (p[0] != 0.0 || p[1] != 0.0)
                glTranslatef(p[0], p[1], 0.0);
            renderFront(action, line, tobj);
            glPopMatrix();
        }

        if (genTexCoord) {
            glPopAttrib();
        }

        glFrontFace(GL_CCW);

        if (lastZ != 0)
            glTranslatef(0, 0, -lastZ);
    }
    if (parts.getValue() & FRONT) {
        if (materialPerPart) mb.sendFirst();

        if (firstZ != 0.0) {
            glTranslatef(0, 0, firstZ);
        }

        glNormal3f(0, 0, 1);

        myFont->setupToRenderFront(state);

        if (genTexCoord) {
            glPushAttrib(GL_TEXTURE_BIT);
            glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
            glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
            GLfloat params[4];
            params[0] = 1.0f/myFont->getHeight();
            params[1] = params[2] = params[3] = 0.0;
            glTexGenfv(GL_S, GL_OBJECT_PLANE, params);
            params[1] = params[0];
            params[0] = 0.0;
            glTexGenfv(GL_T, GL_OBJECT_PLANE, params);

            glEnable(GL_TEXTURE_GEN_S);
            glEnable(GL_TEXTURE_GEN_T);
        }

        for (int line = 0; line < string.getNum(); line++) {
            glPushMatrix();
            SbVec2f p = getStringOffset(line);
            if (p[0] != 0.0 || p[1] != 0.0)
                glTranslatef(p[0], p[1], 0.0);
            renderFront(action, line, tobj);
            glPopMatrix();
        }

        if (genTexCoord) {
            glPopAttrib();
        }

        if (firstZ != 0.0) {
            glTranslatef(0, 0, -firstZ);
        }
    }
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Implements ray picking
//
// Use: extender

void
SoText3::rayPick(SoRayPickAction *action)
//
////////////////////////////////////////////////////////////////////////
{
    // First see if the object is pickable
    if (! shouldRayPick(action))
        return;

    //
    // NOTE: This could be made more efficient by testing the ray
    // against the bounding box of each character, as follows:
    //
    // 	-- For each string {
    // 	    -- For each character in the string: {
    // 		If the ray intersects the character's bounding box,
    // 		  generate primitives for that character
    // 	    }
    // 	}
    //
    // This is a project for the future... ???

    // For now, just use generatePrimitives() to do the work.
    SoShape::rayPick(action);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Computes bounding box of text.
//
// Use: protected

void
SoText3::computeBBox(SoAction *action, SbBox3f &box, SbVec3f &center)
//
////////////////////////////////////////////////////////////////////////
{
    // Set the center to be the origin, which is the natural "center"
    // of the text, regardless of justification
    center.setValue(0.0, 0.0, 0.0);

    int prts = parts.getValue();
    if (prts == 0)
        return;

    SoState *state = action->getState();

    if (!setupFontCache(state))
        return;

    // Get the bounding box of all the characters:
    SbBox2f outlineBox;
    getFrontBBox(outlineBox);

    // If no lines and no characters, return empty bbox:
    if (outlineBox.isEmpty()) return;
    
    // .. and extend it based on what parts are turned on:
    float firstZ, lastZ;
    myFont->getProfileBounds(firstZ, lastZ);

    const SbVec2f &boxMin = outlineBox.getMin();
    const SbVec2f &boxMax = outlineBox.getMax();


    // Front and back are straightforward:
    if (prts & FRONT) {
        SbVec3f vmin(boxMin[0], boxMin[1], firstZ);
        SbVec3f vmax(boxMax[0], boxMax[1], firstZ);
        box.extendBy(vmin);
        box.extendBy(vmax);
    }
    if (prts & BACK) {
        SbVec3f vmin(boxMin[0], boxMin[1], lastZ);
        SbVec3f vmax(boxMax[0], boxMax[1], lastZ);
        box.extendBy(vmin);
        box.extendBy(vmax);
    }
    //
    // Sides are trickier.  We figure out the maximum offset
    // of the profile we're using.  If the offset is
    // negative, we use its absolute value; normally, negative
    // (indented) characters won't expand the bounding box, but if the
    // offset is negative enough the character will inter-penetrate
    // itself-- think of a Helvetica 'I', with a bevel of a big
    // negative spike.  The bounding box is either the bounding box of
    // the front/back, or, if the spike is big enough, the size of the
    // spike minus the width of the I.  I'm being conservative here
    // and just expanding the front/back bounding boxes by the maximum
    // offset (correct for positive offsets, conservative for negative
    // offsets).
    //
    if ((prts & SIDES) && myFont->hasProfile()) {
        SbBox2f profileBox;
        myFont->getProfileBBox(profileBox);

        const SbVec2f &pBoxMin = profileBox.getMin();
        const SbVec2f &pBoxMax = profileBox.getMax();

        // If no profile, return the front/back bbox:
        if (profileBox.isEmpty()) return;

        //
        // Expand the bounding box forward/backward in case the
        // profile extends forwards/backwards:
        //
        SbVec3f min, max;
        min.setValue(boxMin[0], boxMin[1], pBoxMin[0]);
        max.setValue(boxMax[0], boxMax[1], pBoxMax[0]);
        box.extendBy(min);
        box.extendBy(max);

        //
        // And figure out the maximum profile offset, and expand
        // out the outline's bbox:
        //
# define max(a,b)               (a<b ? b : a)
# define abs(x)                 (x>=0 ? x : -(x))
        float maxOffset = max(abs(pBoxMin[1]), abs(pBoxMax[1]));
#undef max
#undef abs
        min.setValue(boxMin[0]-maxOffset, boxMin[1]-maxOffset, firstZ);
        max.setValue(boxMax[0]+maxOffset, boxMax[1]+maxOffset, lastZ);
        box.extendBy(min);
        box.extendBy(max);
    }
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Generates triangles representing a Text3.
//
// Use: protected

void
SoText3::generatePrimitives(SoAction *action)
//
////////////////////////////////////////////////////////////////////////
{
    SoState *state = action->getState();

    if (!setupFontCache(state))
        return;

    currentGeneratingNode = this;

    // Set up default texture coordinate mapping, if necessary:
    SoTextureCoordinateElement::CoordType tcType = SoTextureCoordinateElement::getType(state);
    if (tcType == SoTextureCoordinateElement::EXPLICIT) {
        genTexCoord = TRUE;
        tce = NULL;
    } else {
        genTexCoord = FALSE;
        tce = SoTextureCoordinateElement::getInstance(state);
    }

    // Set up 3 vertices we can use
    SoPrimitiveVertex	v1, v2, v3;
    SoTextDetail detail;
    v1.setDetail(&detail);
    v2.setDetail(&detail);
    v3.setDetail(&detail);

    genPrimVerts[0] = &v1;
    genPrimVerts[1] = &v2;
    genPrimVerts[2] = &v3;

    genAction = action;
    genBack = FALSE;

    SoMaterialBindingElement::Binding mbe = SoMaterialBindingElement::get(state);
    SbBool materialPerPart = (mbe == SoMaterialBindingElement::PER_PART_INDEXED ||
                              mbe == SoMaterialBindingElement::PER_PART);
    if (!materialPerPart) {
        v1.setMaterialIndex(0);
        v2.setMaterialIndex(0);
        v3.setMaterialIndex(0);
    }

    float firstZ, lastZ;
    myFont->getProfileBounds(firstZ, lastZ);

    uint32_t prts = parts.getValue();
    if ((prts & SIDES) && myFont->hasProfile()) {
        if (materialPerPart) {
            v1.setMaterialIndex(1);
            v2.setMaterialIndex(1);
            v3.setMaterialIndex(1);
        }
        detail.setPart(SIDES);

        for (int line = 0; line < string.getNum(); line++) {
            detail.setStringIndex(line);

            SbVec2f p = getStringOffset(line);
            genTranslate.setValue(p[0], p[1], lastZ);
            generateSide(line);
        }
    }
    if (prts & BACK) {
        genBack = TRUE;
        if (materialPerPart) {
            v1.setMaterialIndex(2);
            v2.setMaterialIndex(2);
            v3.setMaterialIndex(2);
        }
        detail.setPart(BACK);

        v1.setNormal(SbVec3f(0, 0, -1));
        v2.setNormal(SbVec3f(0, 0, -1));
        v3.setNormal(SbVec3f(0, 0, -1));

        for (int line = 0; line < string.getNum(); line++) {
            detail.setStringIndex(line);

            SbVec2f p = getStringOffset(line);
            genTranslate.setValue(p[0], p[1], lastZ);
            generateFront(line);
        }
        genBack = FALSE;
    }
    if (prts & FRONT) {
        if (materialPerPart) {
            v1.setMaterialIndex(0);
            v2.setMaterialIndex(0);
            v3.setMaterialIndex(0);
        }
        detail.setPart(FRONT);

        v1.setNormal(SbVec3f(0, 0, 1));
        v2.setNormal(SbVec3f(0, 0, 1));
        v3.setNormal(SbVec3f(0, 0, 1));

        for (int line = 0; line < string.getNum(); line++) {
            detail.setStringIndex(line);

            SbVec2f p = getStringOffset(line);
            genTranslate.setValue(p[0], p[1], firstZ);
            generateFront(line);
        }
    }
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Setup internal font cache.  Called by all action methods before
//    doing their thing.  GLRender passes TRUE to do special rendering
//    setup.  Returns FALSE if there are problems and the action
//    should bail.
//
// Use: private

SbBool
SoText3::setupFontCache(SoState *state, SbBool forRender)
//
////////////////////////////////////////////////////////////////////////
{
    // The state must be pushed here because myFont->isRenderValid
    // depends on the state being the correct depth (it must be the
    // same depth as when the font cache was built).
    state->push();

    if (myFont != NULL) {
        SbBool isValid;
        if (forRender)
            isValid = myFont->isRenderValid(state);
        else
            isValid = myFont->isValid(state);

        if (!isValid) {
            myFont->unref(state);
            myFont = NULL;
        }
    }
    if (myFont == NULL) {
        myFont = SoOutlineFontCache::getFont(state, forRender);
    }
    state->pop();
    return  myFont != NULL;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Gets the 2D bounding box of the outlines of the letters in our
//    string.  How?  Get all the outlines and extend the bbox by every
//    point in every outline... (assuming the lines or characters
//    don't overlap, this could be optimized by just considering the
//    characters on the borders of the string-- although there are
//    some gotchas, like spaces at the beginning of a line...).
//
// Use: private

void
SoText3::getFrontBBox(SbBox2f &result)
//
////////////////////////////////////////////////////////////////////////
{
    SbBox2f charBBox;

    for (int line = 0; line < string.getNum(); line++) {
        // Starting position of string, based on justification:
        SbVec2f charPosition = getStringOffset(line);

        const SbString &str = string[line];
        const char *chars = str.getString();

        for (size_t character = 0; character < str.getLength(); character++) {
            myFont->getCharBBox(chars[character], charBBox);
            if (!charBBox.isEmpty()) {
                SbVec2f min = charBBox.getMin() + charPosition;
                SbVec2f max = charBBox.getMax() + charPosition;
                result.extendBy(min);
                result.extendBy(max);
            }

            // And advance...
            charPosition += myFont->getCharOffset(chars[character]);
        }
    }
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Figure out how much to offset the given line of text.  The X
//    offset depends on the justification and the width of the string
//    in the current font.  The Y offset depends on the line spacing
//    and font height.
//
// Use: private

SbVec2f
SoText3::getStringOffset(int line)
//
////////////////////////////////////////////////////////////////////////
{
    SbVec2f result(0,0);
    
    if (justification.getValue() == RIGHT) {
        float width = myFont->getWidth(string[line]);
        result[0] = -width;
    }
    if (justification.getValue() == CENTER) {
        float width = myFont->getWidth(string[line]);
        result[0] = -width/2.0;
    }
    result[1] = -line*myFont->getHeight()*spacing.getValue();

    return result;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Figure out where a given character starts.
//
// Use: private

SbVec2f
SoText3::getCharacterOffset(int line, int whichChar)
//
////////////////////////////////////////////////////////////////////////
{
    SbVec2f result = getStringOffset(line);
    
    const char *chars = string[line].getString();

    // Now add on all of the character advances up to char:
    for (int i = 0; i < whichChar; i++) {
        result += myFont->getCharOffset(chars[i]);
    }
    return result;
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
SoText3::renderFront(SoGLRenderAction *, int line,
                     GLUtesselator *tobj)
//
////////////////////////////////////////////////////////////////////////
{
    const char *chars = string[line].getString();

    // First, try to figure out if we can use glCallLists:
    SbBool useCallLists = TRUE;

    for (size_t i = 0; i < string[line].getLength(); i++) {
        // See if the font cache already has (or can build) a display
        // list for this character:
        if (!myFont->hasFrontDisplayList(chars[i], tobj)) {
            useCallLists = FALSE;
            break;
        }
    }
    // if we have display lists for all of the characters, use
    // glCallLists:
    if (useCallLists) {
        myFont->callFrontLists(string[line]);
    }
    // if we don't, draw the string character-by-character, using the
    // display lists we do have:
    else {
        myFont->renderFront(string[line], tobj);
    }
}    

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Render the sides of the given string.  The GL transformation
//    matrix is munged by this routine-- surround it by
//    PushMatrix/PopMatrix.
//
// Use: private

void
SoText3::renderSide(SoGLRenderAction *, int line)
//
////////////////////////////////////////////////////////////////////////
{
    const char *chars = string[line].getString();

    // First, try to figure out if we can use glCallLists:
    SbBool useCallLists = TRUE;

    for (size_t i = 0; i < string[line].getLength(); i++) {
        // See if the font cache already has (or can build) a display
        // list for this character:
        if (!myFont->hasSideDisplayList(chars[i], renderSideTris)) {
            useCallLists = FALSE;
            break;
        }
    }
    // if we have display lists for all of the characters, use
    // glCallLists:
    if (useCallLists) {
        myFont->callSideLists(string[line]);
    }
    // if we don't, draw the string character-by-character, using the
    // display lists we do have:
    else {
        myFont->renderSide(string[line], renderSideTris);
    }
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Create text detail and fill in extra info.
//
// Use: protected, virtual

SoDetail *
SoText3::createTriangleDetail(SoRayPickAction *,
                              const SoPrimitiveVertex *v1,
                              const SoPrimitiveVertex *,
                              const SoPrimitiveVertex *,
                              SoPickedPoint *)
//
////////////////////////////////////////////////////////////////////////
{
    SoTextDetail *result = new SoTextDetail;
    const SoTextDetail *old = (const SoTextDetail *)v1->getDetail();
    
    result->setPart(old->getPart());
    result->setStringIndex(old->getStringIndex());
    result->setCharacterIndex(old->getCharacterIndex());

    return result;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Generates primitives for the fronts of characters.  Assumes that
//    genTranslate has been set to the starting position of the
//    string, and that the genPrimVerts array points to three
//    appropriate vertices.
//
// Use: internal

void
SoText3::generateFront(int line)
//
////////////////////////////////////////////////////////////////////////
{
    static GLUtesselator *tobj = NULL;

    const char *chars = string[line].getString();

    if (tobj == NULL) {
        tobj = gluNewTess();
        gluTessCallback(tobj, (GLenum)GLU_BEGIN,  (void (CALLBACK*)())SoText3::beginCB);
        gluTessCallback(tobj, (GLenum)GLU_END,    (void (CALLBACK*)())SoText3::endCB);
        gluTessCallback(tobj, (GLenum)GLU_VERTEX, (void (CALLBACK*)())SoText3::vtxCB);
        gluTessCallback(tobj, (GLenum)GLU_ERROR,  (void (CALLBACK*)())SoOutlineFontCache::errorCB);
    }

    genWhichVertex = 0;

    SoTextDetail *d = (SoTextDetail *)genPrimVerts[0]->getDetail();

    for (size_t i = 0; i < string[line].getLength(); i++) {
        d->setCharacterIndex(i);

        myFont->generateFrontChar(chars[i], tobj);

        SbVec2f p = myFont->getCharOffset(chars[i]);
        genTranslate[0] += p[0];
        genTranslate[1] += p[1];
    }
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Generates sides of the characters.
//
// Use: internal

void
SoText3::generateSide(int line)
//
////////////////////////////////////////////////////////////////////////
{
    const char *chars = string[line].getString();

    SoTextDetail *d = (SoTextDetail *)genPrimVerts[0]->getDetail();

    for (size_t i = 0; i < string[line].getLength(); i++) {
        d->setCharacterIndex(i);

        myFont->generateSideChar(chars[i], generateSideTris);

        SbVec2f p = myFont->getCharOffset(chars[i]);
        genTranslate[0] += p[0];
        genTranslate[1] += p[1];
    }
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Given two correctly rotated and positioned bevels, this routine
//    fills in the triangles in between them.
//
// Use: private, static

void
SoText3::generateSideTris(int nPoints, const SbVec3f *p1, const SbVec3f *n1,
                          const SbVec3f *p2, const SbVec3f *n2,
                          const float *sTexCoords, const float *tTexCoords)
//
////////////////////////////////////////////////////////////////////////
{
    float vertex[3];
    SbVec4f texCoord(0,0,0,1);
    
    const SbVec3f *p[2]; p[0] = p1; p[1] = p2;
    const SbVec3f *n[2]; n[0] = n1; n[1] = n2;

    // Handy little macro to set a primitiveVertice's point, normal, and
    // texture coordinate:
#define SET(pv,i,row,col) \
    {vertex[0] = p[col][i+row][0] + genTranslate[0]; \
    vertex[1] = p[col][i+row][1] + genTranslate[1]; \
    vertex[2] = p[col][i+row][2]; \
    genPrimVerts[pv]->setPoint(vertex);\
    genPrimVerts[pv]->setNormal(n[col][i*2+row]); \
    texCoord[0] = sTexCoords[i+row]; \
    texCoord[1] = tTexCoords[col]; \
    genPrimVerts[pv]->setTextureCoords(texCoord); \
}

    SoText3 *t3 = currentGeneratingNode;

    for (int i = 0; i < nPoints-1; i++) {
        // First triangle:
        SET(0, i, 0, 0)
                SET(1, i, 1, 0)
                SET(2, i, 0, 1)
                t3->invokeTriangleCallbacks(genAction, genPrimVerts[0],
                                            genPrimVerts[1], genPrimVerts[2]);

        // Second triangle:
        SET(0, i, 1, 1)
                SET(1, i, 0, 1)
                SET(2, i, 1, 0)
                t3->invokeTriangleCallbacks(genAction, genPrimVerts[0],
                                            genPrimVerts[1], genPrimVerts[2]);
#undef SET
    }
}
////////////////////////////////////////////////////////////////////////
//
// Description:
//    Given two correctly rotated and positioned bevels, this routine
//    renders triangles in between them.
//
// Use: private, static

void
SoText3::renderSideTris(int nPoints, const SbVec3f *p1, const SbVec3f *n1,
                        const SbVec3f *p2, const SbVec3f *n2,
                        const float *sTex, const float *tTex)
//
////////////////////////////////////////////////////////////////////////
{
    // Note:  the glBegin(GL_QUADS) is optimized up into the
    // routine that calls generateSideChar, so there is one glBegin
    // per character.
    for (int i = 0; i < nPoints-1; i++) {
        if (genTexCoord) glTexCoord2f(sTex[i+1], tTex[0]);
        glNormal3fv(n1[i*2+1].getValue());
        glVertex3fv(p1[i+1].getValue());

        if (genTexCoord) glTexCoord2f(sTex[i+1], tTex[1]);
        glNormal3fv(n2[i*2+1].getValue());
        glVertex3fv(p2[i+1].getValue());

        if (genTexCoord) glTexCoord2f(sTex[i], tTex[1]);
        glNormal3fv(n2[i*2].getValue());
        glVertex3fv(p2[i].getValue());

        if (genTexCoord) glTexCoord2f(sTex[i], tTex[0]);
        glNormal3fv(n1[i*2].getValue());
        glVertex3fv(p1[i].getValue());
    }
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Called by the GLU tesselator when we are beginning a triangle
//    strip, fan, or set of independent triangles.
//
// Use: static, private

void
SoText3::beginCB(GLenum primType)
//
////////////////////////////////////////////////////////////////////////
{
    genPrimType = primType;
    genWhichVertex = 0;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Called by the GLU tesselator when we are done with the
//    strip/fan/etc.
//
// Use: static, private

void
SoText3::endCB()
//
////////////////////////////////////////////////////////////////////////
{
    genWhichVertex = 0;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Called by the GLU tesselator when we are generating primitives.
//
// Use: static, private

void
SoText3::vtxCB(void *v)
//
////////////////////////////////////////////////////////////////////////
{
    SbVec2f &vv = *((SbVec2f *)v);
    float vertex[3];
    vertex[0] = vv[0] + genTranslate[0];
    vertex[1] = vv[1] + genTranslate[1];
    vertex[2] = genTranslate[2];

    SoText3 *t3 = currentGeneratingNode;
    
    // Fill in one of the primitive vertices:
    genPrimVerts[genWhichVertex]->setPoint(vertex);

    SbVec4f texCoord;
    
    // And texture coordinates:
    if (genTexCoord) {
        float textHeight = t3->myFont->getHeight();
        texCoord.setValue(vertex[0]/textHeight, vertex[1]/textHeight,
                          0.0, 1.0);
        // S coordinates go other way on back...
        if (genBack) texCoord[0] = -texCoord[0];
    } else {
        texCoord = tce->get(vertex, genPrimVerts[0]->getNormal());
    }
    genPrimVerts[genWhichVertex]->setTextureCoords(texCoord);

    genWhichVertex = (genWhichVertex+1)%3;

    // If we just filled in the third vertex, we can spit out a
    // triangle:
    if (genWhichVertex == 0) {
        // If we are doing the BACK part, reverse the triangle:
        if (genBack) {
            t3->invokeTriangleCallbacks(genAction,
                                        genPrimVerts[2],
                                        genPrimVerts[1],
                                        genPrimVerts[0]);
        } else {
            t3->invokeTriangleCallbacks(genAction,
                                        genPrimVerts[0],
                                        genPrimVerts[1],
                                        genPrimVerts[2]);
        }
        // Now, need to set-up for the next vertex.
        // Three cases to deal with-- independent triangles, triangle
        // strips, and triangle fans.
        switch (genPrimType) {
        case GL_TRIANGLES:
            // Don't need to do anything-- every three vertices
            // defines a triangle.
            break;

            // Useful macro:
#define SWAP(a, b) { SoPrimitiveVertex *t = a; a = b; b = t; }

        case GL_TRIANGLE_FAN:
            // For triangle fans, vertex zero stays the same, but
            // vertex 2 becomes vertex 1, and the next vertex to come
            // in will replace vertex 2 (the old vertex 1).
            SWAP(genPrimVerts[1], genPrimVerts[2]);
            genWhichVertex = 2;
            break;

        case GL_TRIANGLE_STRIP:
            // For triangle strips, vertex 1 becomes vertex 0, vertex
            // 2 becomes vertex 1, and the new triangle will replace
            // vertex 2 (the old vertex 0).
            SWAP(genPrimVerts[1], genPrimVerts[0]);
            SWAP(genPrimVerts[2], genPrimVerts[1]);
            genWhichVertex = 2;
            break;
#undef SWAP
        }
    }
}


