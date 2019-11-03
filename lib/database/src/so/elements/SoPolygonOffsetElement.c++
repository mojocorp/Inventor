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
 |	SoPolygonOffsetElement
 |
 |   Author(s)		: Morgan Leborgne
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */
#include <Inventor/elements/SoPolygonOffsetElement.h>

SO_ELEMENT_SOURCE(SoPolygonOffsetElement);

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Destructor.
//
// Use: private

SoPolygonOffsetElement::~SoPolygonOffsetElement()
//
////////////////////////////////////////////////////////////////////////
{
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Initializes SoPolygonOffsetElement class.
//
// Use: internal

void
SoPolygonOffsetElement::initClass()
//
////////////////////////////////////////////////////////////////////////
{
   SO_ELEMENT_INIT_CLASS(SoPolygonOffsetElement, SoReplacedElement);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Initializes element
//
// Use: public

void
SoPolygonOffsetElement::init(SoState * /*state*/)
//
////////////////////////////////////////////////////////////////////////
{
   SoPolygonOffsetElement::getDefault(offsetfactor, offsetunits, style, active);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Set element's value.
//
// Use: public, static

void
SoPolygonOffsetElement::set(SoState *state, SoNode *node, float factor, float units, Style styles, SbBool on)
//
////////////////////////////////////////////////////////////////////////
{
   SoPolygonOffsetElement *elem = (SoPolygonOffsetElement *)SoReplacedElement::getElement(state, classStackIndex, node);
   
   if(elem != NULL)
      elem->setElt(factor, units, styles, on);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Returns element values from state
//
// Use: public

void
SoPolygonOffsetElement::get(SoState *state, float &factor, float &units, Style &styles, SbBool &on)
//
////////////////////////////////////////////////////////////////////////
{
   const SoPolygonOffsetElement *elem= (const SoPolygonOffsetElement *)SoElement::getConstElement(state, classStackIndex);

   factor = elem->offsetfactor;
   units  = elem->offsetunits;
   styles = elem->style;
   on     = elem->active;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Sets polygon offset in element to given values.
//
// Use: protected, virtual

void
SoPolygonOffsetElement::setElt(float _factor, float _units, Style _styles, SbBool _on)
//
////////////////////////////////////////////////////////////////////////
{
   offsetfactor = _factor;
   offsetunits  = _units;
   style        = _styles;
   active       = _on;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Returns element default values
//
// Use: public

void
SoPolygonOffsetElement::getDefault(float &_factor, float &_units, Style &_styles, SbBool &_on)
//
////////////////////////////////////////////////////////////////////////
{
   _factor = 0.0f;
   _units  = 0.0f;
   _styles = SoPolygonOffsetElement::FILLED;
   _on     = FALSE;
}
