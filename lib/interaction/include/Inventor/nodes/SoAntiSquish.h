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
 |	This file defines the SoAntiSquish node class.
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
#include <Inventor/fields/SoSFBool.h>
#include <Inventor/fields/SoSFEnum.h>

/// Transformation node that undoes non-uniform 3D scales.
/// \ingroup Nodes
/// This node removes nonuniform 3D scaling from the current transformation
/// matrix when traversed by an action.   It is used by draggers such as the
/// <tt>SoTrackballDragger</tt> that need to stay uniformly scaled no matter
/// where they are located in the scene graph.
///
///
/// The magnitude of the new scale is determined
/// by the current transformation matrix and the \a sizing field.
/// This node does not change the translation or rotation in the matrix.
///
/// \par Action behavior:
/// <b>SoGLRenderAction,SoCallbackAction,SoGetBoundingBoxAction,
/// SoGetMatrixAction, SoRayPickAction</b> Appends the current transformation
/// with a new matrix to create an unsquished result.
///
/// \par File format/defaults:
/// \code
/// SoAntiSquish {
///    sizing           AVERAGE_DIMENSION
///    recalcAlways     TRUE
/// }
/// \endcode
/// \sa SoCenterballDragger, SoJackDragger, SoTrackballDragger,
/// SoTransformerDragger,SoTransformation, SoTransformBoxDragger
class SoAntiSquish : public SoTransformation {

    SO_NODE_HEADER(SoAntiSquish);

  public:
    /// Creates an anti-squish node with default settings.
    SoAntiSquish();

    enum Sizing {
        X, ///< fits the other two axes to match the X axis, whose size is
           ///< unchanged
        Y, ///< fits the other two axes to match the Y axis, whose size is
           ///< unchanged
        Z, ///< fits the other two axes to match the Z axis, whose size is
           ///< unchanged
        AVERAGE_DIMENSION,  ///< uses average of 3 scales in the matrix
        BIGGEST_DIMENSION,  ///< uses biggest of 3 scales in the matrix
        SMALLEST_DIMENSION, ///< uses smallest of 3 scales in the matrix
        LONGEST_DIAGONAL    ///< accounts for shearing; transforms a cube by the
                            ///< matrix and then uses length of longest diagonal
    };

    /// Determines which of the algorithms enumerated by the type #Sizing
    /// will be used to select the new scale when the x,y, and z scales are
    /// not equal.
    SoSFEnum sizing;

    /// If recalcAlways is <b>TRUE</b>, this node calculates its unsquishing
    /// matrix every time it is traversed.  If <b>FALSE</b>, then this only
    /// occurs during the first traversal folllowing a call to recalc().
    SoSFBool recalcAlways;

    /// Sets a flag so that the next time the node is traversed, it will
    /// recalculate its unsquishing matrix.
    void recalc();

    SoEXTENDER
  public:
    // Called by actions that need to change the state with a new matrix.
    void doAction(SoAction *action);

    SoINTERNAL
  public:
    static void initClass();

  protected:
    virtual void callback(SoCallbackAction *action);
    virtual void GLRender(SoGLRenderAction *action);
    virtual void getBoundingBox(SoGetBoundingBoxAction *action);
    virtual void getMatrix(SoGetMatrixAction *action);
    virtual void pick(SoPickAction *action);

    virtual ~SoAntiSquish();

    SbMatrix getUnsquishingMatrix(SbMatrix squishedMatrix, SbBool doInverse,
                                  SbMatrix &inverseAnswer);

  private:
    SbMatrix savedAnswer, savedInverseAnswer;

    SbBool recalcNextTime;
};
