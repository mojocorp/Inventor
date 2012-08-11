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

#ifndef _SB_NAME_
#define _SB_NAME_

#include <Inventor/SbBasic.h>
#include <Inventor/SbNameEntry.h>
#include <Inventor/SbString.h>

/// Character string stored in a hash table.
/// \ingroup Basics
/// This class of strings stores the string in a hash table. It is
/// used by the Inventor toolkit for keywords and other unique names.
/// It is not recommended for general use (only in the context of Inventor
/// objects). When a string is stored in this table, a pointer to the
/// storage is returned. Two identical strings will return the same pointer.
/// This means that comparison of two <tt>SbName</tt>s
/// for equality can be accomplished by comparing their identifiers.
/// <tt>SbName</tt>s
/// are used for strings which are expected to show up frequently, such as
/// node names.
/// \sa SbString
class INVENTOR_API SbName {
public:
    /// Default constructor
    SbName();

    /// Constructor that initializes to given character string
    SbName(const char *s) {
        entry = SbNameEntry::insert(s);
    }

    /// Constructor that initializes to given SbString
    SbName(const SbString &s) {
        entry = SbNameEntry::insert(s.getString());
    }


    /// Constructor that initializes to another SbName
    SbName(const SbName &n) {
        entry = n.entry;
    }

    ~SbName() {}

    /// Returns pointer to the character string
    const char  *getString() const {
        return entry->string;
    }

    /// Returns length of string
    size_t getLength() const {
        return entry->length;
    }

    /// Return the position of the first occurrence in the char of the searched content, -1 if not found.
    int find(char c) const;

    /// Return the position of the last occurrence in the char of the searched content, -1 if not found.
    int rfind(char c) const;

    /// Returns TRUE if given character is a legal starting character
    /// for an identifier
    static SbBool isIdentStartChar(char c);

    /// Returns TRUE if given character is a legal nonstarting
    /// character for an identifier
    static SbBool isIdentChar(char c);

    /// Returns TRUE if given character is a legal starting character
    /// for an SoBase's name
    static SbBool isBaseNameStartChar(char c);

    /// Returns TRUE if given character is a legal nonstarting
    /// character for an SoBase's name
    static SbBool isBaseNameChar(char c);

    /// Unary "not" operator; returns TRUE if string is empty ("")
    int operator !() const   {
        return entry->isEmpty();
    }

    /// Less than operator (using alphabetical order).
    bool operator<(const SbName &other) const {
        return *entry < *other.entry;
    }

    /// Equality operator for SbName/char* and SbName/SbName comparison
    friend INVENTOR_API int operator ==(const SbName &n, const char *s) {
        return n.entry->isEqual(s);
    }

    /// Equality operator for SbName/char* and SbName/SbName comparison
    friend INVENTOR_API int operator ==(const char *s, const SbName &n) {
        return n.entry->isEqual(s);
    }

    /// Equality operator for SbName/char* and SbName/SbName comparison
    friend INVENTOR_API int operator ==(const SbName &n1, const SbName &n2) {
        return n1.entry == n2.entry;
    }

    /// Inequality operator for SbName/char* and SbName/SbName comparison
    friend INVENTOR_API int operator !=(const SbName &n, const char *s) {
        return ! n.entry->isEqual(s);
    }

    /// Inequality operator for SbName/char* and SbName/SbName comparison
    friend INVENTOR_API int operator !=(const char *s, const SbName &n) {
        return ! n.entry->isEqual(s);
    }

    /// Inequality operator for SbName/char* and SbName/SbName comparison
    friend INVENTOR_API int operator !=(const SbName &n1, const SbName &n2) {
        return n1.entry != n2.entry;
    }

private:
    const SbNameEntry *entry;  // Name string storage
};

#endif /* _SB_NAME_ */
