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
 |	This file defines the SoLineSet node class.
 |
 |   Author(s)		: Paul S. Strauss
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

#pragma once

#include <Inventor/fields/SoMFInt32.h>
#include <Inventor/nodes/SoNonIndexedShape.h>

class SoState;

/// Polyline shape node.
/// \ingroup Nodes
/// This node represents a 3D shape formed by constructing polylines from
/// vertices located at the coordinates specified in the
/// #vertexProperty field (from <tt>SoVertexShape</tt>),
/// or the current inherited coordinates.
/// For optimal performance, the #vertexProperty field is recommended.
///
/// <tt>SoLineSet</tt> uses the
/// coordinates in order, starting with the first one.
/// Each line has a number of vertices specified by
/// a value in the #numVertices field. For example, an <tt>SoLineSet</tt>
/// with a #numVertices of [3,4,2] would use
/// coordinates 1, 2, and 3 for the first line, coordinates 4, 5, 6, and 7
/// for the second line, and coordinates 8 and 9 for the third.
///
/// The number of values in the #numVertices field indicates the
/// number of polylines in the set.
///
/// The coordinates of the line set are transformed by the current
/// cumulative transformation. The lines are drawn with the current light
/// model and drawing style (drawing style <b>FILLED</b> is treated as
/// <b>LINES</b>).
///
/// Treatment of the current material and normal binding is as follows:
/// The <b>PER_PART</b> binding specifies a material or normal for each
/// segment of the line. The <b>PER_FACE</b> binding specifies a material or
/// normal for each polyline. The <b>_INDEXED</b> bindings are equivalent to
/// their non-indexed counterparts. The default material binding is
/// <b>OVERALL</b>. The default normal binding is
/// <b>PER_VERTEX</b>.
///
/// The current complexity value has no effect on the rendering of line
/// sets.
///
/// \par Action behavior:
/// <b>SoGLRenderAction</b>
/// Draws lines based on the current coordinates, normals, materials,
/// drawing style, and so on.
/// <b>SoRayPickAction</b>
/// Picks lines based on the current coordinates and transformation.
/// Details about the intersection are returned in an <tt>SoLineDetail</tt>.
/// <b>SoGetBoundingBoxAction</b>
/// Computes the bounding box that encloses all vertices of the line set
/// with the current transformation applied to them.  Sets the center to
/// the average of the coordinates of all vertices.
/// <b>SoCallbackAction</b>
/// If any line segment callbacks are registered with the action, they will
/// be invoked for each successive segment in the line set.
///
/// \par File format/defaults:
/// \code
/// SoLineSet {
///    vertexProperty	NULL
///    startIndex	0
///    numVertices	-1
/// }
/// \endcode
/// \sa SoCoordinate3,SoDrawStyle,SoIndexedLineSet,SoLineDetail,SoVertexProperty
class INVENTOR_API SoLineSet : public SoNonIndexedShape {

    SO_NODE_HEADER(SoLineSet);

  public:
    // Fields
    SoMFInt32 numVertices; ///< Number of vertices per polyline.

    /// Creates a line set node with default settings.
    SoLineSet();

    SoEXTENDER
  public:
    // Implements actions
    virtual void GLRender(SoGLRenderAction *action);

    // Redefine this to tell open caches that they contain lines
    virtual void getBoundingBox(SoGetBoundingBoxAction *action);

    SoINTERNAL
  public:
    static void initClass();

    // This enum is used to indicate the current material or normal binding
    enum Binding { OVERALL, PER_LINE, PER_SEGMENT, PER_VERTEX };

    // Typedef of pointer to method on IndexedLineSet;
    // This will be used to simplify declaration and initialization.
    typedef void (SoLineSet::*PMLS)(SoGLRenderAction *);

  protected:
    // Generates line segments representing line set
    virtual void generatePrimitives(SoAction *action);

    // Computes bounding box of line set
    virtual void computeBBox(SoAction *action, SbBox3f &box, SbVec3f &center);

    // Overrides standard method to create an SoLineDetail instance
    virtual SoDetail *createLineSegmentDetail(SoRayPickAction *        action,
                                              const SoPrimitiveVertex *v1,
                                              const SoPrimitiveVertex *v2,
                                              SoPickedPoint *          pp);

    virtual ~SoLineSet();

  private:
    // Returns current material or normal binding from action's state
    static Binding getMaterialBinding(SoAction *action);
    static Binding getNormalBinding(SoAction *action);

    // Returns TRUE if normal binding is AUTO and there aren't enough
    // PER_VERTEX normals, in which case we turn off lighting and draw
    // BASE_COLOR.
    SbBool wouldGenerateNormals(SoState *state);

    // Blow vpCache when vertexProperty field changes:
    virtual void notify(SoNotList *list);

    // This stores the total number of vertices; we use this
    // information to influence Separator's auto-caching algorithm
    // (shapes with very few triangles should be cached because
    // traversing them can be expensive, shapes with lots of triangles
    // shouldn't be cached because they'll take up too much memory).
    int totalNumVertices;

    // 32 different render functions; names are formed like this:
    // Om -- overall material  Pm -- per part material
    // Fm -- per face material Vm -- per vertex material
    // On -- overall normal    Pm -- per part normal
    // Fn -- per face normal   Vm -- per vertex normal
    // T  -- per vertex texture coordinates
    void OmOn(SoGLRenderAction *);
    void OmOnT(SoGLRenderAction *);
    void OmPn(SoGLRenderAction *);
    void OmPnT(SoGLRenderAction *);
    void OmFn(SoGLRenderAction *);
    void OmFnT(SoGLRenderAction *);
    void OmVn(SoGLRenderAction *);
    void OmVnT(SoGLRenderAction *);
    void PmOn(SoGLRenderAction *);
    void PmOnT(SoGLRenderAction *);
    void PmPn(SoGLRenderAction *);
    void PmPnT(SoGLRenderAction *);
    void PmFn(SoGLRenderAction *);
    void PmFnT(SoGLRenderAction *);
    void PmVn(SoGLRenderAction *);
    void PmVnT(SoGLRenderAction *);
    void FmOn(SoGLRenderAction *);
    void FmOnT(SoGLRenderAction *);
    void FmPn(SoGLRenderAction *);
    void FmPnT(SoGLRenderAction *);
    void FmFn(SoGLRenderAction *);
    void FmFnT(SoGLRenderAction *);
    void FmVn(SoGLRenderAction *);
    void FmVnT(SoGLRenderAction *);
    void VmOn(SoGLRenderAction *);
    void VmOnT(SoGLRenderAction *);
    void VmPn(SoGLRenderAction *);
    void VmPnT(SoGLRenderAction *);
    void VmFn(SoGLRenderAction *);
    void VmFnT(SoGLRenderAction *);
    void VmVn(SoGLRenderAction *);
    void VmVnT(SoGLRenderAction *);

    // Array of function pointers to render functions:
    static PMLS renderFunc[32];
};

