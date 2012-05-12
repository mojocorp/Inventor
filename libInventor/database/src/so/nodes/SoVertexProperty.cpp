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
 * Copyright (C) 1995-96   Silicon Graphics, Inc.
 *
 _______________________________________________________________________
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 |
 |   $Revision: 1.2 $
 |
 |   Classes:
 |      SoVertexProperty,  SoVertexCache
 |
 |   Author(s)          : Alan Norton,  Gavin Bell
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

#include <Inventor/nodes/SoVertexProperty.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/actions/SoPickAction.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/elements/SoGLCoordinateElement.h>
#include <Inventor/elements/SoGLNormalElement.h>
#include <Inventor/elements/SoOverrideElement.h>
#include <Inventor/elements/SoGLTextureCoordinateElement.h>
#include <Inventor/elements/SoTextureCoordinateBindingElement.h>
#include <Inventor/elements/SoMaterialBindingElement.h>
#include <Inventor/elements/SoNormalBindingElement.h>
#include <Inventor/nodes/SoTextureCoordinate2.h>
#include <Inventor/elements/SoGLLazyElement.h>
#include <machine.h>

SO_NODE_SOURCE(SoVertexProperty);


////////////////////////////////////////////////////////////////////////
//
// Description:
//    Constructor
//
// Use: public

SoVertexProperty::SoVertexProperty()
//
////////////////////////////////////////////////////////////////////////
{

    SO_NODE_CONSTRUCTOR(SoVertexProperty);

    // Initialize with dummy values using standard macro, then delete
    // all values:
    SO_NODE_ADD_FIELD(vertex, (SbVec3f(0,0,0)));
    vertex.deleteValues(0);
    vertex.setDefault(TRUE);
    SO_NODE_ADD_FIELD(normal, (SbVec3f(0,0,0)));
    normal.deleteValues(0);
    normal.setDefault(TRUE);
    SO_NODE_ADD_FIELD(texCoord, (SbVec2f(0,0)));
    texCoord.deleteValues(0);
    texCoord.setDefault(TRUE);
    SO_NODE_ADD_FIELD(orderedRGBA, (0));    
    orderedRGBA.deleteValues(0);
    orderedRGBA.setDefault(TRUE);

    // Initialize these with default values.  They'll be ignored if
    // the corresponding fields have no values:
    SO_NODE_ADD_FIELD(materialBinding, (SoVertexProperty::OVERALL));
    SO_NODE_ADD_FIELD(normalBinding, (SoVertexProperty::PER_VERTEX_INDEXED));

    // Set up static info for enum type fields:

    SO_NODE_DEFINE_ENUM_VALUE(Binding, OVERALL);
    SO_NODE_DEFINE_ENUM_VALUE(Binding, PER_VERTEX);
    SO_NODE_DEFINE_ENUM_VALUE(Binding, PER_VERTEX_INDEXED);
    SO_NODE_DEFINE_ENUM_VALUE(Binding, PER_FACE);
    SO_NODE_DEFINE_ENUM_VALUE(Binding, PER_FACE_INDEXED);
    SO_NODE_DEFINE_ENUM_VALUE(Binding, PER_PART);
    SO_NODE_DEFINE_ENUM_VALUE(Binding, PER_PART_INDEXED);

    SO_NODE_SET_SF_ENUM_TYPE(materialBinding, Binding);
    SO_NODE_SET_SF_ENUM_TYPE(normalBinding, Binding);

    transparent = false;
    isBuiltIn = TRUE;
}
////////////////////////////////////////////////////////////////////////
//
// Description:
//    initialize class  
//
void
SoVertexProperty::initClass()
{
    SO__NODE_INIT_CLASS(SoVertexProperty, "VertexProperty", SoNode);

    // enable elements for vertex:
    SO_ENABLE(SoCallbackAction,		SoCoordinateElement);
    SO_ENABLE(SoGLRenderAction, 	SoGLCoordinateElement);
    SO_ENABLE(SoGetBoundingBoxAction,	SoCoordinateElement);
    SO_ENABLE(SoPickAction,		SoCoordinateElement);

    // Enable elements for normal:
    SO_ENABLE(SoGLRenderAction, SoGLNormalElement);
    SO_ENABLE(SoCallbackAction, SoNormalElement);
    SO_ENABLE(SoPickAction,	SoNormalElement);

    // Enable elements for normal binding:
    SO_ENABLE(SoGLRenderAction, SoNormalBindingElement);
    SO_ENABLE(SoCallbackAction, SoNormalBindingElement);
    SO_ENABLE(SoPickAction, SoNormalBindingElement);

    // Enable elements for material:
    SO_ENABLE(SoCallbackAction, SoLazyElement);
    SO_ENABLE(SoPickAction, SoLazyElement);
    SO_ENABLE(SoGLRenderAction, SoGLLazyElement);

    // Enable elements for MaterialBinding:    
    SO_ENABLE(SoGLRenderAction, SoMaterialBindingElement);
    SO_ENABLE(SoCallbackAction, SoMaterialBindingElement);
    SO_ENABLE(SoPickAction,     SoMaterialBindingElement);

    // Enable elements for textureCoordinates:
    SO_ENABLE(SoCallbackAction, SoTextureCoordinateElement);
    SO_ENABLE(SoGLRenderAction, SoGLTextureCoordinateElement);
    SO_ENABLE(SoPickAction,     SoTextureCoordinateElement);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Destructor
//
// Use: private

SoVertexProperty::~SoVertexProperty()
//
////////////////////////////////////////////////////////////////////////
{
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Handles any action derived from SoAction.
//
// Use: extender

void
SoVertexProperty::doAction(SoAction *action)
//
////////////////////////////////////////////////////////////////////////
{
    SoState		*state = action->getState();
    //  Note, we are not supporting isIgnored flags:
    if (vertex.getNum() > 0)
	SoCoordinateElement::set3(state, this,
				  vertex.getNum(), vertex.getValues(0));

    if (normal.getNum() > 0){
	SoNormalElement::set(state, this,
			     normal.getNum(), normal.getValues(0));
    }
    SoNormalBindingElement::set(state, 
	(SoNormalBindingElement::Binding)normalBinding.getValue());

    // set diffuse color
    if (orderedRGBA.getNum() > 0
	&& ! SoOverrideElement::getDiffuseColorOverride(state)) {
	if (isOverride()) {
	    SoOverrideElement::setDiffuseColorOverride(state, this, TRUE);
	}			
	SoLazyElement::setPacked(state, this,
        orderedRGBA.getNum(), orderedRGBA.getValues(0), isTransparent());
    }
    // set material binding 
    SoMaterialBindingElement::set(state, 
	(SoMaterialBindingElement::Binding)materialBinding.getValue());

    if ( texCoord.getNum() > 0) {
	SoTextureCoordinateElement::set2(state, this,
			 texCoord.getNum(), texCoord.getValues(0));
    }
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Does GL render action.
//
// Use: extender

void
SoVertexProperty::GLRender(SoGLRenderAction *action)
//
////////////////////////////////////////////////////////////////////////
{
    if ( texCoord.getNum() > 0) {
    	SoState *state = action->getState();
	SoGLTextureCoordinateElement::setTexGen(state, this, NULL);
    }

    SoVertexProperty::doAction(action);

}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Handles get bounding box action.
//
// Use: extender

void
SoVertexProperty::getBoundingBox(SoGetBoundingBoxAction *action)
//
////////////////////////////////////////////////////////////////////////
{
    // Note that Bounding box only needs to look at coordinates:
    if (vertex.getNum() > 0)
        SoCoordinateElement::set3(action->getState(), this,
                                  vertex.getNum(), vertex.getValues(0));
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Does callback action thing.
//
// Use: extender

void
SoVertexProperty::callback(SoCallbackAction *action)
//
////////////////////////////////////////////////////////////////////////
{
    SoVertexProperty::doAction(action);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Does pick action...
//
// Use: extender

void
SoVertexProperty::pick(SoPickAction *action)
//
////////////////////////////////////////////////////////////////////////
{
    SoVertexProperty::doAction(action);
}

///////////////////////////////////////////////////////////////////////////
//
// Description:
// When notified of change in orderedRGBA field, reevaluate transparent flag
//
// use: SoExtender public
//
void
SoVertexProperty::notify(SoNotList *list)
//
////////////////////////////////////////////////////////////////////////
{
    if ((list->getLastRec()->getType() == SoNotRec::CONTAINER) &&
            (list->getLastField() == &orderedRGBA) ) {
        transparent = false;
        for(int i = 0; i < orderedRGBA.getNum(); i++){
            if((orderedRGBA[i] & 0xff) != 0xff){
                transparent = true;
                break;
            }
        }
    }

    SoNode::notify(list);
}




