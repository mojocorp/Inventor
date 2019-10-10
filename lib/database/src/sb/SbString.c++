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
 |   $Revision: 1.3 $
 |
 |   Classes:
 |	SbString
 |
 |   Author(s)		: Nick Thompson, Paul S. Strauss
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

#include <Inventor/SbString.h>
#include <Inventor/errors/SoDebugError.h>

//
// Constructor that initializes to a substring.
//

SbString::SbString(const char *str, size_t start, size_t end)
{
    string = std::string(str, start, end - start + 1);
}

//
// Constructor that initializes to string formed from given integer.
// For example, SbString(1234) gives the string "1234".
//

SbString::SbString(int digitString)
{
    string.resize(32);
#ifdef SB_OS_WIN
    _snprintf(&string[0], string.size(),"%d",digitString);
#else
    snprintf(&string[0], string.size(),"%d",digitString);
#endif
}

//
// Destructor
//

SbString::~SbString()
{
}

// Simple hashing algorithm that will xor all the characters in a string
//   after shifting to the left 0, 5, 10, 15, 20, 1, 6, 11, 16, 21, 2, ... 
//   positions.

uint32_t
SbString::hash(const char *s)
{
    uint32_t	total, shift;

    total = shift = 0;
    while (*s) {
	total = total ^ ((*s) << shift);
	shift+=5;
	if (shift>24) shift -= 24;
	s++;
    }

    return( total );
}

//
// Sets string to be the empty string (""). If freeOld is TRUE
// (default), any old storage is freed up.
//

void
SbString::makeEmpty()
{
    string.clear();
}

//
// Returns new string representing given sub-string. If endChar is
// -1 (the default), the sub-string from startChar until the end
// is returned.
//

SbString
SbString::getSubString(int startChar, int endChar) const
{
    size_t size = (endChar!=-1) ? endChar - startChar + 1 : std::string::npos;

    SbString str;
    str.string = string.substr(startChar, size);
    return str;
}

//
// Deletes the specified characters from the string. If endChar is
// -1 (the default), all characters from startChar until the end
// are deleted.
//

void
SbString::deleteSubString(int startChar, int endChar)
{
    std::string str = string;
    string = std::string(str, 0, startChar);
    if (endChar != -1)
        string += std::string(str, endChar+1, std::string::npos);
}

//
// Assignment operator for character string
//

SbString &
SbString::operator =(const char *str)
{
    string = str;
    return *this;
}

//
// Concatenation operator "+=" for string
//

SbString &
SbString::operator +=(const char *str)
{
    string += str;
    return *this;
}

//
// Concatenation operator "+=" for SbString
//

SbString &
SbString::operator +=(const SbString &str)
{
    string += str.string;
    return *this;
}

//
// Equality operator for SbString/char* and SbString/SbString comparison
//

bool
operator ==(const SbString &str, const char *s)
{
    return (str.string == s);
}

//
// Inequality operator for SbString/char* and SbString/SbString comparison
//

bool
operator !=(const SbString &str, const char *s)
{
    return (str.string != s);
}
