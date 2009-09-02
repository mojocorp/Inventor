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
 |	Defines the SoAction class and related classes.
 |
 |   Author(s)		: Paul S. Strauss, Gavin Bell
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

#ifndef  _SO_ENABLEDELEMENTSLIST_
#define  _SO_ENABLEDELEMENTSLIST_

#include <Inventor/misc/SoBasic.h>
#include <Inventor/SoLists.h>

//////////////////////////////////////////////////////////////////////////////
//
//  Class: SoEnabledElementsList
//
//  Internal class.  A list of elements enabled for a given action
//  class. Lists of elements can be merged, allowing an action class
//  to inherit enabled elements from its parent class.
//
//////////////////////////////////////////////////////////////////////////////

SoINTERNAL class INVENTOR_API SoEnabledElementsList {

  public:
    /// Constructor
    SoEnabledElementsList(SoEnabledElementsList *parentList);

    /// Returns list of elements.  This automatically merges the
    /// elements with elements enabled in the parentList.
    const SoTypeList &	getElements() const;

    /// Adds an element to the list if it's not already in it
    void		enable(SoType elementType, int stackIndex);

    /// Enables all elements from the given list that are not already
    /// enabled in this one
    void		merge(const SoEnabledElementsList &list);

    /// Returns the current setting of the global counter used to
    /// determine when lists are out of date.
    static int		getCounter()		{ return counter; }

  private:
    // This maintains a global counter used to determine when lists
    // are out of date. It is incremented whenever a new element is
    // added to a list.
    static int		counter;

    // And a per-instance counter so we don't merge enabled elements
    // with the parent list unnecessarily.
    int			setUpCounter;

    // This list holds type id's of enabled elements, indexed by the
    // stack index of the elements.
    SoTypeList		elements;

    // Pointer to parent's list of enabled elements (NULL for
    // SoAction).
    SoEnabledElementsList	*parent;
};

#endif /* _SO_ENABLEDELEMENTSLIST_ */
