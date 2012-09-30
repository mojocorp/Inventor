#include <Inventor/caches/SoFontOutline.h>
#include <Inventor/misc/SoGL.h>
#include <Inventor/errors/SoDebugError.h>

#include <math.h>
#include <algorithm>

////////////////////////////////////////////////////////////////////////
//
// Description:
//   Copy info from the font library into a more convenient form.
//
// Use: internal

SoFontOutline::SoFontOutline(wchar_t ch, FT_Face face, float size)
//
////////////////////////////////////////////////////////////////////////
{
    FT_UInt index = FT_Get_Char_Index(face, ch);
    if(FT_Load_Glyph(face, index, FT_LOAD_IGNORE_TRANSFORM | FT_LOAD_NO_SCALE)) {
#ifdef DEBUG
        SoDebugError::post("SoFontOutline",
                           "FT_Load_Glyph failed");
#endif
    }

    scale = size / (float)face->units_per_EM;

    if (index == 0) {
        verts.resize(1);
        verts[0].push_back(SbVec2f(0.0f, 0.0f)*size);
        verts[0].push_back(SbVec2f(1.0f, 0.0f)*size);
        verts[0].push_back(SbVec2f(1.0f, 1.0f)*size);
        verts[0].push_back(SbVec2f(0.0f, 1.0f)*size);

        charAdvance = SbVec2f(1.0f, 0.0f);
    } else {
        static FT_Outline_Funcs ft2_outline_funcs = {
            (FT_Outline_MoveTo_Func)moveTo,
            (FT_Outline_LineTo_Func)lineTo,
            (FT_Outline_ConicTo_Func)conicTo,
            (FT_Outline_CubicTo_Func)cubicTo,
            0,0
        };

        FT_Outline *outline = &face->glyph->outline;

        verts.reserve(outline->n_contours);

        charAdvance = SbVec2f(face->glyph->advance.x,
                              face->glyph->advance.y)*scale;

        FT_Outline_Decompose (outline, &ft2_outline_funcs, this);

        // reverse the contours if necessary
        if (!(outline->flags & FT_OUTLINE_REVERSE_FILL)) {
            for (size_t ctr=0; ctr<verts.size(); ctr++) {
                std::reverse(verts[ctr].begin(), verts[ctr].end());
            }
        }
        for (size_t ctr=0; ctr<verts.size(); ctr++) {
            if (verts[ctr].front().equals(verts[ctr].back(), 10E-6)) {
                verts[ctr].pop_back();
            }
        }
    }

    bbox.makeEmpty();
    for (size_t ctr=0; ctr<verts.size(); ctr++) {
        for (size_t v=0; v<verts[ctr].size(); v++) {
            bbox.extendBy(verts[ctr][v]);
        }
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

}

/*
 * This function injects a new contour in the render pool.
 */
int
SoFontOutline::moveTo (FT_Vector *to, SoFontOutline* fo)
{
    fo->verts.push_back(std::vector<SbVec2f>());
    fo->verts.back().push_back(SbVec2f(to->x, to->y)*fo->scale);

    return 0;
}

/*
 * This function injects a new line segment in the render pool and
 * adjusts the faces list accordingly.
 */
int
SoFontOutline::lineTo (FT_Vector *to, SoFontOutline* fo)
{
    std::vector<SbVec2f> & ctr = fo->verts.back();

    ctr.push_back(SbVec2f(to->x, to->y)*fo->scale);

    return 0;
}

//
// Injects a new conic Bezier arc and adjusts the face list
// accordingly.
//
int
SoFontOutline::conicTo (FT_Vector *control, FT_Vector *to, SoFontOutline* fo)
{
    std::vector<SbVec2f> & ctr = fo->verts.back();

    SbVec2f p1 = ctr.back();
    SbVec2f p2 = SbVec2f(control->x, control->y)*fo->scale;
    SbVec2f p3 = SbVec2f(to->x, to->y)*fo->scale;

    int num_steps = 15;
    for(int i=1; i<num_steps; i++) {
        float mu = i/(float)num_steps;
        float mu2 = mu * mu;
        float mum1 = 1 - mu;
        float mum12 = mum1 * mum1;

        ctr.push_back(mum12 * p1 + 2*mu*mum1 * p2 + mu2 * p3);
    }
    return 0;
}

/*
 * Injects a new cubic Bezier arc and adjusts the face list
 * accordingly.
 */
int
SoFontOutline::cubicTo (FT_Vector *control1,
                         FT_Vector *control2,
                         FT_Vector *to,
                         SoFontOutline* fo)
{
    std::vector<SbVec2f> & ctr = fo->verts.back();

    SbVec2f p1 = ctr.back();
    SbVec2f p2 = SbVec2f(control1->x, control1->y)*fo->scale;
    SbVec2f p3 = SbVec2f(control2->x, control2->y)*fo->scale;
    SbVec2f p4 = SbVec2f(to->x, to->y)*fo->scale;

    int num_steps = 15;
    for (int i=1; i<num_steps; i++) {
        float mu = i/(float)num_steps;
        float mum1 = 1 - mu;
        float mum13 = mum1 * mum1 * mum1;
        float mu3 = mu * mu * mu;
        ctr.push_back(mum13*p1 + 3*mu*mum1*mum1*p2 + 3*mu*mu*mum1*p3 + mu3*p4);
    }

    return 0;
}



