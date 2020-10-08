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
 |	This file defines the SoResetTransform node class.
 |
 |   Author(s)		: Paul S. Strauss
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

#pragma once

#include <Inventor/fields/SoSFBitMask.h>
#include <Inventor/nodes/SoTransformation.h>

/// Node that resets the current transformation to identity.
/// \ingroup Nodes
/// This node resets the current transformation to identity. It can be
/// used to apply an absolute world space transformation afterwards, such
/// as translating to a specific point from within a hierarchy.
///
/// An <tt>SoResetTransform</tt> node should probably be used under an
/// <tt>SoSeparator</tt> or <tt>SoTransformSeparator</tt> so it won't change
/// transformations for the rest of the scene graph.
///
/// An <tt>SoResetTransform</tt> node can also be used to reset the current
/// bounding box to empty during traversal of an
/// <tt>SoGetBoundingBoxAction</tt>, if the #whatToReset field has the
/// #BBOX bit set.
///
/// \par Action behavior:
/// <b>SoGLRenderAction, SoCallbackAction, SoRayPickAction</b>
/// If specified, resets current transformation matrix to identity.
/// <b>SoGetBoundingBoxAction</b>
/// If specified, resets current transformation matrix to identity and
/// current computed bounding box to be empty.
/// <b>SoGetMatrixAction</b>
/// Returns identity matrix.
///
/// \par File format/defaults:
/// \code
/// SoResetTransform {
///    whatToReset	TRANSFORM
/// }
/// \endcode
/// \sa SoTransform
class SoResetTransform : public SoTransformation {

    SO_NODE_HEADER(SoResetTransform);

  public:
    /// Which things get reset:
    enum ResetType {
        TRANSFORM = 0x01, ///< Reset the current transformation to identity
        BBOX = 0x02       ///< Reset the bounding box to empty
    };

    // Fields
    SoSFBitMask whatToReset; ///< Specifies which items to reset when the node
                             ///< is traversed.

    /// Creates a reset transformation node with default settings.
    SoResetTransform();

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
    virtual ~SoResetTransform();
};

