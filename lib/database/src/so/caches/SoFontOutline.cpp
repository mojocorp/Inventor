#include <Inventor/caches/SoFontOutline.h>
#include <Inventor/misc/SoGL.h>

#include <math.h>

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
    fontSize = 1.0f;
    charAdvance = SbVec2f(0,0);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//   Copy info from the font library into a more convenient form.
//
// Use: internal

SoFontOutline::SoFontOutline(FT_GlyphSlot & glyph, float size)
//
////////////////////////////////////////////////////////////////////////
{
    fontSize = size;

    static FT_Outline_Funcs ft2_outline_funcs = {
        (FT_Outline_MoveTo_Func)moveTo,
        (FT_Outline_LineTo_Func)lineTo,
        (FT_Outline_ConicTo_Func)conicTo,
        (FT_Outline_CubicTo_Func)cubicTo,
        0,0
    };

    FT_Outline &outline = glyph->outline;

    charAdvance = SbVec2f(glyph->advance.x,
                          glyph->advance.y)*fontSize/640.0f;

    FT_Outline_Decompose (&outline, &ft2_outline_funcs, this);

    bbox.makeEmpty();
    for (size_t ctr=0; ctr<verts.size(); ctr++) {
        for (size_t v=0; v<verts[ctr].size(); v++) {
            verts[ctr][v] /= 640.0f;
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

/*
 * This function injects a new contour in the render pool.
 */
int
SoFontOutline::moveTo (FT_Vector *to, SoFontOutline* fo)
{
    fo->verts.resize(fo->verts.size()+1);
    fo->verts.back().push_back(SbVec2f(to->x, to->y)*fo->fontSize);

    return 0;
}

/*
 * This function injects a new line segment in the render pool and
 * adjusts the faces list accordingly.
 */
int
SoFontOutline::lineTo (FT_Vector *to, SoFontOutline* fo)
{
    fo->verts.back().push_back(SbVec2f(to->x, to->y)*fo->fontSize);

    return 0;
}

/*
   Three control point Bezier interpolation
   mu ranges from 0 to 1, start to end of the curve
*/
SbVec2f Bezier3(const SbVec2f & p1,const SbVec2f & p2, const SbVec2f & p3,double mu)
{
    double mu2 = mu * mu;
    double mum1 = 1 - mu;
    double mum12 = mum1 * mum1;

    return p1 * mum12 + 2 * p2 * mum1 * mu + p3 * mu2;
}

/*
   Four control point Bezier interpolation
   mu ranges from 0 to 1, start to end of curve
*/
SbVec2f Bezier4(const SbVec2f & p1, const SbVec2f & p2,const SbVec2f & p3,const SbVec2f & p4,double mu)
{
    double mum1 = 1 - mu;
    double mum13 = mum1 * mum1 * mum1;
    double mu3 = mu * mu * mu;

    return mum13*p1 + 3*mu*mum1*mum1*p2 + 3*mu*mu*mum1*p3 + mu3*p4;
}

//
// Injects a new conic Bezier arc and adjusts the face list
// accordingly.
//
int
SoFontOutline::conicTo (FT_Vector *control,
                         FT_Vector *to,
                         SoFontOutline* fo)
{
    //fo->verts.back().push_back(SbVec2f(to->x, to->y)*fo->fontSize);

    SbVec2f a = fo->verts.back().back();
    SbVec2f b = SbVec2f(control->x, control->y)*fo->fontSize;
    SbVec2f c = SbVec2f(to->x, to->y)*fo->fontSize;

    for (int i=0; i<10; i++) {
        fo->verts.back().push_back(Bezier3(a,b,c,i/10.f));
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
    //fo->verts.back().push_back(SbVec2f(to->x, to->y)*fo->fontSize);
    SbVec2f a = fo->verts.back().back();
    SbVec2f b = SbVec2f(control1->x, control1->y)*fo->fontSize;
    SbVec2f c = SbVec2f(control2->x, control2->y)*fo->fontSize;
    SbVec2f d = SbVec2f(to->x, to->y)*fo->fontSize;

    int num_steps = 15;
    for (int i=0; i<num_steps; i++) {
        fo->verts.back().push_back(Bezier4(a,b,c,d,i/(float)num_steps));
    }

    return 0;
}



