#ifndef _SB_IMAGE_
#define _SB_IMAGE_

#include <Inventor/SbBasic.h>
#include <Inventor/SbString.h>
#include <Inventor/SbVec2s.h>
#include <Inventor/SbRefPtr.h>

class INVENTOR_API SbImage {
public:
    SbImage();
    SbImage(const SbVec2s &size, int numComponents);
    SbImage(const SbVec2s &size, int numComponents, const unsigned char *bytes);
    SbImage(const SbString & filename);

    /// Destructor.
    virtual ~SbImage();

    /// Returns the size of the image.
    const SbVec2s & getSize() const;

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
                  int numComponents,
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
        SbImageRef(const SbVec2s & size, int numComponents, const unsigned char * bytes = NULL);

        void setValue(const SbVec2s & size,
                         int  numComponents,
                         const unsigned char * bytes);
        bool isNull() const;
        bool hasAlphaChannel() const;
        bool read(const SbString & filename);

        bool operator ==(const SbImageRef &other) const;
        void dispose();

        SbVec2s  size;          // Width and height of image
        int   numComponents;    // Number of components per pixel
        unsigned char * bytes;  // Array of pixels
    private:
        ~SbImageRef();
    };

    SbRefPtr<SbImageRef> d;
};

#endif /* _SB_IMAGE_ */
