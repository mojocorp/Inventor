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
 |		SoPathList
 |
 |   Author(s)		: Paul S. Strauss, Nick Thompson, David Mott,
 |                        Gavin Bell
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

#ifndef _SO_PATH_LISTS_
#define _SO_PATH_LISTS_

#include <Inventor/lists/SoBaseList.h>

class SoPath;

/// Maintains a list of pointers to paths.
/// \ingroup General
/// This subclass of <tt>SoBaseList</tt> holds lists of pointers to
/// <tt>SoPath</tt>s.  It updates reference counts to paths in the list
/// whenever adding or removing pointers.
/// \sa SoPath
class SoPathList : public SoBaseList {
  public:
    /// Constructor.
    SoPathList() : SoBaseList() {}

    /// Constructor that pre-allocates storage for \a size pointers.
    SoPathList(int size) : SoBaseList(size) {}

    /// Constructor that copies the contents of another list.
    SoPathList(const SoPathList &l) : SoBaseList(l) {}

    /// Destructor.
    ~SoPathList() {}

    /// Adds a path to the end of the list.
    void append(SoPath *ptr) { ((SoBaseList *)this)->append((SoBase *)ptr); }

    /// Copies a list, keeping all reference counts correct.
    SoPathList &operator=(const SoPathList &l) {
        SoBaseList::copy(l);
        return *this;
    }

    /// Accesses an element of a list.
    SoPath *operator[](int i) const {
        return ((SoPath *)((*(const SoBaseList *)this)[i]));
    }

    /// Returns the index of the matching path in the list, or -1 if not found.
    int findPath(const SoPath &path);

    /// Sorts list in place based on (1) increasing address of head node, then
    /// (2) increasing indices of children.
    void sort();

    /// Given a sorted list, removes any path that (1) is a duplicate, or (2)
    /// goes through a node that is the tail of another path.
    void uniquify();

  private:
    // Comparison method for path list sorting.
    static int comparePaths(const void *p1Ptr, const void *p2Ptr);
};

#endif /* _SO_PATH_LISTS_ */
