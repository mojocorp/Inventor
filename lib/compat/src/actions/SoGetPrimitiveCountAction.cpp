#include <Inventor/actions/SoGetPrimitiveCountAction.h>

SO_ACTION_SOURCE(SoGetPrimitiveCountAction);

SoGetPrimitiveCountAction::SoGetPrimitiveCountAction()
{
    SO_ACTION_CONSTRUCTOR(SoGetPrimitiveCountAction);
}

SoGetPrimitiveCountAction::~SoGetPrimitiveCountAction()
{

}

void
SoGetPrimitiveCountAction::initClass()
{
    SO_ACTION_INIT_CLASS(SoGetPrimitiveCountAction, SoAction);
}
