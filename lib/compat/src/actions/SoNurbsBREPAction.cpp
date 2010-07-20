#include <Inventor/actions/SoNurbsBREPAction.h>

SO_ACTION_SOURCE(SoNurbsBREPAction);

SoNurbsBREPAction::SoNurbsBREPAction()
{
    SO_ACTION_CONSTRUCTOR(SoNurbsBREPAction);
}

SoNurbsBREPAction::~SoNurbsBREPAction()
{

}

void
SoNurbsBREPAction::initClass()
{
    SO_ACTION_INIT_CLASS(SoNurbsBREPAction, SoAction);
}
