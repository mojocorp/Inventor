#include <Inventor/fields/SoMFVec3i32.h>
#include <stdlib.h>

//////////////////////////////////////////////////////////////////////////////
//
// SoMFVec3i32 class
//
//////////////////////////////////////////////////////////////////////////////

// Use standard definitions of all basic methods
SO_MFIELD_SOURCE_MALLOC(SoMFVec3i32, SbVec3i32, const SbVec3i32 &);
SO_MFIELD_SETVALUESPOINTER_SOURCE(SoMFVec3i32, SbVec3i32, SbVec3i32);
SO_MFIELD_SETVALUESPOINTER_SOURCE(SoMFVec3i32, SbVec3i32, int32_t);

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Initializes the SoMFVec3i32 class.
//
// Use: internal

void
SoMFVec3i32::initClass()
//
////////////////////////////////////////////////////////////////////////
{
    SO__FIELD_INIT_CLASS(SoMFVec3i32, "MFVec3i32", SoMField);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Sets values from array of arrays of 3 int32_ts. This can be useful
//    in some applications that have vectors stored in this manner and
//    want to keep them that way for efficiency.
//
// Use: public

void
SoMFVec3i32::setValues(int start,			// Starting index
		     int num,			// Number of values to set
                     const int32_t xyz[][3])	// Array of vector values
//
////////////////////////////////////////////////////////////////////////
{
    int	newNum = start + num;
    int	i;

    if (newNum > getNum())
	makeRoom(newNum);

    for (i = 0; i < num; i++)
	values[start + i].setValue(xyz[i]);

    valueChanged();
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Sets one vector value from 3 separate int32_ts. (Convenience function)
//
// Use: public

void
SoMFVec3i32::set1Value(int index, int32_t x, int32_t y, int32_t z)
//
////////////////////////////////////////////////////////////////////////
{
    set1Value(index, SbVec3i32(x, y, z));
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Sets one vector value from an array of 3 int32_ts. (Convenience function)
//
// Use: public

void
SoMFVec3i32::set1Value(int index, const int32_t xyz[3])
//
////////////////////////////////////////////////////////////////////////
{
    set1Value(index, SbVec3i32(xyz));
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Sets to one vector value from 3 separate int32_ts. (Convenience function)
//
// Use: public

void
SoMFVec3i32::setValue(int32_t x, int32_t y, int32_t z)
//
////////////////////////////////////////////////////////////////////////
{
    setValue(SbVec3i32(x, y, z));
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Sets to one vector value from array of 3 int32_ts. (Convenience function)
//
// Use: public

void
SoMFVec3i32::setValue(const int32_t xyz[3])
//
////////////////////////////////////////////////////////////////////////
{
    setValue(SbVec3i32(xyz));
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Reads one (indexed) value from file. Returns FALSE on error.
//
// Use: private

SbBool
SoMFVec3i32::read1Value(SoInput *in, int index)
//
////////////////////////////////////////////////////////////////////////
{
    return (in->read(values[index][0]) &&
	    in->read(values[index][1]) &&
	    in->read(values[index][2]));
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Writes one (indexed) value to file.
//
// Use: private

void
SoMFVec3i32::write1Value(SoOutput *out, int index) const
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
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Writes array of binary values to file as one chunk.
//
// Use: private

void
SoMFVec3i32::writeBinaryValues(SoOutput *out) const // Defines writing action

//
////////////////////////////////////////////////////////////////////////
{
    out->writeBinaryArray((int32_t *)values, 3*num);
}


////////////////////////////////////////////////////////////////////////
//
// Description:
//    Reads array of binary values from file as one chunk.
//
// Use: private

SbBool
SoMFVec3i32::readBinaryValues(SoInput *in,    // Reading specification
                      	    int numToRead)  // Number of values to read
//
////////////////////////////////////////////////////////////////////////
{
    return (in->readBinaryArray((int32_t *)values, 3*numToRead));
}



