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
 |		SoTypeList
 |
 |   Author(s)		: Paul S. Strauss, Nick Thompson, David Mott,
 |                        Gavin Bell
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

#ifndef _SO_TYPE_LISTS_
#define _SO_TYPE_LISTS_

#include <Inventor/SbPList.h>
#include <Inventor/SoType.h>

/// Maintains a list of SoTypes
/// \ingroup General
/// This subclass of <tt>SbPList</tt> holds lists of <tt>SoType</tt> type
/// identifiers. \sa SoType
class SoTypeList : public SbPList {
  public:
    /// Constructor.
    SoTypeList() : SbPList() {}

    /// Constructor that pre-allocates storage for \a size types.
    SoTypeList(int size) : SbPList(size) {}

    /// Constructor that copies the contents of another list.
    SoTypeList(const SoTypeList &l) : SbPList(l) {}

    /// Destructor.
    ~SoTypeList() { truncate(0); }

    /// Add a typeId to the end of the list
    void append(SoType typeId);

    /// Returns index of given typeId in list, or -1 if not found
    int find(SoType typeId) const;

    /// Insert given typeId in list before typeId with given index
    void insert(SoType typeId, int addBefore);

    /// Access an element of a list
    SoType operator[](int i) const;

    /// Set an element of a list
    void set(int i, SoType typeId);
};

#endif /* _SO_TYPE_LISTS_ */
