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
 |		SoFieldList
 |
 |   Author(s)		: Paul S. Strauss, Nick Thompson, David Mott,
 |                        Gavin Bell
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

#ifndef  _SO_FIELD_LISTS_
#define  _SO_FIELD_LISTS_

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

class SoField;

/// Maintains a list of pointers to fields.
/// \ingroup Fields
/// This subclass of <tt>SbPList</tt> holds lists of pointers to
/// instances of classes derived from <tt>SoField</tt>.
/// \sa SoField
class INVENTOR_API SoFieldList : public SbPList {
  public:
    /// Constructor.
    SoFieldList()			: SbPList()	{}

    /// Constructor that pre-allocates storage for \a size pointers.
    SoFieldList(int size)		: SbPList(size)	{}

    /// Constructor that copies the contents of another list.
    SoFieldList(const SoFieldList &l)	: SbPList(l)	{}

    /// Destructor.
    ~SoFieldList()			{ truncate(0); }

    /// Add a Field to the end of the list
    void		append(SoField *field)
        { SbPList::append((void *) field); }

    /// Insert given field in list before field with given index
    void		insert(SoField *field, int addBefore)
        { SbPList::insert((void *) field, addBefore); }

    /// Access an element of a list
    SoField *		operator [](int i) const
        { return ( (SoField *) ( (* (const SbPList *) this) [i] ) ); }

    /// Set an element of a list
    void		set(int i, SoField *Field)
        { (* (const SbPList *) this) [i] = (void *) Field; }

// Internal versions of [] that do not check for bounds:
  SoINTERNAL public:
    SoField *		get(int i) const
        { return (SoField *)SbPList::get(i); }
};

#endif /* _SO_FIELD_LISTS_ */
