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
 |   $Revision: 1.1 $
 |
 |   Classes:
 |      SoTextureCoordinateObject
 |
 |   Author(s)          : Thad Beier, Gavin Bell
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoPickAction.h>
#include <Inventor/elements/SoGLTextureCoordinateElement.h>
#include <Inventor/elements/SoTextureQualityElement.h>
#include <Inventor/nodes/SoTextureCoordinateObject.h>

#include <Inventor/misc/SoGL.h>

SO_NODE_SOURCE(SoTextureCoordinateObject);

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Constructor
//
// Use: public

SoTextureCoordinateObject::SoTextureCoordinateObject()
//
////////////////////////////////////////////////////////////////////////
{
    SO_NODE_CONSTRUCTOR(SoTextureCoordinateObject);
    SO_NODE_ADD_FIELD(factorS, (1.0f, 0.0f, 0.0f, 0.0f));
    SO_NODE_ADD_FIELD(factorT, (0.0f, 1.0f, 0.0f, 0.0f));
    SO_NODE_ADD_FIELD(factorR, (0.0f, 0.0f, 1.0f, 0.0f));
    SO_NODE_ADD_FIELD(factorQ, (0.0f, 0.0f, 0.0f, 1.0f));
    isBuiltIn = TRUE;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Destructor
//
// Use: public

SoTextureCoordinateObject::~SoTextureCoordinateObject()
//
////////////////////////////////////////////////////////////////////////
{
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    This initializes the SoTextureCoordinateObject class.
//
// Use: internal

void
SoTextureCoordinateObject::initClass()
//
////////////////////////////////////////////////////////////////////////
{
    SO__NODE_INIT_CLASS(SoTextureCoordinateObject,
                      "TextureCoordinateObject", SoTextureCoordinateFunction);

    // Elements are enabled by SoTextureCoordinate nodes.
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Calculates texture coordinate
//
// Use: public

const SbVec4f &
SoTextureCoordinateObject::valueCallback(void *instance,
    const SbVec3f & /* position - not used */,
    const SbVec3f & /* normal - not used */)
//
////////////////////////////////////////////////////////////////////////
{
    return SbVec4f(0.0f, 0.0f, 0.0f, 0.0f);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Does GL render action.
//
// Use: protected

void
SoTextureCoordinateObject::GLRender(SoGLRenderAction *action)
//
////////////////////////////////////////////////////////////////////////
{
    SoState *state = action->getState();

    // Special case to workaround OpenGL on Indigo/IndigoII bug:
    if (SoTextureOverrideElement::getQualityOverride(state) &&
	SoTextureQualityElement::get(state) == 0.0) return;

    SoGLTextureCoordinateElement::setTexGen(state, this, 
					    doTexgen, this,
					    valueCallback, this);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Callback registered with the TextureCoordinateElement that keep
//    the GL up-to-date as the state is pushed/popped.
//
// Use: private, static

void
SoTextureCoordinateObject::doTexgen(void *me)
//
////////////////////////////////////////////////////////////////////////
{
    const SoTextureCoordinateObject *p =
        (const SoTextureCoordinateObject *)me;

    glTexGenf(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
    glTexGenfv(GL_S, GL_OBJECT_PLANE, p->factorS.getValue().getValue());

    glTexGenf(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
    glTexGenfv(GL_T, GL_OBJECT_PLANE, p->factorT.getValue().getValue());

    glTexGenf(GL_R, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
    glTexGenfv(GL_R, GL_OBJECT_PLANE, p->factorR.getValue().getValue());

    glTexGenf(GL_Q, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
    glTexGenfv(GL_Q, GL_OBJECT_PLANE, p->factorQ.getValue().getValue());
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Does callback action thing.
//
// Use: protected

void
SoTextureCoordinateObject::callback(SoCallbackAction *action)
//
////////////////////////////////////////////////////////////////////////
{
    SoTextureCoordinateObject::doAction(action);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Does pick action...
//
// Use: protected

void
SoTextureCoordinateObject::pick(SoPickAction *action)
//
////////////////////////////////////////////////////////////////////////
{
    SoTextureCoordinateObject::doAction(action);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    doAction.  Add this node to the state
//
// Use: Extender public

void
SoTextureCoordinateObject::doAction(SoAction *action)
//
////////////////////////////////////////////////////////////////////////
{
    SoState *state = action->getState();

    SoTextureCoordinateElement::setFunction(state, this,
					    valueCallback, this);
}
