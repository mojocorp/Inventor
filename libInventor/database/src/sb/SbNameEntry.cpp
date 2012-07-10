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
 |	SbNameEntry
 |
 |   Author(s)		: Nick Thompson, Paul S. Strauss
 |
 |   Notes:
 |	SbNames are stored in a hash table of SbNameEntries.
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

#include <Inventor/SbNameEntry.h>
#include <Inventor/SbString.h>

//////////////////////////////////////////////////////////////////////////////
//
// SbNameEntry class. Instances of this class are stored in a hash
// table that is used to find names quickly. All of these methods are
// private. Some are used by SbName, which is a friend class.
//
//////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////
//
// This structure contains a chunk of memory in which strings are
// stored. Doing this reduces memory fragmentation.
//
////////////////////////////////////////////

// The size of a chunk is about a page (4K) minus room for other stuff
// in the structure
#define CHUNK_SIZE	4000

struct SbNameChunk {
    char		mem[CHUNK_SIZE];	// Chunk o'memory
    char		*curByte;		// Current byte of chunk
    int			bytesLeft;		// Bytes left in chunk
    struct SbNameChunk	*next;			// Pointer to next chunk
};

int		SbNameEntry::nameTableSize; // Number of buckets in name table
SbNameEntry **	SbNameEntry::nameTable;	    // Array of name entries
struct SbNameChunk *SbNameEntry::chunk;	    // Chunk of memory for string store

SbBool
SbNameEntry::isEqual(const char *s) const
{
    return (string[0] == s[0] && ! strcmp(string, s));
}

bool
SbNameEntry::operator<(const SbNameEntry &other) const
{
    return strcmp(string,other.string)<0;
}
////////////////////////////////////////////////////////////////////////
//
// Description:
//    Initializes SbNameEntry class. This is done only once.
//
// Use: private

void
SbNameEntry::initClass()
//
////////////////////////////////////////////////////////////////////////
{
    int i;

    // Allocate and empty name hash table

    nameTableSize = 1999;	// About 2000 (primed)
    nameTable	  = new SbNameEntry *[nameTableSize];

    for (i = 0; i < nameTableSize; i++)
        nameTable[i] = NULL;

    chunk = NULL;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Inserts a string into the hash table.
//
// Use: private

const SbNameEntry *
SbNameEntry::insert(const char *s)
//
////////////////////////////////////////////////////////////////////////
{
    uint32_t		h = SbString::hash(s);
    uint32_t		i;
    SbNameEntry		*entry;
    SbNameEntry		*head;

    if (nameTableSize == 0)
        initClass();

    i = h % nameTableSize;
    entry = head = nameTable[i];

    // Look for entry with same string
    while (entry != NULL) {
        if (entry->hashValue == h && entry->isEqual(s))
            break;
        entry = entry->next;
    }

    // If not there, create one
    if (entry == NULL) {

        size_t len = strlen(s) + 1;

        // If there's no way to fit this string in a chunk
        if (len >= CHUNK_SIZE)
            s = strdup(s);

        else {

            // Make sure there's room in the current chunk; create a
            // new chunk if necessary.
            if (chunk == NULL || chunk->bytesLeft < len) {
                struct SbNameChunk	*newChunk = new SbNameChunk;

                newChunk->curByte   = newChunk->mem;
                newChunk->bytesLeft = CHUNK_SIZE;
                newChunk->next      = chunk;

                chunk = newChunk;
            }

            // Store string in chunk
            strcpy(chunk->curByte, s);
            s = chunk->curByte;

            chunk->curByte   += len;
            chunk->bytesLeft -= len;
        }

        entry = new SbNameEntry(s, len-1, h, head);
        nameTable[i] = entry;
    }

    return entry;
}
