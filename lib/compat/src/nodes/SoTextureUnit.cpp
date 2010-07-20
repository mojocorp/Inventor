#include <Inventor/nodes/SoTextureUnit.h>

#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoCallbackAction.h>

#include <Inventor/elements/SoTextureUnitElement.h>

SO_NODE_SOURCE(SoTextureUnit)

void
SoTextureUnit::initClass()
{
    SO__NODE_INIT_CLASS(SoTextureUnit,"TextureUnit", SoNode);

    SO_ENABLE(SoCallbackAction, SoTextureUnitElement);
//    SO_ENABLE(SoGLRenderAction, SoGLTextureUnitElement);
}

SoTextureUnit::SoTextureUnit()
{
    SO_NODE_CONSTRUCTOR(SoTextureUnit);

    SO_NODE_ADD_FIELD(unit,(0));
}

SoTextureUnit::~SoTextureUnit()
{

}

void
SoTextureUnit::doAction(SoAction *action)
{

}

void
SoTextureUnit::callback(SoCallbackAction *action)
{

}

void
SoTextureUnit::GLRender(SoGLRenderAction *action)
{

}
