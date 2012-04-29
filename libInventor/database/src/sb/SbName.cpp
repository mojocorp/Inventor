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
 |	SbName
 |
 |   Author(s)		: Nick Thompson, Paul S. Strauss
 |
 |   Notes:
 |	SbNames are stored in a hash table of SbNameEntries.
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

#include <Inventor/SbName.h>
#include <ctype.h>


//////////////////////////////////////////////////////////////////////////////
//
// SbName class.
//
//////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Default constructor
//
// Use: private

SbName::SbName()
//
////////////////////////////////////////////////////////////////////////
{
    entry = SbNameEntry::insert("");
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Return the position of the first occurrence in the string of the
//    searched content, -1 if not found.
//
// Use: public

int
SbName::find(const char c) const
//
////////////////////////////////////////////////////////////////////////
{
    int index = strchr(entry->string, c)-entry->string;

    return (index>0) ? index : -1;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Return the position of the last occurrence in the string of the
//    searched content, -1 if not found.
//
// Use: public

int
SbName::rfind(char c) const
//
////////////////////////////////////////////////////////////////////////
{
    int index = strrchr(entry->string, c)-entry->string;

    return (index>0) ? index : -1;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Returns TRUE if given character is legal starting character for
//    an identifier.
//
// Use: static, public

SbBool
SbName::isIdentStartChar(char c)
//
////////////////////////////////////////////////////////////////////////
{
    // Digits are illegal as first character:
    if (isdigit(c)) return FALSE;

    return isIdentChar(c);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Returns TRUE if given character is legal character for
//    an identifier.
//
// Use: static, public

SbBool
SbName::isIdentChar(char c)
//
////////////////////////////////////////////////////////////////////////
{
    // Only 0-9, a-z, A-Z and _ are legal:
    if (isalnum(c) || c == '_') return TRUE;

    return FALSE;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Returns TRUE if given character is legal starting character for
//    an SoBase's name
//
// Use: static, public

SbBool
SbName::isBaseNameStartChar(char c)
//
////////////////////////////////////////////////////////////////////////
{
    // Digits are illegal as first character:
    if (isdigit(c)) return FALSE;

    return isIdentChar(c);
}

//
// Characters that are illegal in identifiers:
//   . + ' " \ { }
static const char
badCharacters[] = ".+\'\"\\{}";

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Returns TRUE if given character is legal character for
//    an SoBase's name
//
// Use: static, public

SbBool
SbName::isBaseNameChar(char c)
//
////////////////////////////////////////////////////////////////////////
{
    // First, quick check for common case:
    if (isalnum(c)) return TRUE;

    // Now, look for bad characters:
    if ((strchr(badCharacters, c) != NULL) ||
	isspace(c) || iscntrl(c)) return FALSE;

    // Anything left must be OK
    return TRUE;
}
