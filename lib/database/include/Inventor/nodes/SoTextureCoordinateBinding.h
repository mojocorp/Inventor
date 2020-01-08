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
 |	This file defines the SoTextureCoordinateBinding node class.
 |
 |   Author(s)		: John Rohlf
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

#ifndef _SO_TEXTURE_COORDINATE_BINDING_
#define _SO_TEXTURE_COORDINATE_BINDING_

#include <Inventor/elements/SoTextureCoordinateBindingElement.h>
#include <Inventor/fields/SoSFEnum.h>
#include <Inventor/nodes/SoSubNode.h>

/// Node that specifies how texture coordinates are bound to shapes.
/// \ingroup Nodes
/// This node specifies how the current texture coordinates are bound to
/// vertex-based shapes that follow in the scene graph. The <b>DEFAULT</b>
/// binding causes each shape to define its own default coordinates.
/// These default coordinates typically cause a texture to be mapped
/// across the whole surface of a shape.
///
/// \par Action behavior:
/// <b>SoGLRenderAction, SoCallbackAction</b>
/// Sets the current texture coordinate binding type.
///
/// \par File format/defaults:
/// \code
/// SoTextureCoordinateBinding {
///    value	PER_VERTEX_INDEXED
/// }
/// \endcode
/// \sa
/// SoMaterialBinding,SoNormalBinding,SoTexture2,SoTexture2Transform,SoTextureCoordinate2,SoTextureCoordinateFunction,SoVertexShape
class SoTextureCoordinateBinding : public SoNode {

    SO_NODE_HEADER(SoTextureCoordinateBinding);

  public:
    /// Binding value
    enum Binding {
        /// Current texture coordinates are applied sequentially to vertices of
        /// shape
        PER_VERTEX = SoTextureCoordinateBindingElement::PER_VERTEX,
        /// Current texture coordinates are indexed per vertex
        PER_VERTEX_INDEXED =
            SoTextureCoordinateBindingElement::PER_VERTEX_INDEXED,
    };

    // Fields
    SoSFEnum value; ///< Specifies how to bind texture coordinates to shapes.

    /// Creates a texture coordinate binding node with default settings.
    SoTextureCoordinateBinding();

    SoEXTENDER
  public:
    virtual void doAction(SoAction *action);
    virtual void callback(SoCallbackAction *action);
    virtual void GLRender(SoGLRenderAction *action);
    virtual void pick(SoPickAction *action);

    SoINTERNAL
  public:
    static void initClass();

  protected:
    virtual ~SoTextureCoordinateBinding();

    // Reads stuff into instance. Upgrades old bindings.
    virtual SbBool readInstance(SoInput *in, unsigned short flags);
};

#endif /* _SO_TEXTURE_COORDINATE_BINDING_ */
