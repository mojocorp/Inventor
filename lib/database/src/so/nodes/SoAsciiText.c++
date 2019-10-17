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
 * Copyright (C) 1990-95   Silicon Graphics, Inc.
 *
 _______________________________________________________________________
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 |
 |   $Revision: 1.4 $
 |
 |   Classes:
 |      SoAsciiText
 |
 |
 |   Author(s)          : Gavin Bell,  Chris Marrin
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

#include <Inventor/misc/SoGL.h>
#include <GL/glu.h>
#include <Inventor/SbBox.h>
#include <Inventor/SoPrimitiveVertex.h>
#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoRayPickAction.h>
#include <Inventor/bundles/SoMaterialBundle.h>
#include <Inventor/caches/SoOutlineFontCache.h>
#include <Inventor/details/SoTextDetail.h>
#include <Inventor/elements/SoGLTextureEnabledElement.h>
#include <Inventor/elements/SoGLTextureCoordinateElement.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/misc/SoState.h>
#include <Inventor/nodes/SoAsciiText.h>

#include <algorithm>

#define VALIDATE_CHAR(c)    (((c) >= 0x20 && (c) <= 0x7F) ? (c) : 0x3f)

// Static stuff is used while generating primitives:
SoAsciiText *SoAsciiText::currentGeneratingNode = NULL;
SoPrimitiveVertex *SoAsciiText::genPrimVerts[3];
SbVec3f SoAsciiText::genTranslate;
int SoAsciiText::genWhichVertex = -1;
uint32_t SoAsciiText::genPrimType;
SoAction *SoAsciiText::genAction = NULL;
SbBool SoAsciiText::genBack = FALSE;
SbBool SoAsciiText::genTexCoord = TRUE;
const SoTextureCoordinateElement *SoAsciiText::tce = NULL;

SO_NODE_SOURCE(SoAsciiText);

////////////////////////////////////////////////////////////////////////
//
//  This initializes the SoAsciiText class.
//
void
SoAsciiText::initClass()
//
////////////////////////////////////////////////////////////////////////
{
    SO__NODE_INIT_CLASS(SoAsciiText, "AsciiText", SoShape);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Constructor
//
// Use: public

SoAsciiText::SoAsciiText()
//
////////////////////////////////////////////////////////////////////////
{
    SO_NODE_CONSTRUCTOR(SoAsciiText);

    SO_NODE_ADD_FIELD(string,	(""));
    SO_NODE_ADD_FIELD(spacing,	(1.0));
    SO_NODE_ADD_FIELD(justification,	(LEFT));
    SO_NODE_ADD_FIELD(width,	(0));

    // Set up static info for enumerated type field
    SO_NODE_DEFINE_ENUM_VALUE(Justification,	LEFT);
    SO_NODE_DEFINE_ENUM_VALUE(Justification,	RIGHT);
    SO_NODE_DEFINE_ENUM_VALUE(Justification,	CENTER);

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

SoAsciiText::~SoAsciiText()
//
////////////////////////////////////////////////////////////////////////
{
    if (fontCache != NULL)
        fontCache->unref();
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Performs GL rendering of a Text3.
//
// Use: extender

void
SoAsciiText::GLRender(SoGLRenderAction *action)
//
////////////////////////////////////////////////////////////////////////
{
    // First see if the object is visible and should be rendered now
    if (! shouldGLRender(action))
        return;

    SoState *state = action->getState();

    if (!setupFontCache(state, TRUE))
        return;

    SoMaterialBindingElement::Binding mbe =	SoMaterialBindingElement::get(state);
    SbBool materialPerPart = (mbe == SoMaterialBindingElement::PER_PART_INDEXED ||
                              mbe == SoMaterialBindingElement::PER_PART);

    SoMaterialBundle mb(action);
    if (!materialPerPart) {
        // Make sure the fist current material is sent to GL
        mb.sendFirst();
    }

    // See if texturing is enabled
    genTexCoord = SoGLTextureEnabledElement::get(action->getState());

    if (materialPerPart)
        mb.sendFirst();

    glNormal3f(0, 0, 1);

    if (genTexCoord) {
        glPushAttrib(GL_TEXTURE_BIT);
        glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
        glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
        GLfloat params[4];
        params[0] = 1.0f/fontCache->getHeight();
        params[1] = params[2] = params[3] = 0.0f;
        glTexGenfv(GL_S, GL_OBJECT_PLANE, params);
        params[1] = params[0];
        params[0] = 0.0f;
        glTexGenfv(GL_T, GL_OBJECT_PLANE, params);

        glEnable(GL_TEXTURE_GEN_S);
        glEnable(GL_TEXTURE_GEN_T);
    }

    for (int line = 0; line < string.getNum(); line++) {
        glPushMatrix();
        float w = (line < width.getNum()) ? width[line] : 0;
        SbVec2f p = getStringOffset(line, w);
        if (p[0] != 0.0 || p[1] != 0.0)
            glTranslatef(p[0], p[1], 0.0);
        fontCache->renderFront(state, string[line]);
        glPopMatrix();
    }
    
    if (genTexCoord) {
        glPopAttrib();
    }
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Implements ray picking
//
// Use: extender

void
SoAsciiText::rayPick(SoRayPickAction *action)
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
SoAsciiText::computeBBox(SoAction *action, SbBox3f &box, SbVec3f &center)
//
////////////////////////////////////////////////////////////////////////
{
    // Set the center to be the origin, which is the natural "center"
    // of the text, regardless of justification
    center.setValue(0.0, 0.0, 0.0);

    SoState *state = action->getState();

    if (!setupFontCache(state))
        return;

    // Get the bounding box of all the characters:
    SbBox2f outlineBox;
    getFrontBBox(outlineBox);

    // If no lines and no characters, return empty bbox:
    if (outlineBox.isEmpty())
        return;
    
    const SbVec2f &boxMin = outlineBox.getMin();
    const SbVec2f &boxMax = outlineBox.getMax();
		     
    SbVec3f min(boxMin[0], boxMin[1], 0);
    SbVec3f max(boxMax[0], boxMax[1], 0);
    box.extendBy(min);
    box.extendBy(max);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Generates triangles representing a Text3.
//
// Use: protected

void
SoAsciiText::generatePrimitives(SoAction *action)
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

    SoMaterialBindingElement::Binding mbe =	SoMaterialBindingElement::get(state);
    SbBool materialPerPart = (mbe == SoMaterialBindingElement::PER_PART_INDEXED ||
                              mbe == SoMaterialBindingElement::PER_PART);
    if (!materialPerPart) {
        v1.setMaterialIndex(0);
        v2.setMaterialIndex(0);
        v3.setMaterialIndex(0);
    }

    if (materialPerPart) {
        v1.setMaterialIndex(0);
        v2.setMaterialIndex(0);
        v3.setMaterialIndex(0);
    }

    v1.setNormal(SbVec3f(0, 0, 1));
    v2.setNormal(SbVec3f(0, 0, 1));
    v3.setNormal(SbVec3f(0, 0, 1));
    
    for (int line = 0; line < string.getNum(); line++) {
        detail.setStringIndex(line);

        float w = (line < width.getNum()) ? width[line] : 0;
        SbVec2f p = getStringOffset(line, w);
        genTranslate.setValue(p[0], p[1], 0);
        generateFront(string[line], w);
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
SoAsciiText::setupFontCache(SoState *state, SbBool forRender)
//
////////////////////////////////////////////////////////////////////////
{
    // The state must be pushed here because myFont->isRenderValid
    // depends on the state being the correct depth (it must be the
    // same depth as when the font cache was built).
    state->push();

    if (fontCache != NULL) {
        SbBool isValid;
        if (forRender)
            isValid = fontCache->isRenderValid(state);
        else
            isValid = fontCache->isValid(state);

        if (!isValid) {
            fontCache->unref(state);
            fontCache = NULL;
        }
    }
    if (fontCache == NULL) {
        fontCache = SoOutlineFontCache::getFont(state, forRender);
        fontCache->ref();
    }
    state->pop();
    return  fontCache != NULL;
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
SoAsciiText::getFrontBBox(SbBox2f &result)
//
////////////////////////////////////////////////////////////////////////
{
    for (int line = 0; line < string.getNum(); line++) {
        // Starting position of string, based on justification:
        float w = (line < width.getNum()) ? width[line] : 0;
        SbVec2f charPosition = getStringOffset(line, w);
        SbVec2f curCharPos = charPosition;

        const SbString &str = string[line];
        const char *chars = str.getString();
	
        for (size_t character = 0; character < str.getLength(); character++) {
            SbBox2f charBBox = fontCache->getCharBBox(chars[character]);
            if (!charBBox.isEmpty()) {
                SbVec2f min = charBBox.getMin() + curCharPos;
                SbVec2f max = charBBox.getMax() + curCharPos;
                result.extendBy(min);
                result.extendBy(max);
	    }

	    // And advance...
        curCharPos += fontCache->getCharOffset(chars[character]);
	}
	if (w > 0) { 
	    // force the bbox width
	    SbVec2f min = result.getMin();
	    SbVec2f max = result.getMax();
	    result.setBounds(charPosition[0], min[1], 
			     charPosition[0] + w, max[1]);
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
SoAsciiText::getStringOffset(int line, float width)
//
////////////////////////////////////////////////////////////////////////
{
    SbVec2f result(0,0);
    
    if (justification.getValue() == RIGHT) {
        if (width <= 0)
            width = fontCache->getWidth(string[line].toStdWString());
        result[0] = -width;
    }
    if (justification.getValue() == CENTER) {
        if (width <= 0)
            width = fontCache->getWidth(string[line].toStdWString());
        result[0] = -width/2.0f;
    }
    result[1] = -line*fontCache->getHeight()*spacing.getValue();

    return result;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Create text detail and fill in extra info.
//
// Use: protected, virtual

SoDetail *
SoAsciiText::createTriangleDetail(SoRayPickAction *,
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
SoAsciiText::generateFront(const SbString &string, float width)
//
////////////////////////////////////////////////////////////////////////
{
    static GLUtesselator *tobj = NULL;   

    if (tobj == NULL) {
        tobj = (GLUtesselator *)gluNewTess();
        gluTessCallback(tobj, (GLenum)GLU_TESS_BEGIN, (void (*)())SoAsciiText::beginCB);
        gluTessCallback(tobj, (GLenum)GLU_TESS_END, (void (*)())SoAsciiText::endCB);
        gluTessCallback(tobj, (GLenum)GLU_TESS_VERTEX, (void (*)())SoAsciiText::vtxCB);
        gluTessCallback(tobj, (GLenum)GLU_TESS_ERROR, (void (*)())SoOutlineFontCache::errorCB);
    }

    genWhichVertex = 0;

    SoTextDetail *d = (SoTextDetail *)genPrimVerts[0]->getDetail();
    
    const std::wstring chars = string.toStdWString();

    // if we have a fixed width, use it.
    float off = 0;
    if (width > 0) {
        const float naturalWidth = fontCache->getWidth(chars);
        off = (width - naturalWidth) / (string.getLength() - 1);
    }
    
    for (size_t i = 0; i < string.getLength(); i++) {
        d->setCharacterIndex(i);

        fontCache->generateFrontChar(chars[i], tobj);

        SbVec2f p = fontCache->getCharOffset(chars[i]);
        genTranslate[0] += p[0] + off;
        genTranslate[1] += p[1];
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
SoAsciiText::beginCB(unsigned int primType)
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
SoAsciiText::endCB()
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
SoAsciiText::vtxCB(void *v)
//
////////////////////////////////////////////////////////////////////////
{
    SbVec2f &vv = *((SbVec2f *)v);
    float vertex[3];
    vertex[0] = vv[0] + genTranslate[0];
    vertex[1] = vv[1] + genTranslate[1];
    vertex[2] = genTranslate[2];

    SoAsciiText *t3 = currentGeneratingNode;
    
    // Fill in one of the primitive vertices:
    genPrimVerts[genWhichVertex]->setPoint(vertex);

    SbVec4f texCoord;
    
    // And texture coordinates:
    if (genTexCoord) {
        float textHeight = t3->fontCache->getHeight();
        texCoord.setValue(vertex[0]/textHeight, vertex[1]/textHeight, 0.0, 1.0);

        // S coordinates go other way on back...
        if (genBack)
            texCoord[0] = -texCoord[0];
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

          case GL_TRIANGLE_FAN:
            // For triangle fans, vertex zero stays the same, but
            // vertex 2 becomes vertex 1, and the next vertex to come
            // in will replace vertex 2 (the old vertex 1).
            std::swap(genPrimVerts[1], genPrimVerts[2]);
            genWhichVertex = 2;
            break;

          case GL_TRIANGLE_STRIP:
            // For triangle strips, vertex 1 becomes vertex 0, vertex
            // 2 becomes vertex 1, and the new triangle will replace
            // vertex 2 (the old vertex 0).
            std::swap(genPrimVerts[1], genPrimVerts[0]);
            std::swap(genPrimVerts[2], genPrimVerts[1]);
            genWhichVertex = 2;
            break;
        }
    }
}
