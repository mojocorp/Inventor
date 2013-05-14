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
 * Copyright (C) 1990,91,92   Silicon Graphics, Inc.
 *
 _______________________________________________________________________
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 |
 |   $Revision: 1.2 $
 |
 |   Classes:
 |	SoLightPath
 |
 |   Author(s)		: Paul S. Strauss, Nick Thompson, Alan Norton
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

#include <Inventor/SoLightPath.h>
#include <Inventor/misc/SoTempPath.h>
#include <Inventor/misc/SoChildList.h>
#include <Inventor/errors/SoDebugError.h>

////////////////////////////////////////////////////////////////////////
//
// SoLightPath class
// This is a lightweight version of SoTempPath.  It maintains only a
// list of child indices, for fast pushing and popping during traversal.
//
////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////
//
// Description:
//    Constructor given approximate number of nodes in chain.
//
// Use: public

SoLightPath::SoLightPath(int approxLength) : indices(approxLength)
//
////////////////////////////////////////////////////////////////////////
{
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Destructor
//
// Use: private

SoLightPath::~SoLightPath()
//
////////////////////////////////////////////////////////////////////////
{
    truncate(0);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Sets the head (first) node of the path chain. The path is
//    cleared first, if necessary.
//
// Use: public

void
SoLightPath::setHead(SoNode *node)
//
////////////////////////////////////////////////////////////////////////
{
    // Clear out existing nodes
    truncate(0);

    // The head index value is set to -1, it should never be referenced.
    indices.append(-1);
    headNode = node;

}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    fills in values for a full (Temp) path that is a copy of
//    all of this LightPath. The temp path should already be constructed.
//
// Use: public

void
SoLightPath::makeTempPath(SoTempPath *tmpPath) const
//
////////////////////////////////////////////////////////////////////////
{
    int		i;

    if (tmpPath == NULL){
        SoDebugError::post("SoLightPath::makeTempPath",
                         "Error, NULL SoTempPath");
        return;
        }

    tmpPath->setHead(headNode);
    for (i = 1; i < getFullLength(); i++)
        {
#ifdef DEBUG
        if (indices[i] < 0 ) {
            SoDebugError::post("SoLightPath::makeTempPath",
                         "Error, invalid indices of child");
            return;
            }
#endif
        tmpPath->append(indices[i]);
        }
    return;
}

//////////////////////////////////////////////////////////////
//
// Description:
//    Returns SoNode that is ith node in path
//    (where 0th node is head node)
//
//    use: public
//
SoNode *
SoLightPath::getNode(int i) const
//
/////////////////////////////////////////////////////////////
{
    SoNode * curNode = headNode;
    SoChildList *children;
    for(int j=0; j<i; j++){
        //traverse all (even hidden) children
        children = curNode->getChildren();
#ifdef DEBUG
        // Make sure it's valid:
        if(children == NULL || j>= getFullLength()) {
            SoDebugError::post("SoLightPath::getNode",
                         "Error, no child at level");
        }
#endif
        curNode = (*children)[indices[j+1]];
    }
    return curNode;
}
