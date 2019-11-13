#include "image.h"
#include "image-sgi.h"
#include "dds.h"

extern "C" unsigned char *stbi_load(char const *filename, int *x, int *y,
                                    int *comp, int req_comp);
extern "C" void           stbi_image_free(void *retval_from_stbi_load);

#include <stdlib.h>
#include <string>
#include <vector>

bool ReadSGIImage(const SbString &filename, SbImage &image);

static void
stbi_flip_copy(int w, int h, int comp, const unsigned char *from,
               unsigned char *to) {
    int                  stride = w * comp;
    const unsigned char *p1 = from;
    unsigned char *      p2 = to + (h - 1) * stride;

    for (int y = 0; y < h; ++y) {
        memcpy(p2, p1, stride);

        p1 += stride;
        p2 -= stride;
    }
}

bool
ReadImage(const SbString &filename, SbImage &image) {
    image.setActiveMipmap(0);

    int            w, h, nc;
    unsigned char *data = stbi_load(filename.getString(), &w, &h, &nc, 0);

    if (data) {
        SbImage::Format format = SbImage::Format_Invalid;
        switch (nc) {
        case 1:
            format = SbImage::Format_Luminance;
            break;
        case 2:
            format = SbImage::Format_Luminance_Alpha;
            break;
        case 3:
            format = SbImage::Format_RGB24;
            break;
        case 4:
            format = SbImage::Format_RGBA32;
            break;
        default:
            break;
        }
        image.setValue(SbVec2s(w, h), format, w * h * nc, NULL);
        stbi_flip_copy(w, h, nc, data, image.getBytes());
        stbi_image_free(data);
        return true;
    }

    if (ReadSGIImage(filename, image))
        return true;

    if (ReadDDSImage(filename, image))
        return true;

    return false;
}

bool
ReadSGIImage(const SbString &filename, SbImage &image) {
    sgi_t *sgip = sgiOpen(filename.getString(), SGI_READ, 0, 0, 0, 0, 0);
    if (!sgip)
        return false;

    if (sgip->xsize == 0 || sgip->ysize == 0 || sgip->zsize == 0 ||
        sgip->zsize > 4) {
        fprintf(stderr, "DEBUG: Bad SGI image dimensions %ux%ux%u!\n",
                sgip->xsize, sgip->ysize, sgip->zsize);
        sgiClose(sgip);
        return false;
    }

    int w = sgip->xsize;
    int h = sgip->ysize;
    int nc = sgip->zsize;

    SbImage::Format format = SbImage::Format_Invalid;
    switch (nc) {
    case 1:
        format = SbImage::Format_Luminance;
        break;
    case 2:
        format = SbImage::Format_Luminance_Alpha;
        break;
    case 3:
        format = SbImage::Format_RGB24;
        break;
    case 4:
        format = SbImage::Format_RGBA32;
        break;
    default:
        break;
    }

    image.setValue(SbVec2s(w, h), format, w * h * nc, NULL);

    unsigned char *bytes = image.getBytes();

    std::vector<unsigned short> rbuf(w);
    bool                        readOK = true;

    for (int row = 0; row < h; row++) {
        for (int i = 0; i < nc; i++) {
            if (sgiGetRow(sgip, &rbuf[0], row, i) < 0) {
                row = h; // Don't read any more rows
                readOK = false;
                break;
            }
            for (int j = 0; j < w; j++) {
                bytes[row * w * nc + j * nc + i] = (unsigned char)rbuf[j];
            }
        }
    }

    sgiClose(sgip);

    return readOK;
}
