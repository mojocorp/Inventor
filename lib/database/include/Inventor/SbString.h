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
 |	This file contains definitions of the SbString
 |	class, which are useful variations on our friend, the
 |	character string.
 |
 |   Author(s)		: Paul S. Strauss, Nick Thompson
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

#ifndef _SB_STRING_
#define _SB_STRING_

#include <Inventor/SbBasic.h>

#include <string>

//////////////////////////////////////////////////////////////////////////////
//
//  Class: SbString
//
//  "Smart" character strings, which allow things like concatenation
//  with the "+=" operator and automatic storage management.
//
//////////////////////////////////////////////////////////////////////////////

class SbString {
  public:

    // Default constructor
    SbString()				{}

    // Constructor that initializes to given character string
    SbString(const char *str)		{ string = str; }

    // Constructor that initializes to given character string
    SbString(const char *str, size_t start, size_t end);

    // Constructor that initializes to given SbString
    SbString(const SbString &str)	{ string = str.string; }

    // Constructor that initializes to string formed from given integer.
    // For example, SbString(1234) gives the string "1234".
    SbString(int digitString);

    // Destructor
    ~SbString();

    // Returns a reasonable hash key for string
    uint32_t		hash()		{ return SbString::hash(string.data()); }

    // Returns length of string
    size_t			getLength() const	{ return string.size(); }

    // Sets string to be the empty string ("").
    void		makeEmpty();

    // Returns pointer to the character string
    const char *	getString() const	{ return string.data(); }

    // Returns new string representing sub-string from startChar to
    // endChar, inclusive. If endChar is -1 (the default), the
    // sub-string from startChar until the end is returned.
    SbString		getSubString(int startChar, int endChar = -1) const;

    // Deletes the characters from startChar to endChar, inclusive,
    // from the string. If endChar is -1 (the default), all characters
    // from startChar until the end are deleted.
    void		deleteSubString(int startChar, int endChar = -1);

    // Assignment operator for character string, SbString
    SbString &		operator =(const char *str);
    SbString &		operator =(const SbString &str)
    { return (*this = str.string.data()); }

    // Concatenation operator "+=" for string, SbString
    SbString &		operator +=(const char *str);

    SbString &		operator +=(const SbString &str);

    // Unary "not" operator; returns TRUE if string is empty ("")
    bool			operator !() const { return string.empty(); }

    // Equality operator for SbString/char* and SbString/SbString comparison
    friend bool		operator ==(const SbString &str, const char *s);

    friend bool		operator ==(const char *s, const SbString &str)
	{ return (str.string == s); }


    friend bool		operator ==(const SbString &str1, const SbString &str2)
    { return (str1.string == str2.string); }

    // Inequality operator for SbString/char* and SbString/SbString comparison
    friend bool		operator !=(const SbString &str, const char *s);

    friend bool		operator !=(const char *s, const SbString &str)
	{ return (str.string != s); }

    friend const SbString operator +( const SbString & s1, const SbString & s2 ) {
        SbString t(s1);
        t += s2;
        return t;
    }

    friend int		operator !=(const SbString &str1,
				    const SbString &str2)
    { return (str1.string != str2.string); }

  SoINTERNAL public:

    static uint32_t	hash(const char *s);    // Hash function

  private:
    std::string string;  // String storage
};

#endif /* _SB_STRING_ */
