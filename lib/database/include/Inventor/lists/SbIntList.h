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
 |   $Revision: 1.3 $
 |
 |   Description:
 |	This contains the definition of the SbPList generic pointer
 |	list class. An SbPList is a list of (void *) pointers that
 |	allows easy insertion, removal, and other operations.
 |
 |	The SbIntList class allows lists of integers to be created.
 |
 |   Author(s)		: Paul S. Strauss, Nick Thompson, Gavin Bell
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

#ifndef _SB_INT_LIST_
#define _SB_INT_LIST_

#include <Inventor/SbBasic.h>
#include <Inventor/SbPList.h>

//////////////////////////////////////////////////////////////////////////////
//
//  Class: SbIntList
//
//  List of generic (void *) pointers. SbIntList allows random access,
//  insertion, and removal.
//
//////////////////////////////////////////////////////////////////////////////


SoINTERNAL class INVENTOR_API SbIntList : public SbPList {

  public:
    /// Constructors, similar to SbPList
    SbIntList()					{}
    SbIntList(int initSize) : SbPList(initSize) {}

    void	append(int integer)
        { ((SbPList *) this)->append((void *) (size_t) integer); }

    /// Returns index of given integer in list, or -1 if not found
    int		find(int integer)
        { return ((SbPList *) this)->find((void *) (size_t) integer); }

    /// Inserts given integer in list before integer with given index
    void	insert(int integer, int addBefore)
        { ((SbPList *) this)->insert((void *) (size_t) integer, addBefore); }

    int &	operator [](int i) const
        { return ( reinterpret_cast<int &>( (*(const SbPList *) this) [i] ) ); }
};

#endif /* _SB_INT_LIST_ */
