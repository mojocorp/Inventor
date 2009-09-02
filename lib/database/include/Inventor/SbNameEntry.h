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
 |	This file contains definitions of the SbString and SbName
 |	classes, which are useful variations on our friend, the
 |	character string.
 |
 |   Author(s)		: Paul S. Strauss, Nick Thompson
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

#ifndef _SB_NAME_ENTRY_
#define _SB_NAME_ENTRY_

#include <Inventor/SbBasic.h>

//////////////////////////////////////////////////////////////////////////////
//
//  Class: SbNameEntry (internal to SB)
//
//  This is used to make lists of SbName instances.
//
//////////////////////////////////////////////////////////////////////////////


SoINTERNAL class INVENTOR_API SbNameEntry {

  public:
    /// Returns TRUE if entry's string is empty ("")
    SbBool		isEmpty() const   { return (string[0] == '\0'); }

    /// Returns TRUE if entry's string is same as passed string
    SbBool		isEqual(const char *s) const;

  private:
    static int		nameTableSize;	// Number of buckets in name table
    static SbNameEntry	**nameTable;	// Array of name entries
    static struct SbNameChunk *chunk;	// Chunk of memory for string storage

    const char		*string;	// String for this entry
    uint32_t		hashValue;	// Its hash value
    SbNameEntry		*next;		// Pointer to next entry

    // Initializes SbNameEntry class - done only once
    static void		initClass();

    // Constructor
    SbNameEntry(const char *s, uint32_t h, SbNameEntry *n)
    { string = s; hashValue = h; next = n; }

    // Inserts string in table
    static const SbNameEntry *	insert(const char *s);

    friend INVENTOR_API class SbName;
};

#endif /* _SB_NAME_ENTRY_ */
