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

#ifndef _SB_PLIST_
#define _SB_PLIST_

#include <Inventor/SbBasic.h>

/// List of generic (void *) pointers.
/// \ingroup Basics
/// This class manages a dynamic list of generic <b>void *</b> pointers.
/// This class allows random access, insertion, and removal.
class INVENTOR_API SbPList {
  public:

    /// Constructor
    SbPList();

    /// Constructor allocates enough room for the given number of pointers
    SbPList(int initSize);

    /// Constructor for copying
    SbPList(const SbPList &pl);

    /// Destructor
    ~SbPList();

    /// Adds given pointer to end of list
    void	append(void * ptr)
	{ if (nPtrs + 1 > ptrsSize) expand(nPtrs + 1);
	  ptrs[nPtrs++] = ptr; }

    /// Returns index of given pointer in list, or -1 if not found
    int		find(const void *ptr) const;

    /// Inserts given pointer in list before pointer with given index
    void	insert(void *ptr, int addBefore);

    /// Removes pointer with given index
    void	remove(int which);

    /// Returns number of pointers in list
    int		getLength() const		{ return (int) nPtrs;	}

    /// Removes all pointers after one with given index, inclusive
    void	truncate(int start)
	{ nPtrs = start; }

    /// Copy a PList
    void	copy(const SbPList &pl);

    /// Assignment operator; copies list into this list.
    SbPList &	operator =(const SbPList &pl)	{ copy(pl); return *this; }

    /// Returns pointer with given index
    void *&	operator [](int i) const
    	{ if (i >= nPtrs) grow(i); return ptrs[i]; }

    /// Equality operators.
    int		operator ==(const SbPList &pl) const
	{ return pl.nPtrs == nPtrs ? compare(pl) : FALSE; }

    /// Inequality operators.
    int		operator !=(const SbPList &pl) const
	{ return pl.nPtrs == nPtrs ? ! compare(pl) : TRUE; }
  
// Internal versions of [] that do not check for bounds:
  SoINTERNAL public:
    void *	get(int i) const { return ptrs[i]; }
    void	set(int i, void *j) { ptrs[i] = j; }

  private:

    // NOTE: this must be called only if the number of elements in the two
    // lists is the same, otherwise badness could result
    int		compare(const SbPList &pl) const;

    void **	ptrs;		// The collection of pointers
    int		nPtrs;		// Number of pointers used
    int		ptrsSize;	// Number of pointers allocated

    // There are three(!) methods to expand the list.  grow() is used
    // when space is dynamically created, and needs to be initialized
    // to NULL:
    void	grow(int max) const;

    // setSize is used by grow and in other places internally where we
    // know that nothing needs to be initialized to NULL.
    void	setSize(int size)
	{ if (size > ptrsSize) expand(size); nPtrs = size; }

    // expand is the lowest level routine.  It just reallocates the
    // array and copies over the old values.
    void	expand(int size);
};

#endif /* _SB_PLIST_ */
