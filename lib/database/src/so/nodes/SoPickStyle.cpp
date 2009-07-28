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

/*
 * Copyright (C) 1990,91   Silicon Graphics, Inc.
 *
 _______________________________________________________________________
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 |
 |   $Revision: 1.1 $
 |
 |   Classes:
 |      SoPickStyle
 |
 |   Author(s)          : Paul S. Strauss
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/actions/SoRayPickAction.h>
#include <Inventor/nodes/SoPickStyle.h>
#include <Inventor/elements/SoOverrideElement.h>

SO_NODE_SOURCE(SoPickStyle);

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Constructor
//
// Use: public

SoPickStyle::SoPickStyle()
//
////////////////////////////////////////////////////////////////////////
{
    SO_NODE_CONSTRUCTOR(SoPickStyle);
    SO_NODE_ADD_FIELD(style, (SoPickStyleElement::getDefault()));

    // Set up static info for enumerated type field
    SO_NODE_DEFINE_ENUM_VALUE(Style, SHAPE);
    SO_NODE_DEFINE_ENUM_VALUE(Style, BOUNDING_BOX);
    SO_NODE_DEFINE_ENUM_VALUE(Style, UNPICKABLE);

    // Set up info in enumerated type field
    SO_NODE_SET_SF_ENUM_TYPE(style, Style);

    isBuiltIn = TRUE;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Destructor
//
// Use: private

SoPickStyle::~SoPickStyle()
//
////////////////////////////////////////////////////////////////////////
{
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    This initializes the SoPickStyle class.
//
// Use: internal

void
SoPickStyle::initClass()
//
////////////////////////////////////////////////////////////////////////
{
    SO__NODE_INIT_CLASS(SoPickStyle, "PickStyle", SoNode);

    // Enable elements for picking actions:
    SO_ENABLE(SoCallbackAction, SoPickStyleElement);
    SO_ENABLE(SoPickAction,	SoPickStyleElement);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Handles most actions.
//
// Use: extender

void
SoPickStyle::doAction(SoAction *action)
//
////////////////////////////////////////////////////////////////////////
{
    SoState *state = action->getState();

    if (! style.isIgnored()
	&& ! SoOverrideElement::getPickStyleOverride(state)) {
	if (isOverride()) {
	    SoOverrideElement::setPickStyleOverride(state, this, TRUE);
	}
	SoPickStyleElement::set(state,
				(SoPickStyleElement::Style) style.getValue());
    }
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Handles callback action.
//
// Use: extender

void
SoPickStyle::callback(SoCallbackAction *action)
//
////////////////////////////////////////////////////////////////////////
{
    SoPickStyle::doAction(action);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Handles pick action.
//
// Use: extender

void
SoPickStyle::pick(SoPickAction *action)
//
////////////////////////////////////////////////////////////////////////
{
    SoPickStyle::doAction(action);
}
