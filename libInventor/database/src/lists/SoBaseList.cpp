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
 |	SoBaseList
 |
 |   Author(s)		: Nick Thompson
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

#include <Inventor/lists/SoBaseList.h>
#include <Inventor/misc/SoBase.h>

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Default constructor.
//
// Use: public

SoBaseList::SoBaseList() : SbPList()
//
////////////////////////////////////////////////////////////////////////
{
    // By default, this adds references to things in the list when
    // they are added
    addRefs = TRUE;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Constructor that takes initial approximate size (number of items
//    in list).
//
// Use: public

SoBaseList::SoBaseList(int size) : SbPList(size)
//
////////////////////////////////////////////////////////////////////////
{
    // By default, this adds references to things in the list when
    // they are added
    addRefs = TRUE;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Constructor that takes another list to copy values from.
//
// Use: public

SoBaseList::SoBaseList(const SoBaseList &l) : SbPList()
//
////////////////////////////////////////////////////////////////////////
{
    // By default, this adds references to things in the list when
    // they are added
    addRefs = TRUE;

    copy(l);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Add a pointer to the end of the list
//
// Use: public

void
SoBaseList::append(SoBase * ptr)	// pointer to append
//
////////////////////////////////////////////////////////////////////////
{
    SbPList::append((void *) ptr);
    if (addRefs && ptr)
        ptr->ref();
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Insert given pointer into list before pointer with given index
//
// Use: public

void
SoBaseList::insert(SoBase *ptr,		// pointer to insert
                   int addBefore)	// index to add before
//
////////////////////////////////////////////////////////////////////////
{
    if (addRefs && ptr)
        ptr->ref();
    SbPList::insert((void *) ptr, addBefore);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Remove pointer with given index
//
// Use: public

void
SoBaseList::remove(int which)		// index of pointer to remove
//
////////////////////////////////////////////////////////////////////////
{
    if (addRefs && (*this)[which])
        (*this)[which]->unref();
    SbPList::remove(which);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Remove all pointers from one with given index on, inclusive
//
// Use: public

void
SoBaseList::truncate(int start)		// first index to remove
//
////////////////////////////////////////////////////////////////////////
{
    if (addRefs) {
        for (int i = start; i < getLength(); i++) {
            if ( (*this)[i] ) {
                (*this)[i]->unref();
            }
        }
    }

    SbPList::truncate(start);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Copy a list, keeping all reference counts correct
//
// Use: public

void
SoBaseList::copy(const SoBaseList &bList)	// list to copy from
//
////////////////////////////////////////////////////////////////////////
{
    truncate(0);

    if (addRefs) {
        for (int i = 0; i < bList.getLength(); i++) {
            if ( bList[i] ) {
                bList[i]->ref();
            }
        }
    }

    SbPList::copy(bList);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Set an element of a list
//
// Use: public

void
SoBaseList::set(int i,			// index to set
                SoBase *ptr)		// new pointer value
//
////////////////////////////////////////////////////////////////////////
{
    if (addRefs) {
        if ( ptr ) {
            ptr->ref() ;
        }
        if ( (*this)[i] ) {
            (*this)[i]->unref();
        }
    }
    (*(const SbPList *) this) [i] = (void *) ptr;
}
