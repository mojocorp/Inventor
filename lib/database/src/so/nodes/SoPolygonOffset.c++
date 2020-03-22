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
 |   $Revision $
 |
 |   Classes:
 |      SoPolygonOffset
 |
 |   Author(s)          : Morgan Leborgne
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */
#include <Inventor/nodes/SoPolygonOffset.h>
#include <Inventor/elements/SoOverrideElement.h>
#include <Inventor/elements/SoGLPolygonOffsetElement.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoCallbackAction.h>

SO_NODE_SOURCE(SoPolygonOffset);

SoPolygonOffset::SoPolygonOffset()
{
   SO_NODE_CONSTRUCTOR(SoPolygonOffset);

   SO_NODE_ADD_FIELD(factor, (1.f));
   SO_NODE_ADD_FIELD(units,  (1.f));
   SO_NODE_ADD_FIELD(styles, (FILLED));
   SO_NODE_ADD_FIELD(on,     (TRUE));

   SO_NODE_DEFINE_ENUM_VALUE(Style, FILLED);
   SO_NODE_DEFINE_ENUM_VALUE(Style, LINES);
   SO_NODE_DEFINE_ENUM_VALUE(Style, POINTS);

   SO_NODE_SET_SF_ENUM_TYPE(styles, Style);

   isBuiltIn = TRUE;
}

SoPolygonOffset::~SoPolygonOffset()
{
}

void
SoPolygonOffset::initClass()
{
   SO__NODE_INIT_CLASS(SoPolygonOffset, "PolygonOffset", SoNode);

   SO_ENABLE(SoGLRenderAction, SoGLPolygonOffsetElement);
   SO_ENABLE(SoGLRenderAction, SoOverrideElement);
   
   SO_ENABLE(SoCallbackAction, SoPolygonOffsetElement);
   SO_ENABLE(SoCallbackAction, SoOverrideElement);
}

void
SoPolygonOffset::doAction(SoAction * action)
{
   SoState *state= action->getState();

   if(SoOverrideElement::getPolygonOffsetOverride(state))
      return;

   SoPolygonOffsetElement::set(state, this, factor.getValue(), units.getValue(),
                               (SoPolygonOffsetElement::Style)styles.getValue(),
                               on.getValue());

   if(isOverride())
      SoOverrideElement::setPolygonOffsetOverride(state, this, TRUE);
}

void
SoPolygonOffset::GLRender(SoGLRenderAction * action)
{
   SoPolygonOffset::doAction((SoAction *)action);
}

void
SoPolygonOffset::callback(SoCallbackAction * action)
{
   SoPolygonOffset::doAction((SoAction *)action);
}
