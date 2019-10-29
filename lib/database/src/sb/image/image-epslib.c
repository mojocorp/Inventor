#include "image-eps.h"

#include <stdio.h>

static int putHex(FILE *fp, unsigned char val, int hexPos )
{
    fprintf(fp, "%02x", val);
    if (++hexPos >= 32) {
        fprintf(fp, "\n");
        hexPos = 0;
    }
    return hexPos;
}

int writeEps(FILE * fp, int w, int h, int nc, const unsigned char * bytes, float px, float py)
{
    // Write the PostScript header
    fprintf(fp, "%%!PS-Adobe-2.0 EPSF-1.2\n");
    fprintf(fp, "%%%%Creator: IRIS program output\n");
    fprintf(fp, "%%%%BoundingBox: 0 0 %d %d\n", w, h);
    fprintf(fp, "%%%%EndComments\n");
    fprintf(fp, "gsave\n");

    // Write the image into the PostScript file
    fprintf(fp, "/bwproc {\n");
    fprintf(fp, "    rgbproc\n");
    fprintf(fp, "    dup length 3 idiv string 0 3 0\n");
    fprintf(fp, "    5 -1 roll {\n");
    fprintf(fp, "    add 2 1 roll 1 sub dup 0 eq\n");
    fprintf(fp, "    { pop 3 idiv 3 -1 roll dup 4 -1 roll dup\n");
    fprintf(fp, "        3 1 roll 5 -1 roll put 1 add 3 0 }\n");
    fprintf(fp, "    { 2 1 roll } ifelse\n");
    fprintf(fp, "    } forall\n");
    fprintf(fp, "    pop pop pop\n");
    fprintf(fp, "} def\n");
    fprintf(fp, "systemdict /colorimage known not {\n");
    fprintf(fp, "    /colorimage {\n");
    fprintf(fp, "        pop\n");
    fprintf(fp, "        pop\n");
    fprintf(fp, "        /rgbproc exch def\n");
    fprintf(fp, "        { bwproc } image\n");
    fprintf(fp, "    } def\n");
    fprintf(fp, "} if\n");
    fprintf(fp, "/picstr %d string def\n", w * nc);
    fprintf(fp, "%d %d scale\n", (int)(px*72.0),
                                 (int)(py*72.0));
    fprintf(fp, "%d %d %d\n", w, h, 8);
    fprintf(fp, "[%d 0 0 %d 0 0]\n", w, h);
    fprintf(fp, "{currentfile picstr readhexstring pop}\n");
    fprintf(fp, "false %d\n", nc);
    fprintf(fp, "colorimage\n");

    // Convert the pixel values to ASCII hex and write them out.
    int numValues     = w*nc;
    int hexpos        = 0;

    for (int row=h-1; row>=0; row--) {
        
        const unsigned char * ptr = bytes + row*w*nc;

        // Write out the scanline
        for (int i=0; i<numValues; i++)
            hexpos = putHex(fp, (char)*ptr++, hexpos);
    }

    if (hexpos)
        fprintf(fp, "\n");

    // Finish up the PostScript file.
    fprintf(fp, "grestore\n");
    fprintf(fp, "showpage\n");

    return 0;
}

