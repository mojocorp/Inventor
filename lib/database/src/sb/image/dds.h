#ifndef DDS_H
#define DDS_H

#include <Inventor/SbImage.h>
#include <Inventor/SbString.h>

bool ReadDDSImage(const SbString &filename, SbImage &image);

#endif
