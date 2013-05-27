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

    const SbVec3s & getSize() const;
    bool setActiveMipmap(int level);
    void setValue(const SbVec3s & size, SbImage::Format format, size_t numBytes, const unsigned char * bytes);
    bool isNull() const;
    int getNumComponents() const;
    size_t getNumBytes() const;
    unsigned char* getBytes();
    size_t getNumMipmaps() const;
    bool hasAlphaChannel() const;
    bool isCompressed() const;

    bool operator ==(const SbImageRef &other) const;

    SbImage::Format format;           // Image format

private:
    struct SubImage {
        SbVec3s  size;                     // Width and height of image
        std::vector<unsigned char> bytes;  // Array of pixels
    };
    size_t level;

    std::vector<SubImage> mipmaps;

    ~SbImageRef();
};

SbImageRef::SbImageRef()
    : format(SbImage::Format_Invalid), level(0)
{
    setActiveMipmap(0);
}

SbImageRef::SbImageRef(const SbImageRef * other)
   : format(SbImage::Format_Invalid), level(0)
{
    setActiveMipmap(0);
    setValue(other->mipmaps[level].size, other->format, other->mipmaps[level].bytes.size(), &other->mipmaps[level].bytes[0]);
}

SbImageRef::SbImageRef(const SbVec3s &_size, SbImage::Format _format, size_t _numBytes, const unsigned char *_bytes)
    : format(SbImage::Format_Invalid), level(0)
{
    setActiveMipmap(0);
    setValue(_size, _format, _numBytes, _bytes);
}

SbImageRef::~SbImageRef()
{

}

const SbVec3s &
SbImageRef::getSize() const
{
    return mipmaps[level].size;
}

size_t
SbImageRef::getNumMipmaps() const
{
    return mipmaps.size();
}

bool
SbImageRef::setActiveMipmap(int _level)
{
    level = _level;
    if (mipmaps.size() < level + 1) {
        mipmaps.resize(level + 1);
    }
    return true;
}

void SbImageRef::setValue(const SbVec3s &_size,
                          SbImage::Format _format,
                          size_t _numBytes,
                          const unsigned char *_bytes)
{
    if (mipmaps[level].bytes.size() && (&mipmaps[level].bytes[0] == _bytes)) {
        return;
    }

    format = _format;
    mipmaps[level].size = _size;
    mipmaps[level].bytes.resize(_numBytes);

    if (_bytes) {
        memcpy(&mipmaps[level].bytes[0], _bytes, mipmaps[level].bytes.size());
    }
}

bool SbImageRef::isNull() const
{
    return (mipmaps[level].size[0] == 0 || mipmaps[level].size[1] == 0 || format == SbImage::Format_Invalid);
}

int SbImageRef::getNumComponents() const
{
    switch(format) {
    case SbImage::Format_Luminance:       return 1;
    case SbImage::Format_Luminance_Alpha: return 2;
    case SbImage::Format_RGB24:           return 3;
    case SbImage::Format_RGBA32:          return 4;
    case SbImage::Format_RGB_S3TC_DXT1:   return 3;
    case SbImage::Format_RGBA_S3TC_DXT1:  return 4;
    case SbImage::Format_RGBA_S3TC_DXT3:  return 4;
    case SbImage::Format_RGBA_S3TC_DXT5:  return 4;
    default:                              return 0;
    }
}

unsigned char * SbImageRef::getBytes()
{
    return (mipmaps[level].bytes.size() ? &mipmaps[level].bytes[0] : NULL);
}

size_t SbImageRef::getNumBytes() const
{
    return mipmaps[level].bytes.size();
}

bool SbImageRef::hasAlphaChannel() const
{
    switch(format) {
    case SbImage::Format_Luminance:       return false;
    case SbImage::Format_Luminance_Alpha: return true;
    case SbImage::Format_RGB24:           return false;
    case SbImage::Format_RGBA32:          return true;
    case SbImage::Format_RGB_S3TC_DXT1:   return false;
    case SbImage::Format_RGBA_S3TC_DXT1:  return true;
    case SbImage::Format_RGBA_S3TC_DXT3:  return true;
    case SbImage::Format_RGBA_S3TC_DXT5:  return true;
    default:                              return false;
    }
}

bool SbImageRef::isCompressed() const
{
    switch(format) {
    case SbImage::Format_Luminance:       return false;
    case SbImage::Format_Luminance_Alpha: return false;
    case SbImage::Format_RGB24:           return false;
    case SbImage::Format_RGBA32:          return false;
    case SbImage::Format_RGB_S3TC_DXT1:   return true;
    case SbImage::Format_RGBA_S3TC_DXT1:  return true;
    case SbImage::Format_RGBA_S3TC_DXT3:  return true;
    case SbImage::Format_RGBA_S3TC_DXT5:  return true;
    default:                              return false;
    }
}

bool SbImageRef::operator ==(const SbImageRef &other) const
{
    // Check easy stuff first
    if (mipmaps.size() != other.mipmaps.size() || format != other.format)
        return false;

    for (size_t i=0; i<mipmaps.size(); i++) {
        if (mipmaps[i].size != other.mipmaps[i].size)
            return false;

        if (memcmp(&mipmaps[i].bytes[0], &other.mipmaps[i].bytes[0], mipmaps[i].bytes.size()) != 0)
            return false;
    }
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
    return d ? d->getSize() : dummy;
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

size_t
SbImage::getNumMipmaps() const
{
    return d ? d->getNumMipmaps() : 0;
}

bool
SbImage::setActiveMipmap(unsigned int level)
{
    return d ? d->setActiveMipmap(level) : false;
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
    SbString filename;
    if (!SoInput::findFile(fname, filename)) {
        return false;
    }

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

bool
SbImage::isCompressed() const
//
////////////////////////////////////////////////////////////////////////
{
    return d ? d->isCompressed() : false;
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

