#include <Inventor/SbImage.h>
#include <Inventor/SoInput.h>

#include <image.h>
#include <stdlib.h>

SbImage::SbImageRef::SbImageRef()
    : size(0,0), numComponents(0), bytes(0)
{

}

SbImage::SbImageRef::SbImageRef(const SbImageRef * other)
   : size(0,0), numComponents(0), bytes(0)
{
    setValue(other->size, other->numComponents, other->bytes);
}

SbImage::SbImageRef::SbImageRef(const SbVec2s &_size, int _numComponents, const unsigned char *_bytes)
    : size(0,0), numComponents(0), bytes(0)
{
    setValue(_size, _numComponents, _bytes);
}

SbImage::SbImageRef::~SbImageRef()
{
    dispose();
}

void SbImage::SbImageRef::setValue(const SbVec2s &_size,
                                   int _numComponents,
                                   const unsigned char *_bytes)
{
    if (bytes && (bytes == _bytes)) {
        return;
    }

    dispose();

    size = _size;
    numComponents = _numComponents;

    size_t numBytes = size[0]*size[1]*numComponents;

    if (numBytes != 0) {
        bytes = new unsigned char[numBytes];
        if (_bytes)
            memcpy(bytes, _bytes, numBytes);
    }
}

bool SbImage::SbImageRef::read(const SbString & filename)
{
    dispose();

    int w,h;
    w = h = 0;

    if (ReadImage(filename.getString(), w, h, numComponents, bytes)) {
        size = SbVec2s(w,h);
        return true;
    }
    return false;
}

bool SbImage::SbImageRef::isNull() const
{
    return (size[0] == 0 || size[1] == 0 || numComponents == 0);
}

bool SbImage::SbImageRef::hasAlphaChannel() const
{
    return (numComponents == 2 || numComponents == 4);
}

bool SbImage::SbImageRef::operator ==(const SbImageRef &other) const
{
    // Check easy stuff first
    if (size != other.size || numComponents != other.numComponents)
        return false;

    if (memcmp(bytes, other.bytes, size[0] * size[1] * numComponents) != 0)
        return false;

    return true;
}

void SbImage::SbImageRef::dispose()
{
    size.setValue(0,0);
    numComponents = 0;
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

SbImage::SbImage(const SbVec2s &size, int numComponents)
    : d(new SbImageRef(size, numComponents))
{

}

SbImage::SbImage(const SbVec2s &size, int numComponents, const unsigned char *bytes)
    : d(new SbImageRef(size, numComponents, bytes))
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

int
SbImage::getNumComponents() const
//
////////////////////////////////////////////////////////////////////////
{
    return d ? d->numComponents : 0;
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
                  int numComponents,
                  const unsigned char *bytes)
//
////////////////////////////////////////////////////////////////////////
{
    detach();

    if (!d)
        d = new SbImageRef;

    d->setValue(size, numComponents, bytes);
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

