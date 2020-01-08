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
 |		SoPickedPointList
 |
 |   Author(s)		: Paul S. Strauss, Nick Thompson, David Mott,
 |                        Gavin Bell
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

#ifndef _SO_PICKED_POINT_LISTS_
#define _SO_PICKED_POINT_LISTS_

#include <Inventor/SbPList.h>

class SoPickedPoint;

/// Maintains a list of pointers to SoPickedPoint instances.
/// \ingroup General
/// This subclass of <tt>SbPList</tt> holds lists of pointers to
/// instances of classes derived from <tt>SoPickedPoint</tt>. It is used
/// primarily to return information from picking with the
/// <tt>SoRayPickAction</tt> class. \sa SoPickedPoint, SoRayPickAction
class SoPickedPointList : public SbPList {
  public:
    /// Constructor.
    SoPickedPointList() : SbPList() {}

    /// Constructor that pre-allocates storage for \a size pointers.
    SoPickedPointList(int size) : SbPList(size) {}

    /// Constructor that copies the contents of another list.
    SoPickedPointList(const SoPickedPointList &l);

    /// Destructor
    ~SoPickedPointList() { truncate(0); }

    /// Add a pickedPoint to the end of the list
    void append(SoPickedPoint *pickedPoint) {
        SbPList::append((void *)pickedPoint);
    }

    /// Insert given pickedPoint in list before pickedPoint with given index
    void insert(SoPickedPoint *pickedPoint, int addBefore) {
        SbPList::insert((void *)pickedPoint, addBefore);
    }

    /// Removes all pointers after one with given index, inclusive, deleting all
    /// instances removed from the list.
    void truncate(int start);

    /// Access an element of a list
    SoPickedPoint *operator[](int i) const {
        return ((SoPickedPoint *)((*(const SbPList *)this)[i]));
    }

    /// Set an element of a list, deleting old entry
    void set(int i, SoPickedPoint *pickedPoint);
};

#endif /* _SO_PICKED_POINT_LISTS_ */
