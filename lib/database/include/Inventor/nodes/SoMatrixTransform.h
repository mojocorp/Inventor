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
 |	This file defines the SoMatrixTransform node class.
 |
 |   Author(s)		: Paul S. Strauss, Paul Isaacs
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

#pragma once

#include <Inventor/fields/SoSFMatrix.h>
#include <Inventor/nodes/SoTransformation.h>

/// Node that specifies a 3D geometric transformation as a matrix.
/// \ingroup Nodes
/// This node defines a geometric 3D transformation with a single
/// <tt>SbMatrix</tt>. Note that some matrices (such as singular ones) may
/// result in errors in bounding boxes, picking, and lighting.
///
/// \par Action behavior:
/// <b>SoGLRenderAction, SoCallbackAction, SoGetBoundingBoxAction,
/// SoRayPickAction</b> Concatenates matrix given in the #matrix field with the
/// current transformation matrix. <b>SoGetMatrixAction</b> Returns
/// transformation matrix specified in the #matrix field.
///
/// \par File format/defaults:
/// \code
/// SoMatrixTransform {
///    matrix   1 0 0 0
///             0 1 0 0
///             0 0 1 0
///             0 0 0 1
/// }
/// \endcode
/// \sa SoTransform, SoMultipleCopy
class INVENTOR_API SoMatrixTransform : public SoTransformation {

    SO_NODE_HEADER(SoMatrixTransform);

  public:
    // Fields
    SoSFMatrix matrix; ///< Transformation matrix.

    /// Creates a matrix transformation node with default settings.
    SoMatrixTransform();

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
    virtual ~SoMatrixTransform();
};

