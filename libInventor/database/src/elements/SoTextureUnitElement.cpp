#include <Inventor/misc/SoGL.h>
#include <Inventor/elements/SoTextureUnitElement.h>
#include <Inventor/actions/SoGLRenderAction.h>

SO_ELEMENT_SOURCE(SoTextureUnitElement);

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Initializes SoTextureUnitElement class.
//
// Use: internal

void
SoTextureUnitElement::initClass()
{
    SO_ELEMENT_INIT_CLASS(SoTextureUnitElement, SoInt32Element);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Destructor.
//
// Use: private

SoTextureUnitElement::~SoTextureUnitElement()
//
////////////////////////////////////////////////////////////////////////
{
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Initializes element
//
// Use: public

void
SoTextureUnitElement::init(SoState *)
//
////////////////////////////////////////////////////////////////////////
{
    data = 0;
}

uint32_t
SoTextureUnitElement::getMaxTextureUnit(SoState *state)
{
    GLint maxUnits = 0;
    if (dynamic_cast<SoGLRenderAction*>(state->getAction())) {
        glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxUnits);
    }
    return maxUnits;
}
