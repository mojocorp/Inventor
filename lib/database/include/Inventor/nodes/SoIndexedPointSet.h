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
 |	This file defines the SoIndexedPointSet node class.
 |
 |   Author(s)		: Morgan Leborgne
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

#pragma once

#include <Inventor/fields/SoSFInt32.h>
#include <Inventor/nodes/SoIndexedShape.h>

/// Indexed point set shape node.
/// \ingroup Nodes
/// This node represents a set of points located at the
/// coordinates specified by the #vertexProperty field (from
/// <tt>SoVertexShape</tt>) or the current inherited coordinates. For optimal
/// performance, the #vertexProperty field is recommended.
///
/// <tt>SoPointSet</tt> uses the indices in the #coordIndex field (from
/// <tt>SoIndexedShape</tt>) to specify the points.
///
/// The coordinates of the point set are transformed by the current
/// cumulative transformation. The points are drawn with the current light
/// model and drawing style (drawing styles <b>FILLED</b> and <b>LINES</b> are
/// treated as <b>POINTS</b>).
///
/// Treatment of the current material and normal binding is as follows:
/// <b>PER_PART</b>, <b>PER_FACE</b>, and <b>PER_VERTEX</b> bindings bind one
/// material or normal to each point. The default material binding is
///  <b>OVERALL</b>. The default normal binding is
/// <b>PER_VERTEX</b>.
///
/// \par Action behavior:
/// <b>SoGLRenderAction</b>
/// Draws points based on the current coordinates, normals, materials,
/// drawing style, and so on.
/// <b>SoRayPickAction</b>
/// Picks points based on the current coordinates and transformation.
/// Details about the intersection are returned in an <tt>SoPointDetail</tt>.
/// <b>SoGetBoundingBoxAction</b>
/// Computes the bounding box that encloses all points in the set with the
/// current transformation applied to them.  Sets the center to the
/// average of the coordinates of all points.
/// <b>SoCallbackAction</b>
/// If any point callbacks are registered with the action, they will be
/// invoked for each point in the set.
///
/// \par File format/defaults:
/// \code
/// SoIndexedPointSet {
///    vertexProperty      NULL
///    coordIndex          0
///    materialIndex      -1
///    normalIndex        -1
///    textureCoordIndex  -1
///  }
/// \endcode
/// \sa SoPointSet,SoCoordinate3,SoDrawStyle,SoPointDetail,SoVertexProperty
class INVENTOR_API SoIndexedPointSet : public SoIndexedShape {

    SO_NODE_HEADER(SoIndexedPointSet);

  public:
    /// Creates a point set node with default settings.
    SoIndexedPointSet();

    SoEXTENDER
  public:
    // Implements actions
    virtual void GLRender(SoGLRenderAction *action);

    // Redefine this to tell open caches that they contain points
    virtual void getBoundingBox(SoGetBoundingBoxAction *action);

    SoINTERNAL
  public:
    static void initClass();

  protected:
    // Generates points representing point set
    virtual void generatePrimitives(SoAction *action);

    // Overrides standard method to create an SoPointDetail instance
    virtual SoDetail *createPointDetail(SoRayPickAction *        action,
                                        const SoPrimitiveVertex *v,
                                        SoPickedPoint *          pp);

  protected:
    virtual ~SoIndexedPointSet();

  private:
    // Returns TRUE if materials/normals are bound to individual points
    SbBool areMaterialsPerPoint(SoAction *action) const;
    SbBool areNormalsPerPoint(SoAction *action) const;
};

