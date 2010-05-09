#include <Inventor/caches/SoFontOutline.h>

////////////////////////////////////////////////////////////////////////
//
// Description:
//   Copy info from the font library into a more convenient form.
//
// Use: internal

SoFontOutline::SoFontOutline(FLoutline *outline, float fontSize)
//
////////////////////////////////////////////////////////////////////////
{
    charAdvance = SbVec2f(outline->xadvance,
                          outline->yadvance)*fontSize;
    numOutlines = outline->outlinecount;
    if (numOutlines != 0) {
        numVerts = new int[numOutlines];
        verts = new SbVec2f*[numOutlines];
        for (int i = 0; i < numOutlines; i++) {
            numVerts[i] = outline->vertexcount[i];
            if (numVerts[i] != 0) {
                verts[i] = new SbVec2f[numVerts[i]];
                for (int j = 0; j < numVerts[i]; j++) {
                    verts[i][j] = SbVec2f(outline->vertex[i][j].x,
                                          outline->vertex[i][j].y)*fontSize;
                }
            } else {
                verts[i] = NULL;
            }
        }
    } else {
        numVerts = NULL;
        verts = NULL;
    }
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//   Destructor; free up outline storage
//
// Use: internal

SoFontOutline::~SoFontOutline()
//
////////////////////////////////////////////////////////////////////////
{
    for (int i = 0; i < numOutlines; i++) {
        if (numVerts[i] != 0)
            delete[] verts[i];
    }
    if (numOutlines != 0) {
        delete[] verts;
        delete[] numVerts;
    }
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//   Get a do-nothing outline:
//
// Use: internal, static

SoFontOutline *
SoFontOutline::getNullOutline()
//
////////////////////////////////////////////////////////////////////////
{
    return new SoFontOutline;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//   Internal constructor used by getNullOutline
//
// Use: internal, static

SoFontOutline::SoFontOutline()
//
////////////////////////////////////////////////////////////////////////
{
    charAdvance = SbVec2f(0,0);
    numOutlines = 0;
    numVerts = NULL;
    verts = NULL;
}







