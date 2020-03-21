/*
 * dds.c -- dds texture loader
 * last modification: aug. 14, 2007
 *
 * Copyright (c) 2005-2007 David HENRY
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR
 * ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
 * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * gcc -Wall -ansi -lGL -lGLU -lglut dds.c -o dds
 */

#include <Inventor/SbVec.h>
#include <Inventor/SbImage.h>
#include <glad/gl.h>
#include "dds.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <algorithm>

#pragma pack(1)
/* DirectDraw's structures */
struct DDPixelFormat {
    GLuint size;
    GLuint flags;
    GLuint fourCC;
    GLuint bpp;
    GLuint redMask;
    GLuint greenMask;
    GLuint blueMask;
    GLuint alphaMask;
};

struct DDSCaps {
    GLuint caps;
    GLuint caps2;
    GLuint caps3;
    GLuint caps4;
};

struct DDColorKey {
    GLuint lowVal;
    GLuint highVal;
};

struct DDSurfaceDesc {
    GLuint size;
    GLuint flags;
    GLuint height;
    GLuint width;
    GLuint pitch;
    GLuint depth;
    GLuint mipMapLevels;
    GLuint alphaBitDepth;
    GLuint reserved;
    GLuint surface;

    struct DDColorKey ckDestOverlay;
    struct DDColorKey ckDestBlt;
    struct DDColorKey ckSrcOverlay;
    struct DDColorKey ckSrcBlt;

    struct DDPixelFormat format;
    struct DDSCaps       caps;

    GLuint textureStage;
};
#pragma pack()

#define MAKEFOURCC(ch0, ch1, ch2, ch3)                                         \
    (GLuint)((((GLuint)(GLubyte)(ch3) << 24) & 0xFF000000) |                   \
             (((GLuint)(GLubyte)(ch2) << 16) & 0x00FF0000) |                   \
             (((GLuint)(GLubyte)(ch1) << 8) & 0x0000FF00) |                    \
             ((GLuint)(GLubyte)(ch0)&0x000000FF))

#define FOURCC_DXT1 MAKEFOURCC('D', 'X', 'T', '1')
#define FOURCC_DXT3 MAKEFOURCC('D', 'X', 'T', '3')
#define FOURCC_DXT5 MAKEFOURCC('D', 'X', 'T', '5')
#define DDS_ALPHA_PIXELS 0x00000001

bool
ReadDDSImage(const SbString &filename, SbImage &image) {
    // Open the file
    FILE *fp = fopen(filename.getString(), "rb");
    if (!fp) {
        fprintf(stderr, "error: couldn't open \"%s\"!\n", filename.getString());
        return false;
    }

    // Read magic number and check if valid .dds file
    char magic[4];
    fread(&magic, sizeof(char), 4, fp);

    if (strncmp(magic, "DDS ", 4) != 0) {
        fprintf(stderr,
                "the file \"%s\" doesn't appear to be"
                "a valid .dds file!\n",
                filename.getString());
        fclose(fp);
        return false;
    }

    // Get the surface descriptor
    struct DDSurfaceDesc ddsd;
    fread(&ddsd, sizeof(ddsd), 1, fp);

    SbImage::Format format = SbImage::Format_Invalid;
    size_t          blockSize = 0;
    switch (ddsd.format.fourCC) {
    case FOURCC_DXT1:
        // DXT1's compression ratio is 8:1
        format = (ddsd.format.flags & DDS_ALPHA_PIXELS)
                     ? SbImage::Format_RGBA_S3TC_DXT1
                     : SbImage::Format_RGB_S3TC_DXT1;
        blockSize = 8;
        break;

    case FOURCC_DXT3:
        // DXT3's compression ratio is 4:1
        format = SbImage::Format_RGBA_S3TC_DXT3;
        blockSize = 16;
        break;

    case FOURCC_DXT5:
        // DXT5's compression ratio is 4:1
        format = SbImage::Format_RGBA_S3TC_DXT5;
        blockSize = 16;
        break;

    default:
        // Bad fourCC, unsupported or bad format
        fprintf(stderr,
                "the file \"%s\" doesn't appear to be"
                "compressed using DXT1, DXT3, or DXT5! [%i]\n",
                filename.getString(), ddsd.format.fourCC);
        fclose(fp);
        return false;
    }

    int width = ddsd.width;
    int height = ddsd.height;
    for (GLuint level = 0; level < ddsd.mipMapLevels; ++level) {
        size_t numBytes = ((width + 3) / 4) * ((height + 3) / 4) * blockSize;

        image.setActiveMipmap(level);
        image.setValue(SbVec2s(width, height), format, numBytes, NULL);

        // Read pixel data with mipmaps
        if (fread(image.getBytes(), sizeof(GLubyte), numBytes, fp) !=
            numBytes) {
            printf("Error reading dds\n");
            fflush(stdout);
        }

        width = std::max(width >> 1, 1);
        height = std::max(height >> 1, 1);
    }

    // Close the file
    fclose(fp);

    return true;
}
