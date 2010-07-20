#include <Inventor/actions/SoToVRMLAction.h>

SO_ACTION_SOURCE(SoToVRMLAction);

SoToVRMLAction::SoToVRMLAction()
{
    SO_ACTION_CONSTRUCTOR(SoToVRMLAction);
}

SoNode *SoToVRMLAction::getVRMLSceneGraph() const
{
    return NULL;
}

void
SoToVRMLAction::initClass()
{
    SO_ACTION_INIT_CLASS(SoToVRMLAction, SoAction);
}
