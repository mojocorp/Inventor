#include <Inventor/errors/SoReadError.h>
#include <Inventor/fields/SoSFBox3f.h>

//////////////////////////////////////////////////////////////////////////////
//
// SoSFBox3f class
//
//////////////////////////////////////////////////////////////////////////////

// Use standard definitions of all basic methods
SO_SFIELD_SOURCE(SoSFBox3f, SbBox3f, const SbBox3f &);

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Initializes the SoSFBox3f class.
//
// Use: internal

void
SoSFBox3f::initClass()
//
////////////////////////////////////////////////////////////////////////
{
    SO__FIELD_INIT_CLASS(SoSFBox3f, "SFBox3f", SoSField);
}

void
SoSFBox3f::setValue(float xmin, float ymin, float zmin,
                    float xmax, float ymax, float zmax)
{
    setValue(SbBox3f(xmin, ymin, zmin, xmax, ymax, zmax));
}

void
SoSFBox3f::setValue(const SbVec3f & minvec, const SbVec3f & maxvec)
{
    setValue(SbBox3f(minvec, maxvec));
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Reads value from file. Returns FALSE on error.
//
// Use: private

SbBool
SoSFBox3f::readValue(SoInput *in)
//
////////////////////////////////////////////////////////////////////////
{
    SbVec3f minvec, maxvec;
    if (!in->read(minvec[0]) ||
        !in->read(minvec[1]) ||
        !in->read(minvec[2]) ||
        !in->read(maxvec[0]) ||
        !in->read(maxvec[1]) ||
        !in->read(maxvec[2]))
        return FALSE;

    this->setValue(SbBox3f(minvec, maxvec));

    return TRUE;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Writes value of field to file.
//
// Use: private

void
SoSFBox3f::writeValue(SoOutput *out) const
//
////////////////////////////////////////////////////////////////////////
{
    const SbBox3f & box = this->getValue();

    out->write(box.getMin()[0]);
    if (! out->isBinary()) out->write(' ');

    out->write(box.getMin()[1]);
    if (! out->isBinary()) out->write(' ');

    out->write(box.getMin()[2]);
    if (! out->isBinary()) out->write(' ');

    out->write(box.getMax()[0]);
    if (! out->isBinary()) out->write(' ');

    out->write(box.getMax()[1]);
    if (! out->isBinary()) out->write(' ');

    out->write(box.getMax()[2]);
    if (! out->isBinary()) out->write(' ');
}
