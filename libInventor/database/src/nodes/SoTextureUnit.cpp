#include <Inventor/nodes/SoTextureUnit.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/actions/SoPickAction.h>
#include <Inventor/elements/SoTextureUnitElement.h>
#include <Inventor/elements/SoGLTextureUnitElement.h>

SO_NODE_SOURCE(SoTextureUnit);

////////////////////////////////////////////////////////////////////////
//
// Constructor
//
SoTextureUnit::SoTextureUnit()
{
  // Do standard constructor stuff
  SO_NODE_CONSTRUCTOR(SoTextureUnit);

  // Setup fields
  SO_NODE_ADD_FIELD(unit,     (0) );

  isBuiltIn = TRUE;
}

/////////////////////////////////////////////////////////////////
//
// This initializes the SoDecal class.
//
void SoTextureUnit::initClass()
{
    // Initialize type id variables
    SO__NODE_INIT_CLASS(SoTextureUnit, "TextureUnit", SoNode);

    SO_ENABLE(SoGLRenderAction, SoGLTextureUnitElement);
    SO_ENABLE(SoCallbackAction, SoTextureUnitElement);
    SO_ENABLE(SoPickAction, SoTextureUnitElement);
}

////////////////////////////////////////////////////////////////////////
//
// Destructor
//
SoTextureUnit::~SoTextureUnit()
{
}

uint32_t 
SoTextureUnit::getMaxTextureUnit(SoState *state)
{
    return SoTextureUnitElement::getMaxTextureUnit(state);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Implements all actions.
//
// Use: extender

void
SoTextureUnit::doAction(SoAction *action)
//
////////////////////////////////////////////////////////////////////////
{
    SoTextureUnitElement::set(action->getState(), unit.getValue());
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Does callback action thing.
//
// Use: extender

void
SoTextureUnit::callback(SoCallbackAction *action)
//
////////////////////////////////////////////////////////////////////////
{
    SoTextureUnit::doAction(action);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Does GL render action.
//
// Use: extender

void
SoTextureUnit::GLRender(SoGLRenderAction *action)
//
////////////////////////////////////////////////////////////////////////
{
    SoTextureUnit::doAction(action);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Does pick action.
//
// Use: extender

void
SoTextureUnit::pick(SoPickAction *action)
//
////////////////////////////////////////////////////////////////////////
{
    SoTextureUnit::doAction(action);
}
