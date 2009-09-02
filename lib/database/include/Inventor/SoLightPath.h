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
 |   $Revision: 1.2 $
 |
 |   Description:
 |	This file contains the definition of the SoLightPath classe
 |
 |   Author(s)		: Paul S. Strauss, Nick Thompson, Alan Norton
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

#ifndef  _SO_LIGHT_PATH_
#define  _SO_LIGHT_PATH_

#include <Inventor/SoPath.h>

//////////////////////////////////////////////////////////////////////////////
//
//  SoINTERNAL Class: SoLightPath
//
//  A SoLightPath is a light-weight version of an SoTempPath, intended to
//  be used just to keep track of the current path during traversal.
//  Eventually it is intended to replace SoTempPath (when appropriate
//  modifications are made to SoDraggers).
//  Unlike SoPath, it keeps only a chain of childIndices and a headnode.
//  The methods implemented are only those needed for traversal.
//
//////////////////////////////////////////////////////////////////////////////


class INVENTOR_API SoLightPath  {

  SoINTERNAL public:

    /// Constructor given approximate number of nodes in chain
    SoLightPath(int approxLength);


    /// Sets head node (first node in chain)
    /// Resulting path has only one node.
    void		setHead(SoNode *node);

    /// Adds node specified by childindex to end of chain.
    void		append(int childIndex)
        { indices.append(childIndex);}


    /// Allows path to be treated as a stack: push a node at the end of
    /// the chain and pop the last node off
    void		push(int childIndex)	{ append(childIndex); }
    void		push()			{ append(-1);}
    void		pop()		{ truncate(getFullLength() - 1); }
    void		setTail(int childIndex)
                { indices.set(getFullLength()-1,(void *)(unsigned long)childIndex);}
    SoNode *		getTail()
                { return getNode(getFullLength()-1);}

    /// Returns the first node in a path chain.
    SoNode *		getHead() const	{ return headNode; }

    /// Returns pointer to ith node in chain
    SoNode *		getNode(int i) const;

    /// Returns full length of path chain (number of nodes)
    /// note that public/private distinction is ignored.
    int			getFullLength() const {return indices.getLength();}

    /// Removes all nodes from indexed node on
    void		truncate(int start) { indices.truncate(start);}


    /// fills in nodes for a TempPath that is represented by this
    /// SoLightPath.  Called by SoAction::getCurPath();
    void 		makeTempPath(SoTempPath *) const;

  private:

    ~SoLightPath();
    SoNode *		headNode;	// Pointer to headnode
    SbIntList		indices;	// Child indices


    friend class SoAction;
};

#endif /* _SO_LIGHT_PATH_ */
