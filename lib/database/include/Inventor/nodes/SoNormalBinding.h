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
 * Copyright (C) 1990,91   Silicon Graphics, Inc.
 *
 _______________________________________________________________________
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 |
 |   $Revision $
 |
 |   Description:
 |	This file defines the SoNormalBinding node class.
 |
 |   Author(s)		: Paul S. Strauss
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

#pragma once

#include <Inventor/elements/SoNormalBindingElement.h>
#include <Inventor/fields/SoSFEnum.h>
#include <Inventor/nodes/SoSubNode.h>

/// Node that specifies how multiple surface normals are bound to shapes.
/// \ingroup Nodes
/// This node specifies how the current normals are bound to shapes that
/// follow in the scene graph. Each shape node may interpret bindings
/// differently.
///
/// The bindings for faces and vertices are meaningful only for shapes
/// that are made from faces and vertices. Similarly, the indexed bindings
/// are only used by the shapes that allow indexing. For bindings that
/// require multiple normals, be sure to have at least as many normals
/// defined as are necessary; otherwise, errors will occur.
///
/// \par Action behavior:
/// <b>SoGLRenderAction, SoCallbackAction, SoRayPickAction</b>
/// Sets the current normal binding type.
///
/// \par File format/defaults:
/// \code
/// SoNormalBinding {
///    value	PER_VERTEX_INDEXED
/// }
/// \endcode
/// \sa SoMaterialBinding,SoNormal,SoTextureCoordinateBinding,SoVertexShape
class INVENTOR_API SoNormalBinding : public SoNode {

    SO_NODE_HEADER(SoNormalBinding);

  public:
    enum Binding {
        OVERALL =
            SoNormalBindingElement::OVERALL, ///< Whole object has same normal
        PER_PART = SoNormalBindingElement::PER_PART, ///< One normal for each
                                                     ///< part of object
        PER_PART_INDEXED =
            SoNormalBindingElement::PER_PART_INDEXED, ///< One normal for each
                                                      ///< part of object,
                                                      ///< indexed
        PER_FACE = SoNormalBindingElement::PER_FACE,  ///< One normal for each
                                                      ///< face of object
        PER_FACE_INDEXED =
            SoNormalBindingElement::PER_FACE_INDEXED, ///< One normal for each
                                                      ///< face, indexed
        PER_VERTEX =
            SoNormalBindingElement::PER_VERTEX, ///< One normal for each
                                                ///< vertex of object
        PER_VERTEX_INDEXED =
            SoNormalBindingElement::PER_VERTEX_INDEXED, ///< One normal for each
                                                        ///< vertex, indexed
    };

    // Fields
    SoSFEnum value; ///< Specifies how to bind normals to shapes.

    /// Creates a normal binding node with default settings.
    SoNormalBinding();

    SoEXTENDER
  public:
    virtual void doAction(SoAction *action);
    virtual void GLRender(SoGLRenderAction *action);
    virtual void callback(SoCallbackAction *action);
    virtual void pick(SoPickAction *action);

    SoINTERNAL
  public:
    static void initClass();

  protected:
    virtual ~SoNormalBinding();

    // Reads stuff into instance. Upgrades old bindings.
    virtual SbBool readInstance(SoInput *in, unsigned short flags);
};

