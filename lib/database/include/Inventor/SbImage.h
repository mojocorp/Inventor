#pragma once

#include <Inventor/SbBasic.h>
#include <Inventor/SbString.h>
#include <Inventor/SbVec.h>
#include <Inventor/SbRefPtr.h>

class INVENTOR_API SbImage {
  public:
    enum Format {
        Format_Invalid,
        Format_Luminance,
        Format_Luminance_Alpha,
        Format_RGB24,
        Format_RGBA32,

        Format_RGB_S3TC_DXT1,
        Format_RGBA_S3TC_DXT1,
        Format_RGBA_S3TC_DXT3,
        Format_RGBA_S3TC_DXT5
    };

    SbImage();
    SbImage(const SbImage &);
    SbImage(const SbVec2s &size, Format fmt, size_t numBytes,
            const unsigned char *bytes);
    SbImage(const SbVec3s &size, Format fmt, size_t numBytes,
            const unsigned char *bytes);
    SbImage(const SbString &filename);

    /// Destructor.
    virtual ~SbImage();

    /// Returns the size of the image.
    const SbVec3s &getSize() const;

    /// Return the internal format.
    Format getFormat() const;

    /// Returns the number of unsigned bytes the image contains.
    size_t getNumBytes() const;

    /// Returns the number of components.
    int getNumComponents() const;

    /// Returns the number of mipmaps contained in the image.
    size_t getNumMipmaps() const;

    /// Sets a mipmap of the image as the current mipmap. If level is 0, then
    /// the current base image is set.
    bool setActiveMipmap(unsigned int level);

    /// Returns a pointer to the first pixel data.
    const unsigned char *getConstBytes() const;

    /// Returns a pointer to the first pixel data.
    unsigned char *getBytes();

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
    /// DDS    | DirectDraw Surface (S3TC)        | Read
    bool load(const SbString &filename);

    void setValue(const SbVec2s &size, Format fmt, size_t numBytes,
                  const unsigned char *bytes);
    void setValue(const SbVec3s &size, Format fmt, size_t numBytes,
                  const unsigned char *bytes);

    /// Assigns a shallow copy of the given image to this image and returns a
    /// reference to this image.
    const SbImage &operator=(const SbImage &other);

    /// Equality operator
    bool operator==(const SbImage &other) const;

    /// Inequality operator
    bool operator!=(const SbImage &other) const { return !((*this) == other); }

    /// Returns true if the image has an alpha channel.
    bool hasAlphaChannel() const;

    /// Returns true if the image is compressed.
    bool isCompressed() const;

  private:
    void detach();

    SbRefPtr<class SbImageRef> d;
};

