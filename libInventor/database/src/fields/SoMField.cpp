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
 |   $Revision: 1.3 $
 |
 |   Classes:
 |	SoMField
 |
 |   Author(s)		: Paul S. Strauss, Nick Thompson
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

#include <Inventor/fields/SoMField.h>
#include <Inventor/fields/SoSubField.h>
#include <Inventor/errors/SoReadError.h>

#include <stdlib.h>

// Special characters in files
#define OPEN_BRACE_CHAR		'['
#define CLOSE_BRACE_CHAR	']'
#define VALUE_SEPARATOR_CHAR	','
#define IGNORE_CHAR		'~'
#define CONNECTION_CHAR		'='
#define FIELD_SEP_CHAR		'.'

// Amount of values to allocate at a time when reading in multiple values.
#define VALUE_CHUNK_SIZE	32

//////////////////////////////////////////////////////////////////////////////
//
//  Class: SoMField
//
//	Multiple-valued fields
//
//////////////////////////////////////////////////////////////////////////////

char	*SoMField::fieldBuf;		// Used by SoMField::get1()
size_t	SoMField::fieldBufSize;		// Used by SoMField::get1()
SoType	SoMField::classTypeId;

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Initializes the SoMField class.
//
// Use: internal

void
SoMField::initClass()
//
////////////////////////////////////////////////////////////////////////
{
    SO__FIELD_INIT_CLASS(SoMField, "MField", SoField);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Destructor.
//
// Use: public

SoMField::~SoMField()
//
////////////////////////////////////////////////////////////////////////
{
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Use by SoType::createInstance; in this case, return NULL, since
//    this is an abstract type that has no instances...
//
// Use: public

void *
SoMField::createInstance()
//
////////////////////////////////////////////////////////////////////////
{
    return NULL;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Constructor
//
// Use: protected

SoMField::SoMField()
//
////////////////////////////////////////////////////////////////////////
{
    maxNum = num = 0;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Inserts or deletes values to adjust to the given size
//
// Use: public

void
SoMField::setNum(int n)
{
    if (n > num)
        insertSpace(num, n-num);
    else if (n < num)
        deleteValues(n);
}


////////////////////////////////////////////////////////////////////////
//
// Description:
//    Deletes num values starting at index start. A num of -1 (the default)
//    means delete all values after start, inclusive.
//
// Use: public

void
SoMField::deleteValues(int start,	// Starting index
                       int numToDelete)	// Number of values to delete
//
////////////////////////////////////////////////////////////////////////
{
    if (numToDelete < 0)
        numToDelete = getNum() - start;

    // Special case of deleting all values
    if (numToDelete == getNum())
        deleteAllValues();

    else {
        // Copy from the end of the array to the middle
        int lastToCopy = (getNum() - 1) - numToDelete;
        for (int i = start; i <= lastToCopy; i++)
            copyValue(i, i + numToDelete);

        // Truncate the array
        makeRoom(getNum() - numToDelete);
    }

    // The field value has changed...
    valueChanged();
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Inserts space for num values starting at start.  The initial
//    values in the new space are undefined.
//
// Use: public

void
SoMField::insertSpace(int start,	// Starting index
                      int numToInsert)		// Number of spaces to insert
//
////////////////////////////////////////////////////////////////////////
{
    // Expand the array
    makeRoom(getNum() + numToInsert);

    // Copy stuff out of the inserted area to later in the array
    for (int i = num - 1; i >= start + numToInsert; --i)
        copyValue(i, i - numToInsert);

    // The field value has changed...
    valueChanged();
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    This is equivalent to the SoField::set() method, but operates on
//    only the value given by the index.
//
// Use: public

SbBool
SoMField::set1(int index, const char *valueString)
//
////////////////////////////////////////////////////////////////////////
{
    SoInput	in;
    in.setBuffer((void *) valueString, strlen(valueString));

    if (read1Value(&in, index)) {

        // We have to do this here because read1Value() doesn't
        // indicate that values have changed, since it's usually used
        // in a larger reading context.
        valueChanged();

        return TRUE;
    }
    return FALSE;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    This is equivalent to the SoField::get() method, but operates on
//    only the value given by the index.
//
// Use: public

void
SoMField::get1(int index, SbString &valueString)
//
////////////////////////////////////////////////////////////////////////
{
    SoOutput	out;

    // Make sure the field value is up to date
    evaluate();

    // Prepare a character buffer and SoOutput for writing field strings
    if (fieldBufSize == 0) {
        fieldBufSize = 1028;
        fieldBuf = (char *) malloc((unsigned) fieldBufSize);
    }

    // Set up output into a string buffer
    out.setBuffer((void *) fieldBuf, fieldBufSize, &SoMField::reallocFieldBuf);

    // Make sure that the file header and lots of white space will NOT
    // be written into the string
    out.setCompact(TRUE);

    // Write the data
    write1Value(&out, index);

    // Make sure the string is terminated
    out.reset();

    // Store the result in the passed SbString
    valueString = fieldBuf;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    This makes sure there is enough memory allocated to hold
//    "newNum" values. It reallocates the values if necessary.
//
// Use: protected

void
SoMField::makeRoom(int newNum)		// New number of values
//
////////////////////////////////////////////////////////////////////////
{
    // Allocate room to hold all values if necessary
    if (newNum != num) {
        allocValues(newNum);
    }
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Reads all field values from file. Works for ASCII and binary
//    output. Binary values are read as a chunk.
//
// Use: private

SbBool
SoMField::readValue(SoInput *in)
//
////////////////////////////////////////////////////////////////////////
{
    if (in->isBinary()) {
        int	numToRead;

        // Read number of values
        if (! in->read(numToRead)) {
            SoReadError::post(in, "Couldn't read number of binary values "
                              "in multiple-value field");
            return FALSE;
        }

        // Make space for values; also sets number of values
        makeRoom(numToRead);

        // Read values
        if (! readBinaryValues(in, numToRead))
            return FALSE;
    }

    else {
        char	c;

        // Check for multiple field values
        if (in->read(c) && c == OPEN_BRACE_CHAR) {
            int curIndex = 0;
            // Check for no values: just an open and close brace
            if (in->read(c) && c == CLOSE_BRACE_CHAR)
                ;					// Do nothing now

            else {
                in->putBack(c);
                while (TRUE) {

                    // Make some room at end if necessary
                    if (curIndex >= getNum())
                        makeRoom(getNum() + VALUE_CHUNK_SIZE);

                    if (! read1Value(in, curIndex++) || ! in->read(c)) {
                        SoReadError::post(in,
                                          "Couldn't read value %d of field",
                                          curIndex);
                        return FALSE;
                    }

                    if (c == VALUE_SEPARATOR_CHAR) {

                        // See if this is a trailing separator (right before
                        // the closing brace). This is legal, but ignored.

                        if (in->read(c)) {
                            if (c == CLOSE_BRACE_CHAR)
                                break;
                            else
                                in->putBack(c);
                        }
                    }

                    else if (c == CLOSE_BRACE_CHAR)
                        break;

                    else {
                        SoReadError::post(in,
                                          "Expected '%c' or '%c' but got "
                                          "'%c' while reading value %d",
                                          VALUE_SEPARATOR_CHAR,
                                          CLOSE_BRACE_CHAR, c,
                                          curIndex);
                        return FALSE;
                    }
                }
            }

            // If extra space left over, nuke it
            if (curIndex < getNum())
                makeRoom(curIndex);
        }

        else {
            // Try reading 1 value
            in->putBack(c);
            makeRoom(1);
            if (! read1Value(in, 0))
                return FALSE;
        }
    }

    return TRUE;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Writes all values of field to file. Works for ASCII and binary
//    output. Binary output writes out the number of values and then
//    the values as one big chunk o'memory.
//
// Use: private

void
SoMField::writeValue(SoOutput *out) const
//
////////////////////////////////////////////////////////////////////////
{
    if (out->isBinary()) {
        out->write(num);
        writeBinaryValues(out);
    }
    else {
        if (num == 1)
            write1Value(out, 0);

        else {
            int	numOnLine = 0, maxOnLine = getNumValuesPerLine();

            out->write(OPEN_BRACE_CHAR);
            out->write(' ');

            out->incrementIndent(4);

            for (int i = 0; i < num; i++) {

                write1Value(out, i);

                if (i < num - 1) {
                    out->write(VALUE_SEPARATOR_CHAR);
                    if (++numOnLine == maxOnLine && ! out->isCompact()) {
                        out->write('\n');
                        out->indent();
                        out->write(' ');
                        out->write(' ');
                        numOnLine = 0;
                    }
                    else
                        out->write(' ');
                }
            }

            out->write(' ');
            out->write(CLOSE_BRACE_CHAR);

            out->decrementIndent(4);
        }
    }
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Writes array of binary values to file as one chunk.
//
// Use: private

void
SoMField::writeBinaryValues(SoOutput *out) const // Defines writing action

//
////////////////////////////////////////////////////////////////////////
{
    int	i;

    for (i = 0; i < num; i++)
        write1Value(out, i);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Reads array of binary values from file as one chunk.
//
// Use: private

SbBool
SoMField::readBinaryValues(SoInput *in,    // Reading specification
                           int numToRead)  // Number of values to read
//
////////////////////////////////////////////////////////////////////////
{
    int	i;

    for (i = 0; i < numToRead; i++)
        if (! read1Value(in, i))
            return FALSE;

    return TRUE;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Returns number of ASCII values to write per output line. This
//    defines the default to be 1, just to play it safe.
//
// Use: private

int
SoMField::getNumValuesPerLine() const
//
////////////////////////////////////////////////////////////////////////
{
    return 1;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    This is a callback function passed to SoOutput::setBuffer() by
//    SoMField::get1(). It is used to reallocate the buffer in which the
//    returned field info string is stored. Since we need to keep
//    track of the pointer to the buffer and its size, we can't just
//    call realloc() instead.
//
// Use: private, static

void *
SoMField::reallocFieldBuf(void *ptr, size_t newSize)
//
////////////////////////////////////////////////////////////////////////
{
    fieldBuf = (char *) realloc(ptr, newSize);
    fieldBufSize = newSize;

    return fieldBuf;
}
