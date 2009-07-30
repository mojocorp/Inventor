#include "image.h"

#include "image-sgi.h"
#include "image-gif.h"

#include <stdlib.h>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>

extern "C" {
#include <jpeglib.h>
}
#include <setjmp.h>

bool ReadSGIImage(const std::string & filename, int &w, int &h, int &nc, unsigned char *&bytes);
bool ReadGIFImage(const std::string & filename, int &w, int &h, int &nc, unsigned char *&bytes);
bool ReadJPGImage(const std::string & filename, int &w, int &h, int &nc, unsigned char *&bytes);

std::string getFileExtension(const std::string & fileName)
{
    if (fileName.find_last_of(".") != std::string::npos)
        return fileName.substr(fileName.find_last_of(".")+1);
    return "";
}

bool ReadImage(const char* filename, int &w, int &h, int &nc, unsigned char *&bytes)
{
    std::string extension = getFileExtension(filename);
    std::transform(extension.begin(), extension.end(), extension.begin(), tolower);

    if (extension == "jpg" || extension == "jpeg")
        return ReadJPGImage(filename, w, h, nc, bytes);

    if (extension == "gif")
        return ReadGIFImage(filename, w, h, nc, bytes);

    if (extension == "rgb")
        return ReadSGIImage(filename, w, h, nc, bytes);

    return false;
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

bool ReadGIFImage(const std::string & filename, int &w, int &h, int &nc, unsigned char *&bytes)
{
    int ncolors;
    int bgIndex;
    int errCode;

    FILE *fp = fopen(filename.c_str(), "rb");
    if (!fp)
        return false;

    std::vector<GIF_Color> colors(GIF_MAXCOLORMAPSIZE);

    unsigned char *array = readGIF(fp, &w, &h, &colors[0], &ncolors, &bgIndex, &errCode);
    if (errCode != GIF_NO_ERROR) {
        fclose(fp);
        return false;
    }

    nc = 3;

    // convert color map image to rgb
    // gif files go top down and we need bottom up.  Switch it.
    bytes = new unsigned char[w*h*nc];

    for (int j = 0; j < h; ++j)
        for (int i = 0; i < w; ++i) {
            int c = array[j*w+i];
            int index = ((h-j-1)*w+i)*nc;

            if (c < 0 || c >= ncolors)
                // store black if out of range
                bytes[index] = bytes[index+1] = bytes[index+2] = 0;
            else {
                bytes[index] = colors[c].red / 256;
                bytes[index+1] = colors[c].green / 256;
                bytes[index+2] = colors[c].blue / 256;
            }
        }

    free(array);
    fclose(fp);

    return true;
}

// JPEG error handler

struct my_error_mgr {
    struct jpeg_error_mgr pub;	/* "public" fields */

    jmp_buf setjmp_buffer;	/* for return to caller */
};

typedef struct my_error_mgr * my_error_ptr;

/*
 * Here's the routine that will replace the standard error_exit method:
 */

METHODDEF(void)
my_error_exit (j_common_ptr cinfo)
{
    /* cinfo->err really points to a my_error_mgr struct, so coerce pointer */
    my_error_ptr myerr = (my_error_ptr) cinfo->err;

    /* Always display the message. */
    /* We could postpone this until after returning, if we chose. */
    (*cinfo->err->output_message) (cinfo);

    /* Return control to the setjmp point */
    longjmp(myerr->setjmp_buffer, 1);
}

bool ReadJPGImage(const std::string & filename, int &w, int &h, int &nc, unsigned char *&bytes)
{

    /* This struct contains the JPEG decompression parameters and pointers to
     * working space (which is allocated as needed by the JPEG library).
     */
    struct jpeg_decompress_struct cinfo;
    /* We use our private extension JPEG error handler. */
    struct my_error_mgr jerr;

    /* More stuff */
    FILE * infile;		/* source file */
    JSAMPARRAY buffer;		/* Output row buffer */
    int row_stride;		/* physical row width in output buffer */

    infile = fopen(filename.c_str(), "rb");
    if (!infile)
        return false;

    fseek(infile, 0, SEEK_SET);

    /* Step 1: allocate and initialize JPEG decompression object */

    /* We set up the normal JPEG error routines, then override error_exit. */
    cinfo.err = jpeg_std_error(&jerr.pub);
    jerr.pub.error_exit = my_error_exit;

    /* Establish the setjmp return context for my_error_exit to use. */
    if (setjmp(jerr.setjmp_buffer)) {
        /* If we get here, the JPEG code has signaled an error.
         * We need to clean up the JPEG object, close the input file, and return.
         */
        jpeg_destroy_decompress(&cinfo);
        fclose(infile);
        return false;
    }

    /* Now we can initialize the JPEG decompression object. */
    jpeg_create_decompress(&cinfo);

    /* Step 2: specify data source (eg, a file) */

    jpeg_stdio_src(&cinfo, infile);

    /* Step 3: read file parameters with jpeg_read_header() */

    jpeg_read_header(&cinfo, TRUE);

    /* Step 5: Start decompressor */

    jpeg_start_decompress(&cinfo);

    // make buffer for data to be put into
    w = cinfo.output_width;
    h = cinfo.output_height;
    nc = cinfo.output_components;
    bytes = new unsigned char[w*h*nc];

    row_stride = w*nc;
    /* Make a one-row-high sample array that will go away when done with image */
    buffer = (*cinfo.mem->alloc_sarray)
            ((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);

    unsigned char *curline = bytes+(h-1)*w*nc;

    while (cinfo.output_scanline < cinfo.output_height) {
        jpeg_read_scanlines(&cinfo, buffer, 1);

        // put the data from the sample buffer into the output buffer
        for (int i = 0; i < row_stride; ++i)
            curline[i] = (unsigned char) (buffer[0][i]);
        curline -= w*nc;
    }

    /* Step 7: Finish decompression */

    jpeg_finish_decompress(&cinfo);

    /* Step 8: Release JPEG decompression object */

    /* This is an important step since it will release a good deal of memory. */
    jpeg_destroy_decompress(&cinfo);

    /* At this point you may want to check to see whether any corrupt-data
     * warnings occurred (test whether jerr.pub.num_warnings is nonzero).
     */

    fclose(infile);

    return true;
}
