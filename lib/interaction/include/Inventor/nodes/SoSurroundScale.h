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
 |	This file defines the SoSurroundScale node class.
 |
 |   Author(s)		: Paul Isaacs
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

#pragma once

#include <Inventor/SbMatrix.h>
#include <Inventor/fields/SoFieldData.h>
#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/nodes/SoTransformation.h>
#include <Inventor/fields/SoSFInt32.h>

class SoFullPath;

/// Transformation node that adjusts the current matrix so a default cube will
/// surround other objects.
/// \ingroup Nodes
/// When traversed by an action, this
/// node appends a transformation to the current transformation matrix so that a
/// default size  cube will surround the objects specified by its fields.
/// Transform manipulators, such as <tt>SoHandleBoxManip</tt>, use these nodes
/// to make themselves surround other objects.
///
/// This node only recalculates after the
/// #invalidate() method has been
/// called. Otherwise it uses a saved scale and translation.
///
/// When calculating what to surround, the <tt>SoSurroundScale</tt>
/// looks at the current path in
/// the action and at its own field values.
/// Then <tt>SoSurroundScale</tt> applies an <tt>SoGetBoundingBoxAction</tt>
/// to the node that is
/// #numNodesUpToContainer nodes above it on the path.
/// <tt>SoSurroundScale</tt> also tells the action to reset
/// the bounding box upon traversal of the
/// node located #numNodesUpToReset nodes above it in the path.
/// The <tt>SoSurroundScale</tt> then appends a translation and scale
/// to the current transformation
/// so that a default size <tt>SoCube</tt> will translate and scale to fit this
/// bounding box.
///
/// For example, when an <tt>SoHandleBoxManip</tt> wants to surround the objects
/// it is going to move, the scene graph will look something like this: \code
///                  RootNode
///        -------------------------
///        |                        |
///      handleBoxManip        movingStuff
///        |
///      handleBoxDragger
///        |
///      separator
///       -----------------------------------
///       |            |                     |
///     motionMatrix  surroundScale      cubeGeom
/// \endcode
/// The <tt>SoHandleBoxDragger</tt> wants to transform the <em>cubeGeom</em> so
/// that it surrounds the <em>movingStuff</em>. So it sets the
/// <em>surroundScale</em> fields to: \code
///      #numNodesUpToContainer = 4;
///      #numNodesUpToReset = 3;
/// \endcode
/// The <tt>SoBoundingBoxAction</tt> will then be applied to <em>RootNode</em>,
/// with a reset after traversing the <tt>SoHandleBoxManip</tt>.
/// So the <tt>SoSurroundScale</tt> will surround the objects below
/// <em>separator</em>, and to the right of <em>handleBoxManip</em>, producing
/// the desired effect.
///
/// \par Action behavior:
/// <b>SoGLRenderAction, SoCallbackAction, SoGetBoundingBoxAction,
/// SoRayPickAction</b> Accumulates scaling and translation transformations into
/// the current transformation.
/// <b>SoGetMatrixAction</b>
/// Returns the matrix corresponding to the scaling and translation.
///
/// \par File format/defaults:
/// \code
/// SoSurroundScale {
///    numNodesUpToContainer	0
///    numNodesUpToReset	0
/// }
/// \endcode
/// \sa SoTransformation, SoTransformManip,SoCenterballDragger,
/// SoCenterballManip,SoHandleBoxDragger, \sa SoHandleBoxManip,SoJackDragger,
/// SoJackManip,SoTabBoxDragger, SoTabBoxManip,SoTrackballDragger, \sa
/// SoTrackballManip,SoTransformBoxDragger, SoTransformBoxManip
class INVENTOR_API SoSurroundScale : public SoTransformation {

    SO_NODE_HEADER(SoSurroundScale);

  public:
    /// Creates a surround scale node with default settings.
    SoSurroundScale();

    /// When traversed by an action, if surroundScale needs to
    /// calculate a new box, surroundScale
    /// looks at the current path in
    /// the action. It travels up this path a distance of
    /// #numNodesUpToContainer and  applies an SoGetBoundingBoxAction to the
    /// node that it finds there.
    SoSFInt32 numNodesUpToContainer;

    /// Before applying the SoGetBoundingBoxAction (see the
    /// #numNodesUpToContainer field aove) the surroundScale node
    /// travels up the path a distance of #numNodesUpToReset
    /// and tells the action to reset
    /// the bounding box upon traversal of that node.
    SoSFInt32 numNodesUpToReset;

    /// If you call this, then next time an action is applied the node will
    /// re-calculate it's cached translation and scale values.
    void invalidate();

    SoEXTENDER
  public:
    // Called by actions that need to change the state with a new matrix.
    void doAction(SoAction *action);

    void   setDoingTranslations(SbBool doEm) { doTranslations = doEm; }
    SbBool isDoingTranslations() { return doTranslations; }

    SoINTERNAL
  public:
    static void initClass();

  protected:
    virtual void callback(SoCallbackAction *action);
    virtual void GLRender(SoGLRenderAction *action);
    virtual void getBoundingBox(SoGetBoundingBoxAction *action);
    virtual void getMatrix(SoGetMatrixAction *action);
    virtual void pick(SoPickAction *action);

    void updateMySurroundParams(SoAction *action, const SbMatrix &inv);

    void   setIgnoreInBbox(SbBool newVal) { ignoreInBbox = newVal; }
    SbBool isIgnoreInBbox() { return ignoreInBbox; }

    SbVec3f cachedScale;
    SbVec3f cachedInvScale;
    SbVec3f cachedTranslation;
    SbBool  cacheOK;

    SbBool doTranslations;

    virtual ~SoSurroundScale();

  private:
    SbBool ignoreInBbox;
};

