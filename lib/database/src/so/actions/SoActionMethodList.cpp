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
 |	SoActionMethodList
 |
 |   Author(s)		: Gavin Bell, Paul S. Strauss
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

#include <Inventor/actions/SoActionMethodList.h>
#include <Inventor/nodes/SoNode.h>
#include <Inventor/actions/SoAction.h>

//////////////////////////////////////////////////////////////////////////////
//
//  Class: SoActionMethodList (internal)
//
//////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Constructor
//
// Use: protected

SoActionMethodList::SoActionMethodList(SoActionMethodList *parentList)
//
////////////////////////////////////////////////////////////////////////
{
    parent = parentList;
    numValidTypes = 0;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Adds a method to the traversal list
//
// Use: protected

void
SoActionMethodList::addMethod(SoType nodeType, SoActionMethod method)
//
////////////////////////////////////////////////////////////////////////
{
#ifdef DEBUG
    // Make sure nodeType is a kind of node!
    if (! nodeType.isDerivedFrom(SoNode::getClassTypeId()))
        SoDebugError::post("SoAction::addMethod", "%s is not a node type",
                           nodeType.getName().getString());
#endif /* DEBUG */

    numValidTypes = 0;
    (*this)[SoNode::getActionMethodIndex(nodeType)] = method;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Sets up the traversal list, if necessary.
//
// Use: protected

void
SoActionMethodList::setUp()
//
////////////////////////////////////////////////////////////////////////
{
    if (numValidTypes == SoType::getNumTypes())
        return;		// Already set up the table

    // SoNode's slot must be filled in.  If this action doesn't have a
    // parent action, it is filled in with the null action.  If it
    // does have a parent action, a dummy action is used, and the
    // table is overwritten with the parent's method wherever the
    // dummy action appears in a second pass.
    int i = SoNode::getActionMethodIndex(SoNode::getClassTypeId());
    if ((*this)[i] == NULL) {
        if (parent == NULL)
            (*this)[i] = SoAction::nullAction;
        else
            (*this)[i] = dummyAction;
    }

    // Next, find all nodes derived from SoNode (note: it is a good
    // thing we don't have to do this often, since getAllDerivedFrom
    // must look through the entire list of types).
    SoTypeList nodes;
    SoType::getAllDerivedFrom(SoNode::getClassTypeId(), nodes);

    // Now, for any empty slots, fill in the slot from a parent with a
    // non-NULL slot:
    for (i = 0; i < nodes.getLength(); i++) {
        SoType n = nodes[i];
        if ((*this)[SoNode::getActionMethodIndex(n)] == NULL) {
            (*this)[SoNode::getActionMethodIndex(n)] =
                parentMethod(n);
        }
    }

    // Inherit any undefined methods from parent class
    if (parent != NULL) {
        parent->setUp();

        for (i = 0; i < getLength(); i++) {

            SoActionMethod	&method = (*this)[i];

            if (method == dummyAction)
                method = (*parent)[i];
        }
    }

    numValidTypes = SoType::getNumTypes();
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Dummy action used to mark entries in the action/method table as
//    empty.
//
// Use: internal

void
SoActionMethodList::dummyAction(SoAction *, SoNode *)
//
////////////////////////////////////////////////////////////////////////
{
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Recursively looks for a non-NULL action method from a node's
//    parents.
//
// Use: internal

SoActionMethod
SoActionMethodList::parentMethod(SoType t)
//
////////////////////////////////////////////////////////////////////////
{
    SoActionMethod m;
    SoType parent = t;

    // Look through parents until non-NULL method is found
    do {
        parent = parent.getParent();
        m = (*this)[SoNode::getActionMethodIndex(parent)];
    } while (m == NULL);

    return m;
}
