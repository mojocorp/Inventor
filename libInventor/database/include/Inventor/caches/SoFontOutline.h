#ifndef  _SO_FONTOUTLINE_
#define  _SO_FONTOUTLINE_

#include <Inventor/SbVec2f.h>
#include <Inventor/SbBox2f.h>

#include <vector>

#include "ft2build.h"
#include FT_FREETYPE_H
#include FT_OUTLINE_H

// First, a more convenient structure for outlines:
class SoFontOutline {

  public:
    // Constructor, takes a pointer to the font-library outline
    // structure and the font's size:
    SoFontOutline(wchar_t ch, FT_Face face, float fontSize);
    // Destructor
    ~SoFontOutline();

    // Query routines:
    size_t	    getNumOutlines() const { return verts.size(); }
    size_t	    getNumVerts(size_t i) const { return verts[i].size(); }
    const SbVec2f & getVertex(size_t i, size_t j) const { return verts[i][j]; }
    const SbVec2f & getCharAdvance() const { return charAdvance; }
    const SbBox2f & getBoundingBox() const { return bbox;}

  private:
    static int moveTo(FT_Vector *to, SoFontOutline* fo);
    static int lineTo(FT_Vector *to, SoFontOutline* fo);
    static int conicTo(FT_Vector *control, FT_Vector *to, SoFontOutline* fo);
    static int cubicTo(FT_Vector *control1, FT_Vector *control2, FT_Vector *to, SoFontOutline* fo);

    // This basically mimics the FLoutline structure, with the
    // exception that the font size is part of the outline:
    float scale;
    std::vector< std::vector<SbVec2f> > verts;
    SbVec2f charAdvance;
    SbBox2f bbox;
};

#endif /* _SO_FONTOUTLINE_ */
