#include <Inventor/nodes/SoGradientBackground.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/elements/SoViewportRegionElement.h>

#include <Inventor/misc/SoGL.h>

#include <algorithm>

SO_NODE_SOURCE(SoGradientBackground)

////////////////////////////////////////////////////////////////////////
//
// Description:
//    This initializes the SoGradientBackground class.
//
// Use: internal

void
SoGradientBackground::initClass()
//
////////////////////////////////////////////////////////////////////////
{
    SO__NODE_INIT_CLASS(SoGradientBackground,"GradientBackground", SoBackground);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Constructor
//
// Use: public

SoGradientBackground::SoGradientBackground()
//
////////////////////////////////////////////////////////////////////////
{
    SO_NODE_CONSTRUCTOR(SoGradientBackground);

    SO_NODE_ADD_FIELD(color0,(0.7f,0.7f,0.8f));
    SO_NODE_ADD_FIELD(color1,(0.0f,0.1f,0.3f));
    SO_NODE_ADD_FIELD(swapColors,(FALSE));
    SO_NODE_ADD_FIELD(direction,(0.0f,1.0f));

    isBuiltIn = TRUE;
}

////////////////////////////////////////////////////////////////////////
//
// Destructor
//

SoGradientBackground::~SoGradientBackground()
//
////////////////////////////////////////////////////////////////////////
{
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Traversal for GL rendering
//
// Use: extender

void
SoGradientBackground::GLRender(SoGLRenderAction * action)
//
////////////////////////////////////////////////////////////////////////
{
    SoState *state = action->getState();

    // Take care of tiled display
    const SbViewportRegion & vr = SoViewportRegionElement::get(state);
    const SbVec2f & vorigin = vr.getViewportOrigin();
    const SbVec2f & vsize = vr.getViewportSize();

    SbVec2f dir = this->direction.getValue();
    dir.normalize();

    float la = SbVec2f(vorigin[0], vorigin[1]).dot(dir);
    float lb = SbVec2f(vorigin[0] + vsize[0], vorigin[1]).dot(dir);
    float lc = SbVec2f(vorigin[0] + vsize[0], vorigin[1] + vsize[1]).dot(dir);
    float ld = SbVec2f(vorigin[0], vorigin[1] + vsize[1]).dot(dir);

    float min = std::min(std::min(la, lb), lc);
    float max = std::max(std::max(la, lb), lc);
    float lenght = max - min;

    if(lenght == 0)
        return;

    la = (la - min) / lenght;
    lb = (lb - min) / lenght;
    lc = (lc - min) / lenght;
    ld = (ld - min) / lenght;

    SbColor lc0 = swapColors.getValue() ? this->color1.getValue() : this->color0.getValue();
    SbColor lc1 = swapColors.getValue() ? this->color0.getValue() : this->color1.getValue();

    SbColor c1 = lc0 + (lc1 - lc0) * la;
    SbColor c2 = lc0 + (lc1 - lc0) * lb;
    SbColor c3 = lc0 + (lc1 - lc0) * lc;
    SbColor c4 = lc0 + (lc1 - lc0) * ld;

    glPushAttrib(GL_ENABLE_BIT|GL_POLYGON_BIT|GL_CURRENT_BIT);

    GLboolean depthMask;
    glGetBooleanv(GL_DEPTH_WRITEMASK, &depthMask);
    if (depthMask)
        glDepthMask(FALSE);

    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

    glBegin( GL_QUADS );

    glColor3fv( c1.getValue() ); glVertex2i( -1, -1 );
    glColor3fv( c2.getValue() ); glVertex2i(  1, -1 );
    glColor3fv( c3.getValue() ); glVertex2i(  1,  1 );
    glColor3fv( c4.getValue() ); glVertex2i( -1,  1 );

    glEnd();

    // Restore OpenGL matrices & attrib
    if (depthMask)
        glDepthMask(TRUE);

    glPopMatrix();
    glMatrixMode( GL_MODELVIEW );
    glPopMatrix();
    glPopAttrib();
}
