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
 |   $Revision: 1.1 $
 |
 |   Description:
 |	This file defines the SoSphere node class.
 |
 |   Author(s)		: Paul S. Strauss, Nick Thompson
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

#pragma once

#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/nodes/SoShape.h>

/// Sphere shape node.
/// \ingroup Nodes
/// This node represents a sphere. By default, the sphere is centered at
/// the origin and has a radius of 1. The sphere is transformed by the
/// current cumulative transformation and is drawn with the current
/// lighting model, drawing style, material, and geometric complexity.
///
/// A sphere does not have faces or parts. Therefore, the sphere ignores
/// material and normal bindings, using the first material for the entire
/// sphere and using its own normals. When a texture is applied to a
/// sphere, the texture covers the entire surface, wrapping
/// counterclockwise from the back of the sphere. The texture has a seam
/// at the back on the yz-plane.
///
/// \par Action behavior:
/// <b>SoGLRenderAction</b>
/// Draws sphere based on the current coordinates, materials,
/// drawing style, and so on.
/// <b>SoRayPickAction</b>
/// Intersects the ray with the sphere. No details are created for
/// intersections.
/// <b>SoGetBoundingBoxAction</b>
/// Computes the bounding box that encloses the sphere.
/// <b>SoCallbackAction</b>
/// If any triangle callbacks are registered with the action, they will
/// be invoked for each successive triangle that approximates the sphere.
///
/// \par File format/defaults:
/// \code
/// SoSphere {
///    radius	1
/// }
/// \endcode
/// \sa SoCone, SoCube, SoCylinder
class SoSphere : public SoShape {

    SO_NODE_HEADER(SoSphere);

  public:
    // Fields
    SoSFFloat radius; ///< Radius of sphere. This must be greater than 0.

    /// Creates a sphere node with default settings.
    SoSphere();

    SoEXTENDER
  public:
    virtual void GLRender(SoGLRenderAction *action);
    virtual void rayPick(SoRayPickAction *action);

    SoINTERNAL
  public:
    static void initClass();

  protected:
    // Generates triangles representing a sphere
    virtual void generatePrimitives(SoAction *action);

    // Computes bounding box of sphere
    virtual void computeBBox(SoAction *action, SbBox3f &box, SbVec3f &center);

    virtual ~SoSphere();

  private:
    // These render the sphere
    void GLRenderGeneric(SoGLRenderAction *action, SbBool sendNormals,
                         SbBool doTextures);

    // Computes tesselation depth based on current complexity
    int computeDepth(SoAction *action);
};

