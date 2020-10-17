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
 |	This file defines the SoTransform node class.
 |
 |   Author(s)		: Paul S. Strauss, Gavin Bell
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

#pragma once

#include <Inventor/fields/SoSFRotation.h>
#include <Inventor/fields/SoSFVec3f.h>
#include <Inventor/nodes/SoTransformation.h>

/// General 3D geometric transformation node.
/// \ingroup Nodes
/// This node defines a geometric 3D transformation consisting of (in
/// order) a (possibly) non-uniform scale about an arbitrary point, a
/// rotation about an arbitrary point and axis, and a translation. (While
/// the transformations can be thought of as being applied in that order,
/// matrices are actually premultiplied in the opposite order. Therefore,
/// the operations are listed in the reverse order throughout this reference
/// page.)
///
/// \par Action behavior:
/// <b>SoGLRenderAction, SoCallbackAction, SoGetBoundingBoxAction,
/// SoRayPickAction</b> Accumulates transformation into the current
/// transformation. <b>SoGetMatrixAction</b> Returns the matrix corresponding to
/// the total transformation.
///
/// \par File format/defaults:
/// \code
/// SoTransform {
///    translation          0 0 0
///    rotation             0 0 1  0
///    scaleFactor          1 1 1
///    scaleOrientation     0 0 1  0
///    center               0 0 0
/// }
/// \endcode
/// \sa
/// SoMatrixTransform,SoResetTransform,SoRotation,SoRotationXYZ,SoScale,SoTransformManip,SoTransformSeparator,SoTranslation
class INVENTOR_API SoTransform : public SoTransformation {

    SO_NODE_HEADER(SoTransform);

  public:
    // Fields
    SoSFVec3f    translation;      ///< Translation vector.
    SoSFRotation rotation;         ///< Rotation specification.
    SoSFVec3f    scaleFactor;      ///< Scale factors.
    SoSFRotation scaleOrientation; ///< Rotational orientation for scale.
    SoSFVec3f    center;           ///< Origin for scale and rotation.

    /// Creates a transformation node with default settings.
    SoTransform();

    /// Sets the transform to translate the origin to the \a fromPoint and
    /// rotate the negative z-axis (0,0,-1) to lie on the vector from
    /// \a fromPoint to \a toPoint. This always tries to keep the "up"
    /// direction the positive y-axis, unless that is impossible.
    /// All current info in the node is wiped out.
    void pointAt(const SbVec3f &fromPoint, const SbVec3f &toPoint);

    /// These return composite matrices that transform from object space to each
    /// of the spaces after the scale, rotation, or translation.
    void getScaleSpaceMatrix(SbMatrix &mat, SbMatrix &inv) const;
    void getRotationSpaceMatrix(SbMatrix &mat, SbMatrix &inv) const;
    void getTranslationSpaceMatrix(SbMatrix &mat, SbMatrix &inv) const;

    /// These are convenience functions that combine the effects of a matrix
    /// transformation into the current transformation stored in this node.
    /// The first method premultiplies the transformation and the second
    /// postmultiplies it.
    void multLeft(const SbMatrix &mat);
    void multRight(const SbMatrix &mat);

    /// These are convenience functions that combine the effects of another
    /// transformation node into the current transformation stored in this
    /// node. The first method premultiplies the transformation and the
    /// second postmultiplies it.
    void combineLeft(SoTransformation *nodeOnLeft);
    void combineRight(SoTransformation *nodeOnRight);

    /// Sets the fields in the node to implement the transformation
    /// represented by the given matrix. Note that invalid matrices (such as
    /// singular ones) have undefined results.
    void setMatrix(const SbMatrix &mat);

    /// Changes the center of the transformation to the given point without
    /// affecting the overall effect of the transformation.
    void recenter(const SbVec3f &newCenter);

    SoEXTENDER
  public:
    virtual void doAction(SoAction *action);
    virtual void callback(SoCallbackAction *action);
    virtual void GLRender(SoGLRenderAction *action);
    virtual void getBoundingBox(SoGetBoundingBoxAction *action);
    virtual void getMatrix(SoGetMatrixAction *action);
    virtual void pick(SoPickAction *action);

    SoINTERNAL
  public:
    static void initClass();

  protected:
    virtual ~SoTransform();
};

