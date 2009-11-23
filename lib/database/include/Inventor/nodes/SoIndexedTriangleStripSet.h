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
 |	This file defines the SoIndexedTriangleStripSet node class.
 |
 |   Author(s)		: Gavin Bell
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

#ifndef  _SO_INDEXED_TRIANGLE_SET_
#define  _SO_INDEXED_TRIANGLE_SET_

#include <Inventor/nodes/SoIndexedShape.h>
#include <Inventor/fields/SoSFNode.h>
#include <Inventor/nodes/SoVertexProperty.h>

// This coordinate index indicates that the current triangle ends and the
// next triangle begins
#define SO_END_STRIP_INDEX	-1

class SoCoordinateElement;
class SoNormalBundle;
class SoCoordinateElement;

/// Indexed triangle strip set shape node.
/// \ingroup Nodes
/// This shape node constructs triangle strips out of vertices located at
/// the coordinates specified in the #vertexProperty field
/// (from <tt>SoVertexShape</tt>), or the
/// current inherited coordinates.
/// For optimal performance, the #vertexProperty field is recommended.
///
/// <tt>SoIndexedTriangleStripSet</tt> uses the
/// indices in the #coordIndex field (from <tt>SoIndexedShape</tt>) to
/// specify the vertices of the triangle strips. An index of
/// <b>SO_END_STRIP_INDEX</b> (-1) indicates that the current strip has ended
/// and the next one begins.
///
/// The vertices of the faces are transformed by the current
/// transformation matrix. The faces are drawn with the current light
/// model and drawing style.
///
/// Treatment of the current material and normal binding is as follows:
/// <b>PER_PART</b> specifies a material or normal per strip.  <b>PER_FACE</b>
/// binding specifies a material or normal for each triangle.
/// <b>PER_VERTEX</b> specifies a material or normal for each vertex.  The
/// corresponding <b>_INDEXED</b> bindings are the same, but use the
/// #materialIndex or #normalIndex indices (see <tt>SoIndexedShape</tt>).
/// The default material binding is <b>OVERALL</b>. The
/// default normal binding is <b>PER_VERTEX_INDEXED</b>
///
/// If any normals (or materials) are specified, Inventor assumes
/// you provide the correct number of them, as indicated by the binding.
/// You will see unexpected results
/// if you specify fewer normals (or materials) than the shape requires.
/// If no normals are specified, they will be generated automatically.
///
/// \par Action behavior:
/// <b>SoGLRenderAction</b>
/// Draws a strip set based on the current coordinates, normals,
/// materials, drawing style, and so on.
/// <b>SoRayPickAction</b>
/// Picks on the strip set based on the current coordinates and
/// transformation.  Details about the intersection are returned in an
/// <tt>SoFaceDetail</tt>.
/// <b>SoGetBoundingBoxAction</b>
/// Computes the bounding box that encloses all vertices of the strip set
/// with the current transformation applied to them. Sets the center to
/// the average of the coordinates of all vertices.
/// <b>SoCallbackAction</b>
/// If any triangle callbacks are registered with the action, they will be
/// invoked for each successive triangle forming the strips of the set.
///
/// \par File format/defaults:
/// \code
/// SoIndexedTriangleStripSet {
///    vertexProperty	NULL
///    coordIndex	0
///    materialIndex	-1
///    normalIndex	-1
///    textureCoordIndex	-1
/// }
/// \endcode
/// \sa SoCoordinate3,SoDrawStyle,SoFaceDetail,SoIndexedFaceSet,SoTriangleStripSet,SoVertexProperty
class INVENTOR_API SoIndexedTriangleStripSet : public SoIndexedShape {

    SO_NODE_HEADER(SoIndexedTriangleStripSet);

  public:
    /// Creates an indexed triangle strip set node with default settings.
    SoIndexedTriangleStripSet();

  SoEXTENDER public:
    // Implements actions
    virtual void	GLRender(SoGLRenderAction *action);

    // Generates default normals using the given normal bundle. 
    // Returns TRUE.
    virtual SbBool	generateDefaultNormals(SoState *state,
					       SoNormalBundle *nb);
					       
    //Typedef of pointer to method on IndexedTriangleStripSet;
    //This will be used to simplify declaration and initialization.
    typedef void (SoIndexedTriangleStripSet::*PMTSS)(SoGLRenderAction *);
					          
  SoINTERNAL public:
    static void		initClass();

    // This enum is used to indicate the current material or normal binding
    enum Binding {
	OVERALL,
	PER_STRIP,	PER_STRIP_INDEXED,
	PER_TRIANGLE,	PER_TRIANGLE_INDEXED,
	PER_VERTEX,	PER_VERTEX_INDEXED
    };
    
  protected:
    // Generates triangles representing strips
    virtual void	generatePrimitives(SoAction *action);
    

    // Overrides standard method to create an SoFaceDetail instance
    virtual SoDetail *	createTriangleDetail(SoRayPickAction *action,
					     const SoPrimitiveVertex *v1,
					     const SoPrimitiveVertex *v2,
					     const SoPrimitiveVertex *v3,
					     SoPickedPoint *pp);

    ~SoIndexedTriangleStripSet();

  private:
    // Returns current material or normal binding from action's state
    Binding		getMaterialBinding(SoAction *action);
    Binding		getNormalBinding(SoAction *action,
					 SoNormalBundle &nb);

    // Figures out normals, if necessary. Returns TRUE if normals were
    // generated
    SbBool		figureNormals(SoState *state, SoNormalBundle *nb);
    
    // Reset numVertices/numStrips when notified
    virtual void	notify(SoNotList *list);
    
    // Fill in numStrips, numTriangles and numVertices[]
    void countStripsAndTris();
    
    // Number of strips, total number of triangles, and number of
    // vertices per strip:
    int		numStrips;
    int		numTriangles;
    int *	numVertices;

    // 32 different render functions; names are formed like this:
    // Om -- overall material  Pm -- per part material
    // Fm -- per face material Vm -- per vertex material
    // On -- overall normal    Pm -- per part normal
    // Fn -- per face normal   Vm -- per vertex normal
    // T  -- per vertex texture coordinates
    void OmOn(SoGLRenderAction *); void OmOnT(SoGLRenderAction *);
    void OmPn(SoGLRenderAction *); void OmPnT(SoGLRenderAction *);
    void OmFn(SoGLRenderAction *); void OmFnT(SoGLRenderAction *);
    void OmVn(SoGLRenderAction *); void OmVnT(SoGLRenderAction *);
    void PmOn(SoGLRenderAction *); void PmOnT(SoGLRenderAction *);
    void PmPn(SoGLRenderAction *); void PmPnT(SoGLRenderAction *);
    void PmFn(SoGLRenderAction *); void PmFnT(SoGLRenderAction *);
    void PmVn(SoGLRenderAction *); void PmVnT(SoGLRenderAction *);
    void FmOn(SoGLRenderAction *); void FmOnT(SoGLRenderAction *);
    void FmPn(SoGLRenderAction *); void FmPnT(SoGLRenderAction *);
    void FmFn(SoGLRenderAction *); void FmFnT(SoGLRenderAction *);
    void FmVn(SoGLRenderAction *); void FmVnT(SoGLRenderAction *);
    void VmOn(SoGLRenderAction *); void VmOnT(SoGLRenderAction *);
    void VmPn(SoGLRenderAction *); void VmPnT(SoGLRenderAction *);
    void VmFn(SoGLRenderAction *); void VmFnT(SoGLRenderAction *);
    void VmVn(SoGLRenderAction *); void VmVnT(SoGLRenderAction *);

    // Array of function pointers to render functions:
    static PMTSS renderFunc[32];
    
    
};

#endif /* _SO_INDEXED_TRIANGLE_SET_ */
