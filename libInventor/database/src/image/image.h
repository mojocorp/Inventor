#ifndef LIBIMAGE_H
#define LIBIMAGE_H

#include <Inventor/SbString.h>
#include <Inventor/SbImage.h>

/// Read JPG, GIF and SGI only
bool ReadImage(const SbString & filename, SbImage & image);

#endif // LIBIMAGE
