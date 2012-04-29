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
 |	The SbVec3fList class allows lists of vector3 to be created.
 |
 |   Author(s)		: Paul S. Strauss, Nick Thompson, Gavin Bell
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

#ifndef _SB_VEC3F_LIST_
#define _SB_VEC3F_LIST_

#include <Inventor/SbBasic.h>
#include <Inventor/SbPList.h>
#include <Inventor/SbVec3f.h>

//////////////////////////////////////////////////////////////////////////////
//
//  Class: SbVec3fList
//
//  List of vectors. Probably mostly used for vertices of polygons
//
//////////////////////////////////////////////////////////////////////////////

SoINTERNAL class INVENTOR_API SbVec3fList : public SbPList {

  public:
     SbVec3fList() {}
    ~SbVec3fList();

    void	append(SbVec3f *vec)
        { ((SbPList *) this)->append(new SbVec3f(vec->getValue())); }

    /// Inserts given integer in list before integer with given index
    void	insert(SbVec3f *vec, int addBefore)
        { ((SbPList *) this)->insert(new SbVec3f(vec->getValue()),
                addBefore); }

    SbVec3f *   operator [](int i) const
        { return ( (SbVec3f *) ( (*(const SbPList *) this) [i] ) ); }
};

#endif /* _SB_VEC3FLIST_ */
