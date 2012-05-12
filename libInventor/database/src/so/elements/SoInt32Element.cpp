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
 |	SoInt32Element
 |
 |   Author(s)		: Gavin Bell
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

#include <Inventor/elements/SoInt32Element.h>

SO_ELEMENT_ABSTRACT_SOURCE(SoInt32Element);

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Destructor.
//
// Use: private

SoInt32Element::~SoInt32Element()
//
////////////////////////////////////////////////////////////////////////
{
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Initializes SoInt32Element class.
//
// Use: internal

void
SoInt32Element::initClass()
{
    SO_ELEMENT_INIT_ABSTRACT_CLASS(SoInt32Element, SoElement);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Overrides this method to return TRUE if the two
//    elements match.
//
// Use: public

SbBool
SoInt32Element::matches(const SoElement *elt) const
//
////////////////////////////////////////////////////////////////////////
{
    return (data == ((const SoInt32Element *) elt)->data);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Set element's value.
//
// Use: public, static

void
SoInt32Element::set(int stackIndex, SoState *state, int32_t value)
//
////////////////////////////////////////////////////////////////////////
{
    SoInt32Element	*elt;

    // Get an instance we can change (pushing if necessary)
    elt = (SoInt32Element *) getElement(state, stackIndex);

    if (elt != NULL)
	elt->setElt(value);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Really set the element.
//
// Use: protected, virtual

void
SoInt32Element::setElt(int32_t value)
//
////////////////////////////////////////////////////////////////////////
{
    data = value;
}


////////////////////////////////////////////////////////////////////////
//
// Description:
//     Create a copy of this instance suitable for calling matches()
//     on.
//
// Use: protected

SoElement *
SoInt32Element::copyMatchInfo() const
//
////////////////////////////////////////////////////////////////////////
{
    SoInt32Element *result =
	(SoInt32Element *)getTypeId().createInstance();

    result->data = data;

    return result;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Prints element for debugging.
//
// Use: public
//
////////////////////////////////////////////////////////////////////////

#ifdef DEBUG
void
SoInt32Element::print(FILE *fp) const
{
    SoElement::print(fp);

    fprintf(fp, "%s = %d\n", getTypeId().getName().getString(),
	    data);
}
#else  /* DEBUG */
void
SoInt32Element::print(FILE *) const
{
}
#endif /* DEBUG */
