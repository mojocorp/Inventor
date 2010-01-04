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
 |	Defines the SoCallbackAction class
 |
 |   Author(s)		: Dave Immel, Thad Beier
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

#ifndef  _SO_CALLBACK_ACTION_
#define  _SO_CALLBACK_ACTION_

#include <Inventor/actions/SoSubAction.h>
#include <Inventor/nodes/SoComplexity.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoLightModel.h>
#include <Inventor/nodes/SoMaterialBinding.h>
#include <Inventor/nodes/SoNormalBinding.h>
#include <Inventor/nodes/SoPickStyle.h>
#include <Inventor/nodes/SoShapeHints.h>
#include <Inventor/nodes/SoTexture2.h>
#include <Inventor/nodes/SoTextureCoordinateBinding.h>
#include <Inventor/nodes/SoUnits.h>
#include <Inventor/SbVec2f.h>
#include <Inventor/SbVec4f.h>
#include <Inventor/SbMatrix.h>
#include <Inventor/SbViewVolume.h>

class SoPrimitiveVertex;
class SoShape;

//
// Typedefs for callback routines used with the callbacks for
// generating primitives.
//

typedef void SoTriangleCB(void *userData,
                          SoCallbackAction *action,
                          const SoPrimitiveVertex *v1,
                          const SoPrimitiveVertex *v2,
                          const SoPrimitiveVertex *v3);

typedef void SoLineSegmentCB(void *userData, SoCallbackAction *action,
                             const SoPrimitiveVertex *v1,
                             const SoPrimitiveVertex *v2);

typedef void SoPointCB(void *userData, SoCallbackAction *action,
                       const SoPrimitiveVertex *v);

/// Performs a generic traversal of the scene graph.
/// \ingroup Actions
/// This action defines a generic traversal of the scene graph. The user
/// can specify callback functions for node types or paths; when those
/// node types or paths are encountered during traversal, the user's
/// callback function is called.
///
///
/// In addition, callback functions can be registered for primitives
/// generated by shapes in the scene graph. Most shape types can generate
/// primitives that represent or approximate their geometries.
/// <em>Triangle</em> primitives are used for all surfaces (such as cubes, face
/// sets, or 3D text), <em>line segment</em> primitives are used for line
/// shapes, and <em>point</em> primitives are used for point shapes. Note that
/// the type of primitives generated for a shape is the same, regardless
/// of drawing style or other properties.
///
///
/// Most of the methods on this class access information
/// from the traversal state. They should be called only by callback
/// functions that are invoked during traversal, so there is a valid state
/// to work with.
/// \sa SoCallback, SoEventCallback, SoShape
class INVENTOR_API SoCallbackAction : public SoAction {

    SO_ACTION_HEADER(SoCallbackAction);

public:

    /// Possible responses from a pre or post callback
    enum Response {
        CONTINUE,  ///< Continue as usual
        ABORT,   ///< Stop traversing the rest of the graph
        PRUNE   ///< Do not traverse children of this node
    };

    /// The SoCallbackActionCB typedef is defined within the class, since
    /// it needs to refer to the Response enumerated type.
    /// The first argument is the data pointer that the user supplied
    /// when the callback was registered. The second argument is the
    /// action, from which the state can be extracted. The third
    /// argument is the node that the callback is called from.
    typedef Response SoCallbackActionCB(void *userData,
                                        SoCallbackAction *action,
                                        const SoNode *node);

    /// Constructor.
    SoCallbackAction();

    /// Destructor
    virtual ~SoCallbackAction();

    /// \name Adding callback functions:
    /// These add a callback function to call when a node of the given
    /// type is encountered during traversal.
    /// The PreCallback is called just before
    /// the node is traversed, and the PostCallback is called just after. The
    /// value returned by a callback function indicates whether the action
    /// should continue with the traversal.
    /// @{
    void addPreCallback(SoType type, SoCallbackActionCB *cb, void *data);
    void addPostCallback(SoType type, SoCallbackActionCB *cb, void *data);

    /// These can be used when the action is applied to a path.
    /// They add functions to be called when the tail node of the path is
    /// traversed.
    void addPreTailCallback(SoCallbackActionCB *cb, void *data);
    void addPostTailCallback(SoCallbackActionCB *cb, void *data);

    /// Routines to add callbacks for generated primitives (triangles, line segments, and points) of a shape of a given type.
    /// Primitives will be generated and these callbacks will be
    /// invoked for all shape nodes of that type.
    void addTriangleCallback(SoType type, SoTriangleCB *cb, void *data);
    void addLineSegmentCallback(SoType type, SoLineSegmentCB *cb, void *data);
    void addPointCallback(SoType type, SoPointCB *cb, void *data);
    /// @}

    /// \name Complexity:
    /// Returns complexity information from the state.
    /// @{
    float getComplexity() const;
    SoComplexity::Type getComplexityType() const;
    /// @}

    /// \name Coordinates:
    /// Returns the current coordinates from the state.
    /// @{
    int32_t getNumCoordinates() const;
    const SbVec3f & getCoordinate3(int index) const;
    const SbVec4f & getCoordinate4(int index) const;
    /// @}

    /// \name DrawStyle:
    /// Returns the current drawing style information from the state.
    /// @{
    SoDrawStyle::Style  getDrawStyle() const;
    uint16_t getLinePattern() const;
    float getLineWidth() const;
    float getPointSize() const;
    /// @}

    /// \name Font:
    /// Returns the current font information from the state.
    /// @{
    const SbName & getFontName() const;
    float getFontSize() const;
    /// @}

    /// \name Lighting:
    /// Returns the current lighting model information from the state.
    /// @{
    SoLightModel::Model getLightModel() const;
    const SbVec3f & getLightAttenuation() const;
    /// @}

    /// \name Material:
    /// Returns the current material information from the state.
    /// @{
    void getMaterial(SbColor &ambient,
                     SbColor &diffuse,
                     SbColor &specular,
                     SbColor &emission,
                     float   &shininess,
                     float   &transparency,
                     int     mtlIndex = 0) const; ///< Providing a \a mtlIndex will return the material defined for that index.
    SoMaterialBinding::Binding getMaterialBinding() const;
    /// @}

    /// \name Normals:
    /// Returns the current normal information from the state.
    /// @{
    int32_t getNumNormals() const;
    const SbVec3f & getNormal(int index) const;
    SoNormalBinding::Binding getNormalBinding() const;
    /// @}

    /// \name Profiles:
    /// Returns the current profiles and their coordinates from the state.
    /// @{
    int32_t getNumProfileCoordinates() const;
    const SbVec2f & getProfileCoordinate2(int index) const;
    const SbVec3f & getProfileCoordinate3(int index) const;
    const SoNodeList & getProfile() const;
    /// @}

    /// \name Shape Hints:
    /// Returns the current shape hints from the state.
    /// @{
    SoShapeHints::VertexOrdering getVertexOrdering() const;
    SoShapeHints::ShapeType getShapeType() const;
    SoShapeHints::FaceType getFaceType() const;
    float getCreaseAngle() const;
    /// @}

    /// \name Textures:
    /// Returns texture information from the state.
    /// @{
    int32_t getNumTextureCoordinates() const; ///< Returns 0 if texture coordinates are generated by a function
    const SbVec2f & getTextureCoordinate2(int index) const;
    const SbVec4f & getTextureCoordinate4(int index) const;
    SoTextureCoordinateBinding::Binding getTextureCoordinateBinding() const;
    const SbColor & getTextureBlendColor() const;
    const unsigned char * getTextureImage(SbVec2s &size, int &numComps) const; ///< Returns NULL if no texture is enabled

    /// Returns the current texture mapping information from the state.
    const SbMatrix & getTextureMatrix() const;
    SoTexture2::Model getTextureModel() const;
    SoTexture2::Wrap getTextureWrapS() const;
    SoTexture2::Wrap getTextureWrapT() const;
    /// @}

    /// \name Transformations:
    /// Returns the current modeling transformation and the current units from the state.
    /// @{
    const SbMatrix & getModelMatrix() const;
    SoUnits::Units getUnits() const;
    /// @}

    /// \name Viewing:
    /// Returns the current camera and viewing information from the state.
    /// @{
    float getFocalDistance() const;
    const SbMatrix & getProjectionMatrix() const;
    const SbMatrix & getViewingMatrix() const;
    const SbViewVolume & getViewVolume() const;
    /// @}

    /// \name Miscellaneous:
    /// @{
    SoPickStyle::Style getPickStyle() const; ///< Returns the current picking style.
    int32_t getSwitch() const;  ///< Returns the current switch value.
    /// @}

SoEXTENDER public:
    /// Returns the current response
    Response getCurrentResponse() const {
        return response;
    }

    /// Call the pre-callbacks and post-callbacks
    void invokePreCallbacks(const SoNode *node);
    void invokePostCallbacks(const SoNode *node);

    /// Call the primitive callbacks
    void invokeTriangleCallbacks(const SoShape *shape,
                                 const SoPrimitiveVertex *v1,
                                 const SoPrimitiveVertex *v2,
                                 const SoPrimitiveVertex *v3);
    void invokeLineSegmentCallbacks(const SoShape *shape,
                                    const SoPrimitiveVertex *v1,
                                    const SoPrimitiveVertex *v2);
    void invokePointCallbacks(const SoShape *shape,
                              const SoPrimitiveVertex *v);

    /// Should primitives be generated for this shape?
    SbBool shouldGeneratePrimitives(const SoShape *shape) const;

SoINTERNAL public:
    static void initClass();

    /// Returns the node that caused the callback:
    virtual SoNode * getCurPathTail();

    /// Set the current node during traversal:
    void setCurrentNode(SoNode *node) {
        currentNode = node;
    }

protected:
    /// Initiates action on graph
    virtual void beginTraversal(SoNode *node);

private:
    // Callback lists
    SbPList  preCallbackList;
    SbPList  postCallbackList;

    SbPList  preTailCallbackList;
    SbPList  postTailCallbackList;

    SbPList  triangleCallbackList;
    SbPList  lineSegmentCallbackList;
    SbPList  pointCallbackList;

    // Response from last callback
    Response  response;

    // Node where the callback occurs:
    SoNode *  currentNode;
};

#endif /* _SO_CALLBACK_ACTION_ */
