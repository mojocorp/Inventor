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
 |	This file defines the SoIndexedLineSet node class.
 |
 |   Author(s)		: Paul S. Strauss, Nick Thompson
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

#ifndef  _SO_INDEXED_LINE_SET_
#define  _SO_INDEXED_LINE_SET_

#include <Inventor/nodes/SoIndexedShape.h>

class SoState;

//////////////////////////////////////////////////////////////////////////////
//
//  Class: SoIndexedLineSet
//
//  Indexed set of (poly)lines. Each line consists of 2 or more
//  vertices, each of which is denoted by an integer index (from the
//  coordIndex field) into the current coordinates. Depending on the
//  current material and normal binding values, the materials and
//  normals for the lines or vertices may be accessed in order or
//  indexed. If they are indexed, the materialIndex and normalIndex
//  fields are used. Texture coordinates may also be indexed, using
//  the textureCoordIndex field.
//
//////////////////////////////////////////////////////////////////////////////

// This coordinate index indicates that the current line ends and the
// next line begins
#define SO_END_LINE_INDEX	(-1)

/// Indexed polyline shape node.
/// \ingroup Nodes
/// This node represents a 3D shape formed by constructing polylines from
/// vertices located at the coordinates specified in the #vertexProperty
/// field (from <tt>SoVertexShape</tt>), or the current inherited coordinates.
/// For optimal performance, the #vertexProperty field is recommended.
///
/// <tt>SoIndexedLineSet</tt>
/// uses the indices in the #coordIndex field (from <tt>SoIndexedShape</tt>)
/// to specify the polylines. An index of <b>SO_END_LINE_INDEX</b> (-1)
/// indicates that the current polyline has ended and the next one begins.
///
/// The coordinates of the line set are transformed by the current
/// cumulative transformation. The lines are drawn with the current light
/// model and drawing style (drawing style <b>FILLED</b> is treated as
/// <b>LINES</b>).
///
/// Treatment of the current material and normal binding is as follows:
/// The <b>PER_PART</b> binding specifies a material or normal for each
/// segment of the line. The <b>PER_FACE</b> binding specifies a material or
/// normal for each polyline.  <b>PER_VERTEX</b> specifies a material or
/// normal for each vertex.  The corresponding <b>_INDEXED</b> bindings are
/// the same, but use the #materialIndex or #normalIndex indices
/// (see <tt>SoIndexedShape</tt>). The default material binding is
/// <b>OVERALL</b>. The default normal binding is
/// <b>PER_VERTEX_INDEXED</b>
///
/// The current complexity value has no effect on the rendering of indexed
/// line sets.
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
/// SoIndexedLineSet {
///    vertexProperty	NULL
///    coordIndex	0
///    materialIndex	-1
///    normalIndex	-1
///    textureCoordIndex	-1
/// }
/// \endcode
/// \sa SoCoordinate3,SoDrawStyle,SoLineDetail,SoLineSet,SoVertexProperty
class INVENTOR_API SoIndexedLineSet : public SoIndexedShape {

    SO_NODE_HEADER(SoIndexedLineSet);

  public:
    // Inherits all fields

    /// Creates an indexed line set node with default settings.
    SoIndexedLineSet();

  SoEXTENDER public:
    // Implements actions
    virtual void	GLRender(SoGLRenderAction *action);

    // Redefine this to tell open caches that they contain lines
    virtual void	getBoundingBox(SoGetBoundingBoxAction *action);
					       
    //Typedef of pointer to method on IndexedLineSet;
    //This will be used to simplify declaration and initialization.
    typedef void (SoIndexedLineSet::*PMILS)(SoGLRenderAction *);
					          
  SoINTERNAL public:
    static void		initClass();

    // This enum is used to indicate the current material or normal binding
    enum Binding {
	OVERALL,
	PER_SEGMENT,	PER_SEGMENT_INDEXED,
	PER_LINE,	PER_LINE_INDEXED,
	PER_VERTEX,	PER_VERTEX_INDEXED
    };

  protected:
    // Generates line segments representing line set
    virtual void	generatePrimitives(SoAction *action);

    // Overrides standard method to create an SoLineDetail instance
    virtual SoDetail *	createLineSegmentDetail(SoRayPickAction *action,
						const SoPrimitiveVertex *v1,
						const SoPrimitiveVertex *v2,
						SoPickedPoint *pp);
    virtual ~SoIndexedLineSet();

  private:
    // Returns current material or normal binding from action's state
    static Binding	getMaterialBinding(SoAction *action);
    static Binding	getNormalBinding(SoAction *action);

    // Reset numVertices/numSegments/numPolylines when notified
    virtual void	notify(SoNotList *list);
    
    // Fill in numPolylines, numSegments 
    void countPolylinesAndSegments();
    
    // Number of polylines, number of line segments, and vertices per polyline

    int		numSegments;
    int		numPolylines;
    int *	numVertices;
        
    // Returns TRUE if normal binding is AUTO and there aren't enough
    // PER_VERTEX normals, in which case we turn off lighting and draw
    // BASE_COLOR.
    SbBool	wouldGenerateNormals(SoState *state);

    void GLRenderGeneric(SoGLRenderAction *action);
};

#endif /* _SO_INDEXED_LINE_SET_ */
    
