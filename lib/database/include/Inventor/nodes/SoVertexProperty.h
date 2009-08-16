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


//  -*- C++ -*-

/*
 * Copyright (C) 1995-96   Silicon Graphics, Inc.
 *
 _______________________________________________________________________
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 |
 |   $Revision: 1.1 $
 |
 |   Description:
 |	This file defines the SoVertexProperty node class.
 |
 |   Author(s)		: Alan Norton, Gavin Bell
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

#ifndef  _SO_VERTEX_PROPERTY
#define  _SO_VERTEX_PROPERTY

#include <Inventor/fields/SoMFUInt32.h>
#include <Inventor/fields/SoMFVec3f.h>
#include <Inventor/fields/SoMFVec2f.h>
#include <Inventor/fields/SoSFEnum.h>
#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/nodes/SoMaterialBinding.h>

//////////////////////////////////////////////////////////////////////////////
//
//  Class: SoVertexProperty
//
//  SoNode class that manages arrays of data for GLVertex Array Extension.
//  Data arrays include: vertices, normals, colors, texture coordinates.
//  Also has NormalBinding and MaterialBinding fields
//
//////////////////////////////////////////////////////////////////////////////

class SoVertexPropertyCache;
class INVENTOR_API SoVertexProperty : public SoNode {

    SO_NODE_HEADER(SoVertexProperty);

  public:

    // Fields:
    SoMFVec3f		vertex;		// Coordinate point(s)
    SoMFVec2f    	texCoord;	// Texture coordinate(s)

    SoMFVec3f		normal;		// Normal vector(s)
    SoSFEnum		normalBinding;  // Ignored unless normal field
					// set

    SoMFUInt32		orderedRGBA;	// Diffuse+transparency
    SoSFEnum		materialBinding;// Ignored unless orderedRGBA
					// field set

    enum Binding {
	OVERALL =		SoMaterialBindingElement::OVERALL,
	PER_PART =		SoMaterialBindingElement::PER_PART,
	PER_PART_INDEXED =	SoMaterialBindingElement::PER_PART_INDEXED,
	PER_FACE =		SoMaterialBindingElement::PER_FACE,
	PER_FACE_INDEXED =	SoMaterialBindingElement::PER_FACE_INDEXED,
	PER_VERTEX =		SoMaterialBindingElement::PER_VERTEX,
	PER_VERTEX_INDEXED =	SoMaterialBindingElement::PER_VERTEX_INDEXED
    };

    // Constructor
    SoVertexProperty();

  SoEXTENDER public:
    virtual void	doAction(SoAction *action);
    virtual void	GLRender(SoGLRenderAction *action);
    virtual void	getBoundingBox(SoGetBoundingBoxAction *action);
    virtual void	callback(SoCallbackAction *action);
    virtual void	pick(SoPickAction *action);

  SoINTERNAL public:
    static void		initClass();

  protected:
    virtual ~SoVertexProperty();
    
};

#endif /* _SO_VERTEX_PROPERTY */

