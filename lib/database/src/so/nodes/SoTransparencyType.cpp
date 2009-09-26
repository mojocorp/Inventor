#include <Inventor/nodes/SoTransparencyType.h>

#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoPickAction.h>
#include <Inventor/elements/SoOverrideElement.h>
#include <Inventor/elements/SoShapeStyleElement.h>
#include <Inventor/elements/SoLazyElement.h>

SO_NODE_SOURCE(SoTransparencyType);

SoTransparencyType::SoTransparencyType(void)
{
    SO_NODE_CONSTRUCTOR(SoTransparencyType);

    SO_NODE_ADD_FIELD(type, (SCREEN_DOOR));

    SO_NODE_DEFINE_ENUM_VALUE(TransparencyType, SCREEN_DOOR);
    SO_NODE_DEFINE_ENUM_VALUE(TransparencyType, ADD);
    SO_NODE_DEFINE_ENUM_VALUE(TransparencyType, DELAYED_ADD);
    SO_NODE_DEFINE_ENUM_VALUE(TransparencyType, SORTED_OBJECT_ADD);
    SO_NODE_DEFINE_ENUM_VALUE(TransparencyType, BLEND);
    SO_NODE_DEFINE_ENUM_VALUE(TransparencyType, DELAYED_BLEND);
    SO_NODE_DEFINE_ENUM_VALUE(TransparencyType, SORTED_OBJECT_BLEND);

    SO_NODE_SET_SF_ENUM_TYPE(type, TransparencyType);

    isBuiltIn = TRUE;
}

SoTransparencyType::~SoTransparencyType()
{
}

void
SoTransparencyType::initClass(void)
{
    SO__NODE_INIT_CLASS(SoTransparencyType, "TransparencyType", SoNode);

    SO_ENABLE(SoCallbackAction, SoTransparencyTypeElement);

    SO_ENABLE(SoGLRenderAction, SoTransparencyTypeElement);
}

void
SoTransparencyType::doAction(SoAction * action)
{
    SoState	*state = action->getState();

    if (! type.isIgnored()
        && ! SoOverrideElement::getTransparencyTypeOverride(state)) {
        if (isOverride()) {
            SoOverrideElement::setTransparencyTypeOverride(state, this, TRUE);
        }
        SoTransparencyTypeElement::set(state, (SoTransparencyTypeElement::TransparencyType)type.getValue());
        SoShapeStyleElement::setTransparencyType(state, type.getValue());
        SoLazyElement::setTransparencyType(state, type.getValue());
    }
}

void
SoTransparencyType::callback(SoCallbackAction * action)
{
    SoTransparencyType::doAction((SoAction *)action);
}

void
SoTransparencyType::GLRender(SoGLRenderAction * action)
{
    SoTransparencyType::doAction(action);
}

