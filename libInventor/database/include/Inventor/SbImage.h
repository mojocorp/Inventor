#ifndef _SB_IMAGE_
#define _SB_IMAGE_

#include <Inventor/misc/SoGL.h>

#include <Inventor/SbBasic.h>
#include <Inventor/SbString.h>
#include <Inventor/SbVec2s.h>
#include <Inventor/SbRefPtr.h>

class INVENTOR_API SbImage {
public:
    enum Format
    {
        Format_Invalid,
        Format_Luminance       = GL_LUMINANCE,
        Format_Luminance_Alpha = GL_LUMINANCE_ALPHA,
        Format_RGB24           = GL_RGB,
        Format_RGBA32          = GL_RGBA
    };

    SbImage();
    SbImage(const SbVec2s &size, Format fmt);
    SbImage(const SbVec2s &size, Format fmt, const unsigned char *bytes);
    SbImage(const SbString & filename);

    /// Destructor.
    virtual ~SbImage();

    /// Returns the size of the image.
    const SbVec2s & getSize() const;

    /// Return the internal format.
    Format getFormat() const;

    /// Returns the number of components.
    int getNumComponents() const;

    /// Returns a pointer to the first pixel data.
    const unsigned char * getConstBytes() const;

    /// Returns a pointer to the first pixel data.
    unsigned char * getBytes();

    /// Returns true if it is a null image, otherwise returns false.
    bool isNull() const;

    /// Loads an image from the file with the given filename.
    bool load(const SbString & filename);

    void setValue(const SbVec2s & size,
                  Format fmt,
                  const unsigned char *bytes);

    /// Assigns a shallow copy of the given image to this image and returns a reference to this image.
    const SbImage & operator =(const SbImage &other);

    /// Equality operator
    bool operator ==(const SbImage &other) const;

    /// Inequality operator
    bool operator !=(const SbImage &other) const {
        return ! ((*this) == other);
    }

    /// Returns true if the image has an alpha channel.
    bool hasAlphaChannel() const;
private:
    void detach();

    class SbImageRef : public SbRefCounted
    {
    public:
        SbImageRef();
        SbImageRef(const SbImageRef * other);
        SbImageRef(const SbVec2s & size, Format format, const unsigned char * bytes = NULL);

        void setValue(const SbVec2s & size,
                      Format format,
                      const unsigned char * bytes);
        bool isNull() const;
        int getNumComponents() const;
        bool hasAlphaChannel() const;
        bool read(const SbString & filename);

        bool operator ==(const SbImageRef &other) const;
        void dispose();

        SbVec2s  size;          // Width and height of image
        Format format;          // Image format
        unsigned char * bytes;  // Array of pixels
    private:

        ~SbImageRef();
    };

    SbRefPtr<SbImageRef> d;
};

#endif /* _SB_IMAGE_ */
