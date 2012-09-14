#include <Inventor/SbImage.h>
#include <Inventor/SoInput.h>

#include <image.h>
#include <stdlib.h>

SbImage::SbImageRef::SbImageRef()
    : size(0,0), format(Format_Invalid), bytes(0)
{

}

SbImage::SbImageRef::SbImageRef(const SbImageRef * other)
   : size(0,0), format(Format_Invalid), bytes(0)
{
    setValue(other->size, other->format, other->bytes);
}

SbImage::SbImageRef::SbImageRef(const SbVec2s &_size, Format _format, const unsigned char *_bytes)
    : size(0,0), format(Format_Invalid), bytes(0)
{
    setValue(_size, _format, _bytes);
}

SbImage::SbImageRef::~SbImageRef()
{
    dispose();
}

void SbImage::SbImageRef::setValue(const SbVec2s &_size,
                                   Format _format,
                                   const unsigned char *_bytes)
{
    if (bytes && (bytes == _bytes)) {
        return;
    }

    dispose();

    size = _size;
    format = _format;

    size_t numBytes = size[0]*size[1]*getNumComponents();

    if (numBytes != 0) {
        bytes = new unsigned char[numBytes];
        if (_bytes)
            memcpy(bytes, _bytes, numBytes);
    }
}

bool SbImage::SbImageRef::read(const SbString & filename)
{
    dispose();

    int w,h,nc;
    w = h = 0;

    if (ReadImage(filename.getString(), w, h, nc, bytes)) {
        size = SbVec2s(w,h);
        switch(nc){
        case 1: format = Format_Luminance; break;
        case 2: format = Format_Luminance_Alpha; break;
        case 3: format = Format_RGB24; break;
        case 4: format = Format_RGBA32; break;
        default: break;
        }
        return true;
    }
    return false;
}

bool SbImage::SbImageRef::isNull() const
{
    return (size[0] == 0 || size[1] == 0 || format == Format_Invalid);
}

int SbImage::SbImageRef::getNumComponents() const
{
    switch(format) {
    case Format_Luminance:       return 1;
    case Format_Luminance_Alpha: return 2;
    case Format_RGB24:           return 3;
    case Format_RGBA32:          return 4;
    default:                     return 0;
    }
}

bool SbImage::SbImageRef::hasAlphaChannel() const
{
    return (format == Format_Luminance_Alpha || format == Format_RGBA32);
}

bool SbImage::SbImageRef::operator ==(const SbImageRef &other) const
{
    // Check easy stuff first
    if (size != other.size || format != other.format)
        return false;

    if (memcmp(bytes, other.bytes, size[0] * size[1] * getNumComponents()) != 0)
        return false;

    return true;
}

void SbImage::SbImageRef::dispose()
{
    size.setValue(0,0);
    format = Format_Invalid;
    delete [] bytes;
    bytes = NULL;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//
//
// Use: public

SbImage::SbImage()
    : d(0)
//
////////////////////////////////////////////////////////////////////////
{

}

SbImage::SbImage(const SbVec2s &size, Format fmt)
    : d(new SbImageRef(size, fmt))
{

}

SbImage::SbImage(const SbVec2s &size, Format fmt, const unsigned char *bytes)
    : d(new SbImageRef(size, fmt, bytes))
{

}

SbImage::SbImage(const SbString & filename)
    : d(0)
{
    load(filename);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//
//
// Use: public

SbImage::~SbImage()
//
////////////////////////////////////////////////////////////////////////
{

}

////////////////////////////////////////////////////////////////////////
//
// Description:
//
//
// Use: public

const SbVec2s&
SbImage::getSize() const
//
////////////////////////////////////////////////////////////////////////
{
    static SbVec2s dummy(0, 0);
    return d ? d->size : dummy;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//
//
// Use: public

SbImage::Format
SbImage::getFormat() const
//
////////////////////////////////////////////////////////////////////////
{
    return d ? d->format : Format_Invalid;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//
//
// Use: public

int
SbImage::getNumComponents() const
//
////////////////////////////////////////////////////////////////////////
{
    return d ? d->getNumComponents() : 0;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//
//
// Use: public

const unsigned char *
SbImage::getConstBytes() const
//
////////////////////////////////////////////////////////////////////////
{
    return d ? d->bytes : 0;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//
//
// Use: public

unsigned char *
SbImage::getBytes()
//
////////////////////////////////////////////////////////////////////////
{
    detach();

    return d ? d->bytes : 0;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//
//
// Use: public

bool
SbImage::isNull() const
//
////////////////////////////////////////////////////////////////////////
{
    return d ? d->isNull() : true;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//
//
// Use: public

bool
SbImage::load(const SbString & fname)
//
////////////////////////////////////////////////////////////////////////
{
    SoInput in;
    if (!in.openFile(fname.getString(), TRUE)) {
        return false;
    }

    SbString filename = in.getCurFileName();

    in.closeFile();

    detach();

    if (!d)
        d = new SbImageRef;

    return d->read(filename);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//
//
// Use: public

void
SbImage::setValue(const SbVec2s &size,
                  Format fmt,
                  const unsigned char *bytes)
//
////////////////////////////////////////////////////////////////////////
{
    detach();

    if (!d)
        d = new SbImageRef;

    d->setValue(size, fmt, bytes);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//
//
// Use: public

const SbImage &
SbImage::operator =(const SbImage &other)
//
////////////////////////////////////////////////////////////////////////
{
    d = other.d;

    return *this;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Returns TRUE if image has same value as given image.
//
// Use: public

bool
SbImage::operator ==(const SbImage &other) const
//
////////////////////////////////////////////////////////////////////////
{
    return (d == other.d);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//
//
// Use: public

bool
SbImage::hasAlphaChannel() const
//
////////////////////////////////////////////////////////////////////////
{
    return d ? d->hasAlphaChannel() : false;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//
//
// Use: public

void
SbImage::detach()
//
////////////////////////////////////////////////////////////////////////
{
    if (d && d->getRefCount() > 1) {
        d = new SbImageRef(d.data());
    }
}

