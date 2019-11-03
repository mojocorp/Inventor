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
 |	SoTransparencyTypeElement
 |
 |   Author(s)		: Morgan Leborgne
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

#include <Inventor/elements/SoTransparencyTypeElement.h>
#include <Inventor/elements/SoShapeStyleElement.h>
#include <Inventor/elements/SoTransparencyTypeElement.h>

SO_ELEMENT_SOURCE(SoTransparencyTypeElement);

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Destructor.
//
// Use: private

SoTransparencyTypeElement::~SoTransparencyTypeElement()
//
////////////////////////////////////////////////////////////////////////
{
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Initializes element
//
// Use: public

void
SoTransparencyTypeElement::init(SoState *)
//
////////////////////////////////////////////////////////////////////////
{
    data = getDefault();
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Initializes SoTransparencyTypeElement class.
//
// Use: internal
void
SoTransparencyTypeElement::initClass()
//
////////////////////////////////////////////////////////////////////////
{
    SO_ELEMENT_INIT_CLASS(SoTransparencyTypeElement, SoInt32Element);
}

///////////////////////////////////////////////////////////////////////
//
// Description
// set the current transparency in the state
//
void
SoTransparencyTypeElement::set(SoState *state, TransparencyType type)
{
    SoInt32Element::set(classStackIndex, state, (int32_t)type);

    // Set the transparency bit in the ShapeStyle element
    // and the lazy element.
    SoShapeStyleElement::setTransparencyType(state,type);
    SoLazyElement::setTransparencyType(state, type);
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
SoTransparencyTypeElement::print(FILE *fp) const
{
    SoElement::print(fp);

    switch(data)
    {
    case SCREEN_DOOR:         fprintf(fp, "\tTransparency type = SCREEN_DOOR\n"); break;
    case ADD:                 fprintf(fp, "\tTransparency type = ADD\n"); break;
    case DELAYED_ADD:         fprintf(fp, "\tTransparency type = DELAYED_ADD\n"); break;
    case SORTED_OBJECT_ADD:   fprintf(fp, "\tTransparency type = SORTED_OBJECT_ADD\n"); break;
    case BLEND:               fprintf(fp, "\tTransparency type = BLEND\n"); break;
    case DELAYED_BLEND:       fprintf(fp, "\tTransparency type = DELAYED_BLEND\n"); break;
    case SORTED_OBJECT_BLEND: fprintf(fp, "\tTransparency type = SORTED_OBJECT_BLEND\n"); break;
    default:                  fprintf(fp, "\tTransparency type = UNKNOWN\n"); break;
    }
}
#else  /* DEBUG */
void
SoTransparencyTypeElement::print(FILE *) const
{
}
#endif /* DEBUG */
