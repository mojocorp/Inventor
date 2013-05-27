#include <Inventor/SbImage.h>
#include <Inventor/SoInput.h>

#include "image/image.h"
#include <vector>

class SbImageRef : public SbRefCounted
{
public:
    SbImageRef();
    SbImageRef(const SbImageRef * other);
    SbImageRef(const SbVec3s & size, SbImage::Format format, size_t numBytes, const unsigned char * bytes = NULL);

    void setValue(const SbVec3s & size, SbImage::Format format, size_t numBytes, const unsigned char * bytes);
    bool isNull() const;
    int getNumComponents() const;
    size_t getNumBytes() const;
    unsigned char* getBytes();
    bool hasAlphaChannel() const;

    bool operator ==(const SbImageRef &other) const;

    SbVec3s  size;                    // Width and height of image
    SbImage::Format format;           // Image format
private:
    ~SbImageRef();

     std::vector<unsigned char> bytes; // Array of pixels
};

SbImageRef::SbImageRef()
    : size(0,0,0), format(SbImage::Format_Invalid), bytes(0)
{

}

SbImageRef::SbImageRef(const SbImageRef * other)
   : size(0,0,0), format(SbImage::Format_Invalid)
{
    setValue(other->size, other->format, other->bytes.size(), &other->bytes[0]);
}

SbImageRef::SbImageRef(const SbVec3s &_size, SbImage::Format _format, size_t _numBytes, const unsigned char *_bytes)
    : size(0,0,0), format(SbImage::Format_Invalid)
{
    setValue(_size, _format, _numBytes, _bytes);
}

SbImageRef::~SbImageRef()
{

}

void SbImageRef::setValue(const SbVec3s &_size,
                          SbImage::Format _format,
                          size_t _numBytes,
                          const unsigned char *_bytes)
{
    if (!bytes.empty() && (&bytes[0] == _bytes)) {
        return;
    }

    size = _size;
    format = _format;
    bytes.resize(_numBytes);

    if (_bytes)
        memcpy(&bytes[0], _bytes, bytes.size());
}

bool SbImageRef::isNull() const
{
    return (size[0] == 0 || size[1] == 0 || format == SbImage::Format_Invalid);
}

int SbImageRef::getNumComponents() const
{
    switch(format) {
    case SbImage::Format_Luminance:       return 1;
    case SbImage::Format_Luminance_Alpha: return 2;
    case SbImage::Format_RGB24:           return 3;
    case SbImage::Format_RGBA32:          return 4;
    default:                     return 0;
    }
}

size_t SbImageRef::getNumBytes() const
{
    return bytes.size();
}

unsigned char* SbImageRef::getBytes()
{
    return (bytes.size() ? &bytes[0] : NULL);
}

bool SbImageRef::hasAlphaChannel() const
{
    return (format == SbImage::Format_Luminance_Alpha || format == SbImage::Format_RGBA32);
}

bool SbImageRef::operator ==(const SbImageRef &other) const
{
    // Check easy stuff first
    if (size != other.size || format != other.format)
        return false;

    if (memcmp(&bytes[0], &other.bytes[0], bytes.size()) != 0)
        return false;

    return true;
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

SbImage::SbImage(const SbImage & other)
    : d(other.d)
{

}

SbImage::SbImage(const SbVec2s &size, Format fmt, size_t numBytes, const unsigned char *bytes)
    : d(new SbImageRef(SbVec3s(size[0], size[1], 1), fmt, numBytes, bytes))
{

}

SbImage::SbImage(const SbVec3s &size, Format fmt, size_t numBytes, const unsigned char *bytes)
    : d(new SbImageRef(size, fmt, numBytes, bytes))
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

const SbVec3s&
SbImage::getSize() const
//
////////////////////////////////////////////////////////////////////////
{
    static SbVec3s dummy(0, 0, 0);
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

size_t
SbImage::getNumBytes() const
//
////////////////////////////////////////////////////////////////////////
{
    return d ? d->getNumBytes() : 0;
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
    return d ? d->getBytes() : 0;
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

    return d ? d->getBytes() : 0;
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

    return ReadImage(filename, *this);
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
                  size_t numBytes,
                  const unsigned char *bytes)
//
////////////////////////////////////////////////////////////////////////
{
    detach();

    if (!d)
        d = new SbImageRef;

    d->setValue(SbVec3s(size[0], size[1], 1), fmt, numBytes, bytes);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//
//
// Use: public

void
SbImage::setValue(const SbVec3s &size,
                  Format fmt,
                  size_t numBytes,
                  const unsigned char *bytes)
//
////////////////////////////////////////////////////////////////////////
{
    detach();

    if (!d)
        d = new SbImageRef;

    d->setValue(size, fmt, numBytes, bytes);
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
        d = new SbImageRef(d.get());
    }
}

