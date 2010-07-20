#include <Inventor/actions/SoDistributeAction.h>

SO_ACTION_SOURCE(SoDistributeAction);

SoDistributeAction::SoDistributeAction()
{
    SO_ACTION_CONSTRUCTOR(SoDistributeAction);
}

SoDistributeAction::~SoDistributeAction()
{

}

void
SoDistributeAction::initClass()
{
    SO_ACTION_INIT_CLASS(SoDistributeAction, SoAction);
}
