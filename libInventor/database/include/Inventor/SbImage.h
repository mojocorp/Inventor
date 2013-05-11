#ifndef _SB_IMAGE_
#define _SB_IMAGE_

#include <Inventor/misc/SoGL.h>

#include <Inventor/SbBasic.h>
#include <Inventor/SbString.h>
#include <Inventor/SbVec2s.h>
#include <Inventor/SbVec3s.h>
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
    SbImage(const SbImage&);
    SbImage(const SbVec2s &size, Format fmt, const unsigned char *bytes);
    SbImage(const SbVec3s &size, Format fmt, const unsigned char *bytes);
    SbImage(const SbString & filename);

    /// Destructor.
    virtual ~SbImage();

    /// Returns the size of the image.
    const SbVec3s & getSize() const;

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
    /// Format | Description                      | Support
    /// -------|----------------------------------|-----------
    /// RGB    | Silicon Graphics Image (SGI)     | Read
    /// GIF    | Graphic Interchange Format       | Read
    /// JPG    | Joint Photographic Experts Group | Read
    /// PNG    | Portable Network Graphics        | Read
    /// TGA    | Truevision Targa                 | Read
    bool load(const SbString & filename);

    void setValue(const SbVec2s & size, Format fmt, const unsigned char *bytes);
    void setValue(const SbVec3s & size, Format fmt, const unsigned char *bytes);

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

    SbRefPtr<class SbImageRef> d;
};

#endif /* _SB_IMAGE_ */
