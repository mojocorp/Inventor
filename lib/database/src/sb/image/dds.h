#pragma once

#include <Inventor/SbImage.h>
#include <Inventor/SbString.h>

bool ReadDDSImage(const SbString &filename, SbImage &image);

