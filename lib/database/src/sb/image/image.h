#ifndef LIBIMAGE_H
#define LIBIMAGE_H

#include <Inventor/SbString.h>
#include <Inventor/SbImage.h>

bool ReadImage(const SbString &filename, SbImage &image);

#endif // LIBIMAGE
