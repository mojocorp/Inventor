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
 |	This file contains the definitions of subclasses of SbPList for
 |	some of the specific Inventor pointer types so that lists of
 |	pointers can be created easily.
 |
 |   Classes:
 |	subclasses of SbPList:
 |		SoBaseList
 |
 |   Author(s)		: Paul S. Strauss, Nick Thompson, David Mott,
 |                        Gavin Bell
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

#ifndef  _SO_BASE_LISTS_
#define  _SO_BASE_LISTS_

#include <Inventor/SbPList.h>

//////////////////////////////////////////////////////////////////////////////
//
// Subclasses of the SbPList class which hold lists of pointers of a
// specific type.
//
// Each contains:
//	A default constructor
//	A constructor taking an initial number of items in the list
//	An "append" function that adds a pointer to the end of the list
//	The index ([]) operator that returns the nth pointer in the list
//
//////////////////////////////////////////////////////////////////////////////

class SoBase;

class INVENTOR_API SoBaseList : public SbPList {
  public:
    SoBaseList();
    SoBaseList(int size);
    SoBaseList(const SoBaseList &l);
    ~SoBaseList()			{ truncate(0); }

    // Add a pointer to the end of the list
    void		append(SoBase * ptr);

    // Insert given pointer in list before pointer with given index
    void		insert(SoBase *ptr, int addBefore);

    // Remove pointer with given index
    void		remove(int which);

    // Remove all pointers after one with given index, inclusive
    void		truncate(int start);

    // Copy a list, keeping all reference counts correct
    void		copy(const SoBaseList &l);
    SoBaseList &	operator =(const SoBaseList &l)
        { copy(l) ; return *this; }

    // Access an element of a list
    SoBase *		operator [](int i) const
        { return ( (SoBase *) ( (*(const SbPList *) this) [i] ) ); }

    // Set an element of a list
    void		set(int i, SoBase *ptr);

    // Tells list whether to call ref() and unref() for bases in the
    // list when adding/removing them. The default value is TRUE.
    void		addReferences(SbBool flag)	{ addRefs = flag; }

  private:
    // If TRUE (the default), this refs and unrefs things in the list
    SbBool		addRefs;
};

#endif /* _SO_BASE_LISTS_ */
