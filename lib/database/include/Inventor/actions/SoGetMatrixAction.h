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
 |	Defines the SoGetMatrixAction class
 |
 |   Author(s)		: Paul S. Strauss
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

#pragma once

#include <Inventor/SbMatrix.h>
#include <Inventor/actions/SoSubAction.h>
#include <Inventor/SbViewportRegion.h>

/// Computes transformation matrix for subgraph.
/// \ingroup Actions
/// This action computes transformation matrices for a given subgraph.  It
/// computes the cumulative transformation matrix and its inverse, along
/// with a cumulative texture transformation matrix and its inverse.
///
///
/// This action is unlike most others in that it does not traverse
/// downwards from groups. When applied to a node, it computes the matrix
/// for just that node. (This makes sense for transformation nodes, but
/// not for others, really.) It is much more useful when applied to a
/// path. When applied to a path, it gathers the transformation info for
/// all nodes in the path and those that affect nodes in the path, but it
/// stops when it hits the last node in the path; it does not traverse
/// downwards from it as other actions (such as rendering) do. This
/// behavior makes the most sense for this action.
/// \sa SoGetBoundingBoxAction
class SoGetMatrixAction : public SoAction {

    SO_ACTION_HEADER(SoGetMatrixAction);

  public:
    /// Constructor takes viewport region to use for picking. Even
    /// though the matrix computation may not involve a window
    /// per se, some nodes need this information to determine their
    /// placement.
    SoGetMatrixAction(const SbViewportRegion &newRegion);

    /// Destructor
    virtual ~SoGetMatrixAction();

    /// Sets current viewport region to use for action
    void setViewportRegion(const SbViewportRegion &newRegion);

    /// Returns current viewport region
    const SbViewportRegion &getViewportRegion() const { return vpRegion; }

    /// Returns cumulative transformation matrix.
    /// Warning: the matrices returned by these routines should not be changed
    /// (unless you are implementing your own transformation nodes).
    SbMatrix &getMatrix() { return ctm; }

    /// Returns cumulative inverse transformation matrix.
    /// Warning: the matrices returned by these routines should not be changed
    /// (unless you are implementing your own transformation nodes).
    SbMatrix &getInverse() { return inv; }

    /// Returns cumulative texture transformation matrix.
    /// Warning: the matrices returned by these routines should not be changed
    /// (unless you are implementing your own transformation nodes).
    SbMatrix &getTextureMatrix() { return texCtm; }

    /// Returns cumulative texture inverse transformation matrix.
    /// Warning: the matrices returned by these routines should not be changed
    /// (unless you are implementing your own transformation nodes).
    SbMatrix &getTextureInverse() { return texInv; }

    SoINTERNAL
  public:
    static void initClass();

  protected:
    // Initiates action on graph
    virtual void beginTraversal(SoNode *node);

  private:
    SbMatrix         ctm;      // Cumulative transformation matrix
    SbMatrix         inv;      // Inverse transformation matrix
    SbMatrix         texCtm;   // Cumulative texture transf matrix
    SbMatrix         texInv;   // Inverse texture transf matrix
    SbViewportRegion vpRegion; // Current viewport region
};

