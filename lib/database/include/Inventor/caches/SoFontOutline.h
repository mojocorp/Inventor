#ifndef  _SO_FONTOUTLINE_
#define  _SO_FONTOUTLINE_

#include <Inventor/SbVec2f.h>

// Font library:
#include <flclient.h>

// First, a more convenient structure for outlines:
class SoFontOutline {

  public:
    // Constructor, takes a pointer to the font-library outline
    // structure and the font's size:
    SoFontOutline(FLoutline *outline, float fontSize);
    // Destructor
    ~SoFontOutline();

    // Query routines:
    int		getNumOutlines() { return numOutlines; }
    int		getNumVerts(int i) { return numVerts[i]; }
    SbVec2f	&getVertex(int i, int j) { return verts[i][j]; }
    SbVec2f	getCharAdvance() { return charAdvance; }

    static SoFontOutline *getNullOutline();

  private:
    // Internal constructor used by getNullOutline:
    SoFontOutline();

    // This basically mimics the FLoutline structure, with the
    // exception that the font size is part of the outline:
    int numOutlines;
    int *numVerts;
    SbVec2f **verts;
    SbVec2f charAdvance;
};

#endif /* _SO_FONTOUTLINE_ */
