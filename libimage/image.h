#ifndef LIBIMAGE_H
#define LIBIMAGE_H

/// Read JPG, GIF and SGI only
bool ReadImage(const char* filename, int &w, int &h, int &nc, unsigned char *&bytes);

#endif // LIBIMAGE
