#include <Inventor/actions/SoToVRML2Action.h>

SO_ACTION_SOURCE(SoToVRML2Action);

SoToVRML2Action::SoToVRML2Action()
{
    SO_ACTION_CONSTRUCTOR(SoToVRML2Action);
}

SoVRMLGroup *
SoToVRML2Action::getVRML2SceneGraph() const
{
    return NULL;
}

void
SoToVRML2Action::reuseAppearanceNodes(SbBool appearance)
{

}

void
SoToVRML2Action::reusePropertyNodes(SbBool property)
{

}

void
SoToVRML2Action::reuseGeometryNodes(SbBool geometry)
{

}

void
SoToVRML2Action::initClass()
{
    SO_ACTION_INIT_CLASS(SoToVRML2Action, SoAction);
}
