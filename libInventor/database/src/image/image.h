#ifndef LIBIMAGE_H
#define LIBIMAGE_H

#include <Inventor/SbString.h>

/// Read JPG, GIF and SGI only
bool ReadImage(const SbString & filename, int &w, int &h, int &nc, unsigned char *&bytes);

#endif // LIBIMAGE
