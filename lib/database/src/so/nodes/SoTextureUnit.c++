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
 |      SoTextureUnit
 |
 |   Author(s)          : Morgan Leborgne
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

#include <Inventor/nodes/SoTextureUnit.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/actions/SoPickAction.h>
#include <Inventor/elements/SoTextureUnitElement.h>
#include <Inventor/elements/SoGLTextureUnitElement.h>

SO_NODE_SOURCE(SoTextureUnit);

////////////////////////////////////////////////////////////////////////
//
// Constructor
//
SoTextureUnit::SoTextureUnit() {
    // Do standard constructor stuff
    SO_NODE_CONSTRUCTOR(SoTextureUnit);

    // Setup fields
    SO_NODE_ADD_FIELD(unit, (0));

    isBuiltIn = TRUE;
}

/////////////////////////////////////////////////////////////////
//
// This initializes the SoDecal class.
//
void
SoTextureUnit::initClass() {
    // Initialize type id variables
    SO__NODE_INIT_CLASS(SoTextureUnit, "TextureUnit", SoNode);

    SO_ENABLE(SoGLRenderAction, SoGLTextureUnitElement);
    SO_ENABLE(SoCallbackAction, SoTextureUnitElement);
    SO_ENABLE(SoPickAction, SoTextureUnitElement);
}

////////////////////////////////////////////////////////////////////////
//
// Destructor
//
SoTextureUnit::~SoTextureUnit() {}

uint32_t
SoTextureUnit::getMaxTextureUnit(SoState *state) {
    return SoTextureUnitElement::getMaxTextureUnit(state);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Implements all actions.
//
// Use: extender

void
SoTextureUnit::doAction(SoAction *action)
//
////////////////////////////////////////////////////////////////////////
{
    SoTextureUnitElement::set(action->getState(), unit.getValue());
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Does callback action thing.
//
// Use: extender

void
SoTextureUnit::callback(SoCallbackAction *action)
//
////////////////////////////////////////////////////////////////////////
{
    SoTextureUnit::doAction(action);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Does GL render action.
//
// Use: extender

void
SoTextureUnit::GLRender(SoGLRenderAction *action)
//
////////////////////////////////////////////////////////////////////////
{
    SoTextureUnit::doAction(action);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Does pick action.
//
// Use: extender

void
SoTextureUnit::pick(SoPickAction *action)
//
////////////////////////////////////////////////////////////////////////
{
    SoTextureUnit::doAction(action);
}
