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

#ifndef  _SO_ACTIONMETHODLIST_
#define  _SO_ACTIONMETHODLIST_

#include <Inventor/misc/SoBasic.h>
#include <Inventor/SbPList.h>
#include <Inventor/SoType.h>

//////////////////////////////////////////////////////////////////////////////
//
//  Class: SoActionMethodList
//
//  Internal class.  A list of routines to call, one for each node type.
//
//////////////////////////////////////////////////////////////////////////////
class SoAction;
class SoNode;

typedef void	(*SoActionMethod)(SoAction *, SoNode *);

SoINTERNAL class INVENTOR_API SoActionMethodList : public SbPList {

  public:
    // Constructor.  Pass in list from parent action.
    SoActionMethodList(SoActionMethodList *parentList);

    // Operator used to get and set methods.  The list will grow
    // dynamically as we access items off the end of the list, and
    // entries will be initialized to NULL.
    SoActionMethod &	operator [](int i) const
        { return ((SoActionMethod &)((*(const SbPList *)this)[i])); }

    // Add a method to the appropriate place in the list.
    void		addMethod(SoType nodeType, SoActionMethod method);

    // This MUST be called before using the list.  It fills in NULL
    // entries with their parents' method.
    void		setUp();

  private:
    SoActionMethod	parentMethod(SoType);	// Method from parent node
    SoActionMethodList	*parent;
    int			numValidTypes;   // Stores the number of types at most
                                         // recent setup

    // Dummy action used internally
    static void		dummyAction(SoAction *, SoNode *);

};

#endif /* _SO_ACTIONMETHODLIST_ */
