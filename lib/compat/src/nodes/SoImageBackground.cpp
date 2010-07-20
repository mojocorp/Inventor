#include <Inventor/nodes/SoImageBackground.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/elements/SoViewportRegionElement.h>
#include <Inventor/sensors/SoFieldSensor.h>

#include <Inventor/misc/SoGL.h>

#include <algorithm>

SO_NODE_SOURCE(SoImageBackground)

////////////////////////////////////////////////////////////////////////
//
// Description:
//    This initializes the SoImageBackground class.
//
// Use: internal

void
SoImageBackground::initClass()
//
////////////////////////////////////////////////////////////////////////
{
    SO__NODE_INIT_CLASS(SoImageBackground,"ImageBackground", SoBackground);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Constructor
//
// Use: protected

SoImageBackground::SoImageBackground()
//
////////////////////////////////////////////////////////////////////////
{
    SO_NODE_CONSTRUCTOR(SoImageBackground);
    SO_NODE_ADD_FIELD(filename, (""));
    SO_NODE_ADD_FIELD(image, (SbVec2s(0,0), 0, NULL));
    SO_NODE_ADD_FIELD(style, (SoImageBackground::TILE));

    SO_NODE_DEFINE_ENUM_VALUE(Style, NONE);
    SO_NODE_DEFINE_ENUM_VALUE(Style, CENTER);
    SO_NODE_DEFINE_ENUM_VALUE(Style, LOWER_LEFT);
    SO_NODE_DEFINE_ENUM_VALUE(Style, UPPER_LEFT);
    SO_NODE_DEFINE_ENUM_VALUE(Style, UPPER_RIGHT);
    SO_NODE_DEFINE_ENUM_VALUE(Style, LOWER_RIGHT);
    SO_NODE_DEFINE_ENUM_VALUE(Style, STRETCH);
    SO_NODE_DEFINE_ENUM_VALUE(Style, TILE);
    SO_NODE_SET_SF_ENUM_TYPE(style, Style);

    isBuiltIn = TRUE;

    filenamesensor = new SoFieldSensor(&filenameChangedCB, this);
    filenamesensor->setPriority(0);
    filenamesensor->attach(&this->filename);

    imagesensor = new SoFieldSensor(&imageChangedCB, this);
    imagesensor->setPriority(0);
    imagesensor->attach(&this->image);
}

////////////////////////////////////////////////////////////////////////
//
// Destructor
//

SoImageBackground::~SoImageBackground()
//
////////////////////////////////////////////////////////////////////////
{
    delete filenamesensor;
    delete imagesensor;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Callback that reads in the given file and stuffs it into the
//    image field whenever the filename field changes.
//
// Use: static, internal

void SoImageBackground::filenameChangedCB( void * data, SoSensor * )
//
////////////////////////////////////////////////////////////////////////
{
    SoImageBackground * bkg = (SoImageBackground*) data;
    if (bkg->filename.getValue().getLength()) {
#if 0
        SbImage tmpimage;
        const SbStringList & sl = SoInput::getDirectories();
        if (tmpimage.readFile(thisp->filename.getValue(), sl.getArrayPtr(), sl.getLength())) {
            int nc;
            SbVec2s size;
            const unsigned char * bytes = tmpimage.getValue(size, nc);
            // disable notification on image while setting data from filename
            // as a notify will cause a filename.setDefault(TRUE).
            SbBool oldnotify = thisp->image.enableNotify(FALSE);
            thisp->image.setValue(size, nc, bytes);
            thisp->image.enableNotify(oldnotify);
            thisp->glimagevalid = FALSE;
        }else{
            SoDebugError::postWarning("SoImageBackground::filenameSensorCB",
                                      "could not read image file '%s'",
                                      thisp->filename.getValue().getString());
        }
#endif
    }
    //  set its default bit so it isn't written out
    bkg->image.setDefault(TRUE);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Callback that sets the filename to "" whenever the image field
//    is changed.
//
// Use: static, internal

void SoImageBackground::imageChangedCB( void * data, SoSensor * )
//
////////////////////////////////////////////////////////////////////////
{
  SoImageBackground * bkg = (SoImageBackground*) data;
  bkg->filenamesensor->detach();
  bkg->filename.setValue("");
  bkg->filename.setDefault(TRUE);
  bkg->filenamesensor->attach(&bkg->filename);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Traversal for GL rendering
//
// Use: extender

void
SoImageBackground::GLRender(SoGLRenderAction * action)
//
////////////////////////////////////////////////////////////////////////
{
    SoState *state = action->getState();
#if 0
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

    glPushAttrib(GL_ENABLE_BIT);

    GLboolean depthMask;
    glGetBooleanv(GL_DEPTH_WRITEMASK, &depthMask);
    if (depthMask)
        glDepthMask(FALSE);

    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);

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
#endif
}
