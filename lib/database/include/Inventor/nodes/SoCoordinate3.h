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
 |	This file defines the SoCoordinate3 node class.
 |
 |   Author(s)		: Paul S. Strauss
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

#ifndef  _SO_COORDINATE3_
#define  _SO_COORDINATE3_

#include <Inventor/fields/SoMFVec3f.h>
#include <Inventor/nodes/SoSubNode.h>

/// Coordinate point node.
/// \ingroup Nodes
/// This node defines a set of 3D coordinates to be used by subsequent
/// vertex-based shape nodes (those derived from <tt>SoVertexShape</tt>) or
/// shape nodes that use them as control points (such as NURBS curves and
/// surfaces). This node does not produce a visible result during
/// rendering; it simply replaces the current coordinates in the rendering
/// state for subsequent nodes to use.
///
/// \par Action behavior:
/// <b>SoGLRenderAction, SoCallbackAction, SoGetBoundingBoxAction, SoRayPickAction</b>
/// Sets coordinates in current traversal state.
///
/// \par File format/defaults:
/// \code
/// SoCoordinate3 {
///    point	0 0 0
/// }
/// \endcode
/// \sa SoCoordinate4,SoVertexShape
class INVENTOR_API SoCoordinate3 : public SoNode {

    SO_NODE_HEADER(SoCoordinate3);

  public:
    // Fields
    SoMFVec3f		point;		///< Coordinate point(s)

    /// Constructor
    SoCoordinate3();

  SoEXTENDER public:
    virtual void	doAction(SoAction *action);
    virtual void	GLRender(SoGLRenderAction *action);
    virtual void	getBoundingBox(SoGetBoundingBoxAction *action);
    virtual void	callback(SoCallbackAction *action);
    virtual void	pick(SoPickAction *action);

  SoINTERNAL public:
    static void		initClass();

  protected:
    virtual ~SoCoordinate3();
};

#endif /* _SO_COORDINATE3_ */
