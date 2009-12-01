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
 |   $Revision: 1.1 $
 |
 |   Classes:
 |	SoMFVec4i32
 |
 |   Author(s)		: Paul S. Strauss
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

#include <Inventor/fields/SoMFVec4i32.h>
#include <stdlib.h>

//////////////////////////////////////////////////////////////////////////////
//
// SoMFVec4i32 class
//
//////////////////////////////////////////////////////////////////////////////

// Use standard definitions of all basic methods
SO_MFIELD_SOURCE_MALLOC(SoMFVec4i32, SbVec4i32, const SbVec4i32 &);
SO_MFIELD_SETVALUESPOINTER_SOURCE(SoMFVec4i32, SbVec4i32, SbVec4i32);
SO_MFIELD_SETVALUESPOINTER_SOURCE(SoMFVec4i32, SbVec4i32, int32_t);

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Initializes the SoMFVec4i32 class.
//
// Use: internal

void
SoMFVec4i32::initClass()
//
////////////////////////////////////////////////////////////////////////
{
    SO__FIELD_INIT_CLASS(SoMFVec4i32, "MFVec4f", SoMField);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Sets values from array of arrays of 4 floats. This can be useful
//    in some applications that have vectors stored in this manner and
//    want to keep them that way for efficiency.
//
// Use: public

void
SoMFVec4i32::setValues(int start,			// Starting index
		     int num,			// Number of values to set
		     const int32_t xyzw[][4])	// Array of vector values
//
////////////////////////////////////////////////////////////////////////
{
    int	newNum = start + num;
    int	i;

    if (newNum > getNum())
	makeRoom(newNum);

    for (i = 0; i < num; i++)
	values[start + i].setValue(xyzw[i]);

    valueChanged();
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Sets one vector value from 4 separate floats. (Convenience function)
//
// Use: public

void
SoMFVec4i32::set1Value(int index, int32_t x, int32_t y, int32_t z, int32_t w)
//
////////////////////////////////////////////////////////////////////////
{
    set1Value(index, SbVec4i32(x, y, z, w));
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Sets one vector value from an array of 4 floats. (Convenience function)
//
// Use: public

void
SoMFVec4i32::set1Value(int index, const int32_t xyzw[4])
//
////////////////////////////////////////////////////////////////////////
{
    set1Value(index, SbVec4i32(xyzw));
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Sets to one vector value from 4 separate floats. (Convenience function)
//
// Use: public

void
SoMFVec4i32::setValue(int32_t x, int32_t y, int32_t z, int32_t w)
//
////////////////////////////////////////////////////////////////////////
{
    setValue(SbVec4i32(x, y, z, w));
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Sets to one vector value from array of 4 floats. (Convenience function)
//
// Use: public

void
SoMFVec4i32::setValue(const int32_t xyzw[4])
//
////////////////////////////////////////////////////////////////////////
{
    setValue(SbVec4i32(xyzw));
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Reads one (indexed) value from file. Returns FALSE on error.
//
// Use: private

SbBool
SoMFVec4i32::read1Value(SoInput *in, int index)
//
////////////////////////////////////////////////////////////////////////
{
    return (in->read(values[index][0]) &&
	    in->read(values[index][1]) &&
	    in->read(values[index][2]) &&
	    in->read(values[index][3]));
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Writes one (indexed) value to file.
//
// Use: private

void
SoMFVec4i32::write1Value(SoOutput *out, int index) const
//
////////////////////////////////////////////////////////////////////////
{
    out->write(values[index][0]);

    if (! out->isBinary())
	out->write(' ');

    out->write(values[index][1]);

    if (! out->isBinary())
	out->write(' ');

    out->write(values[index][2]);

    if (! out->isBinary())
	out->write(' ');

    out->write(values[index][3]);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Writes array of binary values to file as one chunk.
//
// Use: private

void
SoMFVec4i32::writeBinaryValues(SoOutput *out) const

//
////////////////////////////////////////////////////////////////////////
{
    out->writeBinaryArray((int32_t *) values, 4 * num);
}


////////////////////////////////////////////////////////////////////////
//
// Description:
//    Reads array of binary values from file as one chunk.
//
// Use: private

SbBool
SoMFVec4i32::readBinaryValues(SoInput *in, int numToRead)
//
////////////////////////////////////////////////////////////////////////
{
    return (in->readBinaryArray((int32_t *) values, 4 * numToRead));
}
