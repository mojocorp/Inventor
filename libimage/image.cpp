#include "image.h"

#include "image-sgi.h"

extern "C" unsigned char *stbi_load(char const *filename,     int *x, int *y, int *comp, int req_comp);
extern "C" void stbi_image_free(void *retval_from_stbi_load);

#include <stdlib.h>
#include <string>
#include <vector>

bool ReadSGIImage(const std::string & filename, int &w, int &h, int &nc, unsigned char *&bytes);

void stbi_flip_copy(int w, int h, int comp, const unsigned char *from, unsigned char *to)
{
   int stride = w * comp;
   const unsigned char *p1 = from;
   unsigned char *p2 = to + (h-1) * stride;

   for (int y=0; y < h; ++y) {
       memcpy(p2, p1, stride);

      p1 += stride;
      p2 -= stride;
   }
}

bool ReadImage(const char* filename, int &w, int &h, int &nc, unsigned char *&bytes)
{
    unsigned char *data = stbi_load(filename, &w, &h, &nc, 0);

    if (data) {
        bytes = new unsigned char[w*h*nc];
        stbi_flip_copy(w, h, nc, data, bytes);
        stbi_image_free(data);
        return true;
    }

    return ReadSGIImage(filename, w, h, nc, bytes);
}

bool ReadSGIImage(const std::string & filename, int &w, int &h, int &nc, unsigned char *&bytes)
{
    sgi_t * sgip = sgiOpen(filename.c_str(), SGI_READ, 0, 0, 0, 0, 0);
    if (!sgip)
        return false;

    if (sgip->xsize == 0 || sgip->ysize == 0 ||
        sgip->zsize == 0 || sgip->zsize > 4)
    {
      fprintf(stderr, "DEBUG: Bad SGI image dimensions %ux%ux%u!\n", sgip->xsize, sgip->ysize, sgip->zsize);
      sgiClose(sgip);
      return false;
    }

    w = sgip->xsize;
    h = sgip->ysize;
    nc = sgip->zsize;

    bytes = new unsigned char[w*h*nc];

    std::vector<unsigned short> rbuf(w);
    bool readOK = true;

    for (int row = 0; row < h; row++) {
        for (int i = 0; i < nc; i++) {
            if (sgiGetRow(sgip, &rbuf[0], row, i) < 0) {
                row = h;  // Don't read any more rows
                readOK = false;
                break;
            }
            for (int j = 0; j < w; j++) {
                bytes[row*w*nc + j*nc + i] = (unsigned char) rbuf[j];
            }
        }
    }

    sgiClose(sgip);

    return readOK;
}
