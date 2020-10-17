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
 |	This file defines the SoVertexProperty node class, and the
 |       SoVertexPropertyCache class.
 |
 |   Author(s)		: Alan Norton, Gavin Bell
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

#pragma once

#include <Inventor/fields/SoMFUInt32.h>
#include <Inventor/fields/SoMFVec3f.h>
#include <Inventor/fields/SoMFVec2f.h>
#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/nodes/SoNormalBinding.h>
#include <Inventor/nodes/SoMaterialBinding.h>
#include <Inventor/nodes/SoTextureCoordinateBinding.h>

/// Vertex property node.
/// \ingroup Nodes
/// This property node may be used to efficiently specify coordinates, normals,
/// texture coordinates, colors, transparency values, material
/// binding and normal binding for vertex-based shapes, i.e., shapes of
/// class <tt>SoVertexShape</tt>.  An <tt>SoVertexProperty</tt> node can be used
/// as a child of a group node in a scene graph, in which case the
/// properties it specifies are inherited by subsequent shape nodes in the
/// graph.  It can also be directly referenced as the <tt>VertexProperty</tt>
/// <tt>SoSFField</tt> of a vertex-based shape, bypassing scene graph
/// inheritance.
///
/// When directly referenced by a <b>VertexProperty</b> <tt>SoSFField</tt> of a
/// vertex-based shape, the <tt>SoVertexProperty</tt> node is the most efficient
/// way of specifying vertex-based shapes.  Use of the directly referenced
/// <tt>SoVertexProperty</tt> node results in significantly faster scene
/// rendering than scene graph inheritance of vertex properties, provided all
/// required vertex properties are specified in the <tt>SoVertexProperty</tt>
/// node.
///
/// Because the class <tt>SoVertexProperty</tt> is derived from <tt>SoNode</tt>,
/// a vertex property node can be inserted as a child node in a scene graph.
/// When inserted as a node in a scene graph, the <tt>SoVertexProperty</tt> node
/// is traversed as any other property node and the properties it specifies are
/// inherited by subsequent shape nodes in the scene graph.  It specifies
/// the current material and normal bindings, and can be used to specify
/// the current 3D
/// coordinates, the current normals, the current texture
/// coordinates, the current diffuse colors, and the current transparencies.
///
/// All multiple-valued fields in the <tt>SoVertexProperty</tt> node are
/// optional. If a field is not present (i.e. if it has 0 values), then shapes
/// that require the missing information are required to obtain it from the
/// current traversal state.   However, users are cautioned that, for optimal
/// performance, the vertex property node should be referenced as the
/// <b>VertexProperty</b> field of an <tt>SoVertexShape</tt>, and should specify
/// in its fields all values required to render that shape.
///
/// The various fields in a vertex property node can be used in place of
/// corresponding fields in other property nodes, as follows:  The #vertex field
/// contains 3D coordinates, as in the <b>point</b> field of an
/// <tt>SoCoordinate3</tt> node. The #normal field contains normal vectors, as
/// in the <b>vector</b> field of the #SoNormal node. The #orderedRGBA field
/// contains packed colors in the hexadecimal format <tt>0xrrggbbaa</tt>, where
/// - <tt>rr</tt> is the red value (between 00 and 0xFF hex)
/// - <tt>gg</tt> is the green value (between 00 and 0xFF hex)
/// - <tt>bb</tt> is the blue value (between 00 and 0xFF hex)
/// - <tt>aa</tt> is the alpha value (between 00 = transparent and 0xFF =
/// opaque).
///
/// The packed colors are equivalent to an <tt>SoPackedColor</tt> node, and
/// provide values for both diffuse color and transparency.
/// The #texCoord field replaces
/// the <b>point</b> field of the <tt>SoTextureCoordinate2</tt> node.
///
/// If the transparency type is <tt>SoGLRenderAction::SCREEN_DOOR</tt>, only the
/// first transparency value will be used.  With other transparency types,
/// multiple transparencies will be used.
///
/// The #materialBinding field replaces the <b>value</b> field of the
/// <tt>SoMaterialBinding</tt> node.  The #materialBinding field in a directly
/// referenced <tt>SoVertexProperty</tt> node has no effect unless there is a
/// nonempty #orderedRGBA field, in which case the material binding specifies
/// the assignment of diffuse colors and alpha values to the shape. The
/// #materialBinding field can take as value any of the material bindings
/// supported by Inventor.
///
/// The #normalBinding field replaces the <b>value</b> field of the
/// <tt>SoNormalBinding</tt> node.   The #normalBinding field of
/// a directly referenced <tt>SoVertexProperty</tt> node has no effect
/// unless there is a nonempty
/// #normal field, in which case the normal binding specifies the assignment
/// of normal vectors to the shape.  The value of the #normalBinding
/// field can be any of the normal bindings supported by Inventor.
///
/// \par Action behavior:
/// <b>SoGLRenderAction, SoCallbackAction, SoPickAction</b>
/// When traversed in a scene graph, sets coordinates, normals, texture
/// coordinates, diffuse colors, transparency, normal binding and material
/// binding in current traversal state.  If not traversed, has no effect on
/// current traversal state associated with action.  The normalBinding field has
/// no effect if there are no normals.  The materialBinding has no effect if
/// there are no packed colors. <b>SoGetBoundingBoxAction</b> When traversed in
/// a scene graph, sets coordinates in current traversal state.  If not
/// traversed, has no effect on current traversal state associated with action.
///
/// \par File format/defaults:
/// \code
/// SoVertexProperty {
///    vertex      	[  ]
///    normal      	[  ]
///    texCoord  	[  ]
///    orderedRGBA	[  ]
///    materialBinding	OVERALL
///    normalBinding	PER_VERTEX_INDEXED
/// }
/// \endcode
/// \sa SoIndexedTriangleStripSet, SoIndexedFaceSet, SoIndexedLineSet,
/// SoTriangleStripSet, \sa SoLineSet, SoFaceSet, SoPointSet, SoQuadMesh,
/// SoVertexShape, SoIndexedShape, SoNonindexedShape
class INVENTOR_API SoVertexProperty : public SoNode {

    SO_NODE_HEADER(SoVertexProperty);

  public:
    // Fields:
    SoMFVec3f vertex;   ///< Coordinate point(s)
    SoMFVec2f texCoord; ///< Texture coordinate(s)

    SoMFVec3f normal;        ///< Normal vector(s)
    SoSFEnum  normalBinding; ///< Ignored unless normal field set

    SoMFUInt32 orderedRGBA;     ///< Packed color(s), including transparencies.
    SoSFEnum   materialBinding; ///< Ignored unless orderedRGBA field set

    enum Binding {
        OVERALL = SoMaterialBindingElement::OVERALL, ///< Whole object has same
                                                     ///< material/normal
        PER_PART =
            SoMaterialBindingElement::PER_PART, ///< One material/normal for
                                                ///< each part of object
        PER_PART_INDEXED =
            SoMaterialBindingElement::PER_PART_INDEXED, ///< One material/normal
                                                        ///< for each part,
                                                        ///< indexed
        PER_FACE =
            SoMaterialBindingElement::PER_FACE, ///< One material/normal for
                                                ///< each face of object
        PER_FACE_INDEXED =
            SoMaterialBindingElement::PER_FACE_INDEXED, ///< One material/normal
                                                        ///< for each face,
                                                        ///< indexed
        PER_VERTEX =
            SoMaterialBindingElement::PER_VERTEX, ///< One material/normal for
                                                  ///< each vertex of object
        PER_VERTEX_INDEXED = SoMaterialBindingElement::
            PER_VERTEX_INDEXED ///< One material/normal for each vertex, indexed
    };

    /// Creates an SoVertexProperty node with default settings.
    SoVertexProperty();

    SoEXTENDER
  public:
    virtual void doAction(SoAction *action);
    virtual void GLRender(SoGLRenderAction *action);
    virtual void getBoundingBox(SoGetBoundingBoxAction *action);
    virtual void callback(SoCallbackAction *action);
    virtual void pick(SoPickAction *action);

    bool isTransparent() const { return transparent; }
    SoINTERNAL
  public:
    static void initClass();

  protected:
    // check for transparency when field changes.
    virtual void notify(SoNotList *list);
    // store whether transparent or not
    bool transparent;

    virtual ~SoVertexProperty();
};

//////////////////////////////////////////////////////////////////////////////
//
//   Description:
//	The SoVertexPropertyCache class is a convenient way for vertex-based
//      shapes to store information on how they should render
//      themselves.  It takes care of some messiness that they'd
//      otherwise have to figure out.  It is designed to be a very
//      lightweight class.
//
//////////////////////////////////////////////////////////////////////////////

#include <Inventor/elements/SoShapeStyleElement.h>

typedef void SoVPCacheFunc(const char *const);

SoEXTENDER
class INVENTOR_API SoVertexPropertyCache {
  public:
    // API used by shapes:

    SbBool mightNeedSomethingFromState(const SoShapeStyleElement *sse) const {
        return needFromState & sse->getRenderCaseMask();
    }

    void fillInCache(const SoVertexProperty *, SoState *);

    SbBool shouldGenerateNormals(const SoShapeStyleElement *sse) const {
        return (generateNormals && sse->needNormals());
    }

    SbBool shouldGenerateTexCoords(const SoShapeStyleElement *sse) const {
        return (generateTexCoords && sse->needTexCoords());
    }

    int getRenderCase(const SoShapeStyleElement *sse) const {
        return renderCase & sse->getRenderCaseMask();
    }

    SbBool haveTexCoordsInVP() { return needFromState & TEXTURE_FUNCTION_BIT; }

    void sendVertex(const char *vp) const { (*vertexFunc)(vp); }
    void sendNormal(const char *np) const { (*normalFunc)(np); }
    void sendColor(const char *cp) const { (*colorFunc)(cp); }
    void sendTexCoord(const char *tcp) const { (*texCoordFunc)(tcp); }

    int getNumVertices() const { return numVerts; }
    int getNumNormals() const { return numNorms; }
    int getNumColors() const { return numColors; }
    int getNumTexCoords() const { return numTexCoords; }

    const char *getVertices(int i) const {
        return vertexPtr + vertexStride * i;
    }
    const char *getNormals(int i) const { return normalPtr + normalStride * i; }
    const char *getColors(int i) const { return colorPtr + colorStride * i; }
    const char *getTexCoords(int i) const {
        return texCoordPtr + texCoordStride * i;
    }

    SbBool colorIsInVtxProp() const { return colorIsInVP; }

    // Indicates if non-opaque transparency is in vertex property.
    SbBool transpIsInVtxProp() const { return transpIsInVP; }

    int getVertexStride() const { return vertexStride; }
    int getNormalStride() const { return normalStride; }
    int getColorStride() const { return colorStride; }
    int getTexCoordStride() const { return texCoordStride; }

    SoNormalBindingElement::Binding getNormalBinding() {
        return (SoNormalBindingElement::Binding)normalBinding;
    }

    SoMaterialBindingElement::Binding getMaterialBinding() {
        return (SoMaterialBindingElement::Binding)materialBinding;
    }

    SoTextureCoordinateBindingElement::Binding getTexCoordBinding() {
        return (SoTextureCoordinateBindingElement::Binding)texCoordBinding;
    }

    void invalidate() { needFromState = ALL_FROM_STATE_BITS; }

    // Constructor.   Initializes everything.
    SoVertexPropertyCache();

  public:
    // Tables of functions, data, and strides to increment through data.
    SoVPCacheFunc *vertexFunc;
    const char *   vertexPtr;
    int            vertexStride, numVerts;
    SoVPCacheFunc *normalFunc;
    const char *   normalPtr;
    int            normalStride, numNorms;
    SoVPCacheFunc *colorFunc;
    const char *   colorPtr;
    int            colorStride, numColors;
    SoVPCacheFunc *texCoordFunc;
    const char *   texCoordPtr;
    int            texCoordStride, numTexCoords;

    SbBool generateNormals;
    SbBool generateTexCoords;

    unsigned char needFromState;

    // 32 rendering cases.  Non-indexed shapes can get away with just
    // looking at the low 5 bits.  The bits are arranged like this:
    // 43210  BITS
    // -----  ----
    // 00...  Overall color
    // 01...  Part color
    // 10...  Face color
    // 11...  Vtx color
    // ..00.  Overall/No norm
    // ..01.  Part norm
    // ..10.  Face norm
    // ..11.  Vtx norm
    // ....?  Explicit exture coordinates needed
    SoINTERNAL
  public:
    enum {
        COLOR_BITS = 0x18,
        PART_COLOR = 0x8,
        FACE_COLOR = 0x10,
        VERTEX_COLOR = 0x18,

        NORMAL_BITS = 0x6,
        PART_NORMAL = 0x2,
        FACE_NORMAL = 0x4,
        VERTEX_NORMAL = 0x6,

        TEXCOORD_BIT = 0x1
    };
    // Bit-mask, set by ::fillInCache method
    // Bits use same as above, plus one for coords and one for
    // overrides (diffuse color, material binding, transparency)
    // plus one for texture function (if tcoords in VP)

    enum {
        TEXTURE_FUNCTION_BIT = 0x80,
        OVERRIDE_FROM_STATE_BIT = 0x40,
        COORD_FROM_STATE_BIT = 0x20,
        COLOR_FROM_STATE_BITS = COLOR_BITS,
        NORMAL_FROM_STATE_BITS = NORMAL_BITS,
        TEXCOORD_FROM_STATE_BIT = TEXCOORD_BIT,
        ALL_FROM_STATE_BITS = 0xFF
    };

  private:
    //
    // VertexPropertyCache also stores whether or not colors, normals and
    // texture coordinates should be indexed.  Suggestion for writing
    // rendering loops:
    //
    // To minimize loop overhead, loop unrolling is a good idea.
    // Doing two iterations per loop makes it about 20% faster.  Doing
    // three iterations per loop makes it about 28% faster (only 8%
    // more than two iterations).
    //
    // On 200MHZ machines, an unrolled-by-3 loop can do close to 5
    // million iterations per second, assuming one call per iteration
    // (e.g. one-color, unlit polygons).
    //
    unsigned char normalBinding;
    unsigned char materialBinding;
    unsigned char texCoordBinding;
    unsigned char renderCase;
    unsigned char colorIsInVP;
    unsigned char transpIsInVP;
};

