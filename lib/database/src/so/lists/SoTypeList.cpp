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
 |   $Revision: 1.2 $
 |
 |   Classes:
 |	SoTypeList
 |
 |   Author(s)		: Nick Thompson
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

#include <Inventor/lists/SoTypeList.h>

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Add a typeId to the end of the list
//
// Use: public

void
SoTypeList::append(SoType typeId)	// typeId to append
//
////////////////////////////////////////////////////////////////////////
{
    // we have to do some hackage to cast an SoType into a void *...
    void *hackage = (void*)(unsigned long)(*(int32_t *)&typeId);
    SbPList::append(hackage);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Returns index of given typeId in list, or -1 if not found.
//
// Use: public

int
SoTypeList::find(SoType typeId) const
//
////////////////////////////////////////////////////////////////////////
{
    // we have to do some hackage to cast an SoType into a void *...
    void *hackage = (void*)(unsigned long)(*(int32_t *)&typeId);
    return SbPList::find(hackage);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Insert given typeId into list before typeId with given index
//
// Use: public

void
SoTypeList::insert(SoType typeId,	// typeId to insert
                   int addBefore)	// index to add before
//
////////////////////////////////////////////////////////////////////////
{
    // we have to do some hackage to cast an SoType into a void *...
    void *hackage = (void*)(unsigned long)(*(int32_t *)&typeId);
    SbPList::insert(hackage, addBefore);
}


////////////////////////////////////////////////////////////////////////
//
// Description:
//    Access an element of a list
//
// Use: public

SoType
SoTypeList::operator [](int i) const
//
////////////////////////////////////////////////////////////////////////
{
    // we have to do some hackage to cast our void * back to an SoType...
    int32_t hackage = (long)(* (const SbPList *) this)[i];
    return *(SoType*)&hackage;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Set an element of a list
//
// Use: public

void
SoTypeList::set(int i,			// index to set
                SoType typeId)		// new typeId value
//
////////////////////////////////////////////////////////////////////////
{
    // we have to do some hackage to cast an SoType into a void *...
    void *hackage = (void*)(unsigned long)(*(int32_t *)&typeId);
    (*(const SbPList *) this) [i] = hackage;
}
