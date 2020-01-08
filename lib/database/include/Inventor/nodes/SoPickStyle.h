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
 |   $Revision $
 |
 |   Description:
 |	This file defines the SoPickStyle node class.
 |
 |   Author(s)		: Paul S. Strauss
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

#ifndef _SO_PICK_STYLE_
#define _SO_PICK_STYLE_

#include <Inventor/SbString.h>
#include <Inventor/elements/SoPickStyleElement.h>
#include <Inventor/fields/SoSFEnum.h>
#include <Inventor/nodes/SoSubNode.h>

/// Picking style node.
/// \ingroup Nodes
/// This node determines how subsequent geometry nodes in the scene graph
/// are to be picked, as indicated by the #style field.
///
/// Note that this is the only way to change the pick behavior of shapes;
/// drawing style, complexity, and other rendering-related properties have
/// no effect on picking.
///
/// \par Action behavior:
/// <b>SoRayPickAction, SoCallbackAction</b>
/// Sets the current pick style in the state.
///
/// \par File format/defaults:
/// \code
/// SoPickStyle {
///    style	SHAPE
/// }
/// \endcode
/// \sa SoComplexity, SoDrawStyle, SoRayPickAction
class SoPickStyle : public SoNode {

    SO_NODE_HEADER(SoPickStyle);

  public:
    /// Picking style:
    enum Style {
        SHAPE = SoPickStyleElement::SHAPE, ///< Points on the surfaces of shapes
                                           ///< may be picked"
        BOUNDING_BOX =
            SoPickStyleElement::BOUNDING_BOX, ///< Points on the surfaces of 3D
                                              ///< bounding boxes of shapes may
                                              ///< be picked
        UNPICKABLE = SoPickStyleElement::UNPICKABLE ///< Subsequent objects are
                                                    ///< transparent to picks
    };

    // Fields
    SoSFEnum style; ///< Picking style.

    /// Creates a pick style node with default settings.
    SoPickStyle();

    SoEXTENDER
  public:
    virtual void doAction(SoAction *action);
    virtual void callback(SoCallbackAction *action);
    virtual void pick(SoPickAction *action);

    SoINTERNAL
  public:
    static void initClass();

  protected:
    virtual ~SoPickStyle();
};

#endif /* _SO_PICK_STYLE_ */
