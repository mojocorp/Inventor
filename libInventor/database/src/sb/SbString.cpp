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

#include <string>
#include <cctype> //tolower toupper
#include <algorithm>

#ifdef SB_OS_WIN
#   include<windows.h>
#endif

// "Counting characters in UTF-8 strings is fast" by Kragen Sitaker
size_t strlen_utf8(const char *s) {
    size_t i = 0, j = 0;
    while (s[i]) {
        if ((s[i] & 0xc0) != 0x80) j++;
        i++;
    }
    return j;
}

//
// Constructor that initializes to a substring.
//

SbString::SbString() {
    string = staticStorage;
    string[0] = '\0';
}

//
// Constructor that initializes to a substring.
//

SbString::SbString(const char *str) {
    string = staticStorage;
    *this = fromLatin1(str);
}

//
// Constructor that initializes to a substring.
//

SbString::SbString(const char *str, int start, int end)
{
    int size = end - start + 1;

    if (size < SB_STRING_STATIC_STORAGE_SIZE)
        string = staticStorage;
    else
        string = new char[size+1];

    strncpy(string, str+start, size);
    string[size] = '\0';
    storageSize = size;
}

//
// Constructor that initializes to a substring.
//

SbString::SbString(const SbString &str) {
    string = staticStorage;
    *this = str;
}

//
// Constructor that initializes to string formed from given integer.
// For example, SbString(1234) gives the string "1234".
//

SbString::SbString(int digitString)
{
    string = staticStorage;
#ifdef SB_OS_WIN
    _snprintf(string,SB_STRING_STATIC_STORAGE_SIZE,"%d",digitString);
#else
    snprintf(string,SB_STRING_STATIC_STORAGE_SIZE,"%d",digitString);
#endif
}

//
// Destructor
//

SbString::~SbString()
{
    if (string != staticStorage)
        delete [] string;
}

//
// Makes more room in storage for string for n more bytes,
// allocating or reallocating if necessary.
//

void
SbString::expand(size_t bySize)
{
    size_t newSize = strlen(string) + bySize + 1;

    if (newSize >= SB_STRING_STATIC_STORAGE_SIZE &&
            (string == staticStorage || newSize > storageSize)) {

        char *newString = new char[newSize];

        strcpy(newString, string);

        if (string != staticStorage)
            delete [] string;

        string      = newString;
        storageSize = newSize;
    }
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

size_t
SbString::getLength() const
{
    return strlen_utf8(string);
}

//
// Sets string to be the empty string (""). If freeOld is TRUE
// (default), any old storage is freed up.
//

void
SbString::makeEmpty(SbBool freeOld)
{
    if (string != staticStorage) {
        if (freeOld)
            delete [] string;
        string = staticStorage;
    }
    string[0] = '\0';
}

std::wstring
SbString::toStdWString () const
{
    std::wstring wstr;
#ifdef SB_OS_WIN
    int wlen = MultiByteToWideChar(CP_UTF8, 0, string, -1, NULL, 0);
    wstr.resize(wlen-1);
    MultiByteToWideChar(CP_UTF8, 0, string, -1, (LPWSTR)wstr.data(), wlen);
#else
    setlocale(LC_CTYPE, "en_US.UTF-8");
    size_t wlen = mbstowcs(NULL, string, 0);
    wstr.resize(wlen);
    mbstowcs((wchar_t*)wstr.data(), string, wlen+1);
#endif
    return wstr;
}

int
SbString::find(const SbString & str, int pos) const
{
    size_t index = std::string(string).find(str.getString(), pos);

    return (index!=std::string::npos) ? (int)index : -1;
}

int
SbString::rfind(const SbString & str, int pos) const
{
    pos = (pos==-1) ? (int)std::string::npos : pos;

    size_t index = std::string(string).rfind(str.string, pos);

    return (index!=std::string::npos) ? (int)index : -1;
}

//
// Returns new string representing given sub-string. If endChar is
// -1 (the default), the sub-string from startChar until the end
// is returned.
//

SbString
SbString::getSubString(int startChar, int endChar) const
{
    size_t		len = strlen(string);

    // Get substring that starts at specified character
    SbString	tmp = SbString::fromUtf8(&string[startChar]);

    // Delete characters from end if necessary
    if (endChar >= 0 && endChar < (int)len - 1)
        tmp.deleteSubString(endChar - startChar + 1);

    return tmp;
}

//
// Deletes the specified characters from the string. If endChar is
// -1 (the default), all characters from startChar until the end
// are deleted.
//

void
SbString::deleteSubString(int startChar, int endChar)
{
    size_t len = strlen(string);

    // Modify string in place
    if (endChar < 0 || endChar >= (int)len - 1)
        string[startChar] = '\0';
    else {

#ifdef DEBUG
        if (startChar > endChar) {
            SoDebugError::post("SbString::deleteSubString",
                               "startChar > endChar");
            return;
        }
#endif

        int	numToMove = len - endChar - 1;

        for (int i = 0; i < numToMove; i++)
            string[startChar + i] = string[endChar + i + 1];
        string[startChar + numToMove] = '\0';
    }

    // Use temporary string to allow us to free up old storage if necessary
    SbString	tmp = SbString::fromUtf8(string);
    *this = tmp;
}

//
// Assignment operator for character string
//
SbString &
SbString::operator =(const SbString &str)
{
    size_t size = (str.string == NULL) ? 0 : strlen(str.string) + 1;

    // If the string we are assigning to this is a pointer into the
    // string already in this, we have to make sure we don't step on
    // the old string.
    if (str.string >= string &&
            str.string < string + (string != staticStorage ? storageSize :
                            SB_STRING_STATIC_STORAGE_SIZE)) {

        SbString tmp = str;
        *this = tmp;
        return *this;
    }

    // If there's enough room in the static storage, use it. First,
    // free up string if it was allocated.
    if (size < SB_STRING_STATIC_STORAGE_SIZE) {
        if (string != staticStorage)
            makeEmpty();
    }

    // If we were using the static storage, we have to allocate a new string
    else if (string == staticStorage)
        string = new char[size];

    // Otherwise, if there is not enough room in the currently
    // allocated string, allocate a new one. If there is, use it again.
    else if (size > storageSize) {
        delete [] string;
        string = new char[size];
    }

    // Copy away!
    strncpy(string, str.string, size);
    storageSize = size;
    return *this;
}

//
// Concatenation operator "+=" for SbString
//

SbString &
SbString::operator +=(const SbString &str)
{
    expand(strlen(str.string));
    strcat(string, str.string);
    return *this;
}

//
// Equality operator for SbString/char* and SbString/SbString comparison
//

bool
operator ==(const SbString &str1, const SbString &str2)
{
    if (str2.string == NULL)
        return ((str1.getLength() == 0) ? 1 : 0);

    return (str1.string[0] == str2.string[0] && ! strcmp(str1.string, str2.string));
}

//
// Creates a string from ISO-8859-1.
//
SbString
SbString::fromLatin1(const char *latin1, int size)
{
    SbString str;

    if (latin1) {
        size_t len = (size > 0) ? size : 2 * strlen(latin1);
        str.expand(len);

        for (size_t i=0; i<len;) {
            unsigned char c = latin1[i];
            if (c < 0x80) {
                str.string[i++] = c;
            } else {
                str.string[i++] = 0xC0 | (c >> 6);
                str.string[i++] = 0x80 | (c & 0x3F);
            }
        }
        str.string[len] = '\0';
    }
    return str;
}

//
// Creates a string from UTF-8.
//
SbString
SbString::fromUtf8(const char *utf8, int size)
{
    SbString str;
    if (utf8) {
        size_t len = (size > 0) ? size : strlen(utf8);
        str.expand(len);
        strncat(str.string, utf8, len);
    }
    return str;
}

//
// Creates a string from UTF-16 (wide character).
//
SbString
SbString::fromWideChar(const wchar_t *wcs, int size)
{
    SbString str;

#ifdef SB_OS_WIN
    size_t len = (size == -1) ? WideCharToMultiByte( CP_UTF8, 0, wcs, -1, NULL, 0,  NULL, NULL)-1 : size;
    str.expand(len);

    WideCharToMultiByte(CP_UTF8, 0, wcs, -1, str.string, len, NULL, NULL);
#else
    setlocale(LC_CTYPE, "en_US.UTF-8");
    size_t len = (size == -1) ? wcstombs(NULL, wcs, 0) : size;
    str.expand(len);
    wcstombs(str.string, wcs, len+1);
#endif

    return str;
}
