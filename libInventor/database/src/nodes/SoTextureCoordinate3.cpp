#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoPickAction.h>
#include <Inventor/elements/SoGLTextureCoordinate3Element.h>
#include <Inventor/nodes/SoTextureCoordinate3.h>

SO_NODE_SOURCE(SoTextureCoordinate3);

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Constructor
//
// Use: public

SoTextureCoordinate3::SoTextureCoordinate3()
//
////////////////////////////////////////////////////////////////////////
{
    SO_NODE_CONSTRUCTOR(SoTextureCoordinate3);
    SO_NODE_ADD_FIELD(point, (SbVec3f(0,0,0)));
    point.deleteValues(0);
    isBuiltIn = TRUE;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    This initializes the SoTextureCoordinate3 class.
//
// Use: internal

void
SoTextureCoordinate3::initClass()
//
////////////////////////////////////////////////////////////////////////
{
    SO__NODE_INIT_CLASS(SoTextureCoordinate3, "TextureCoordinate3", SoNode);

    SO_ENABLE(SoCallbackAction, SoTextureCoordinate3Element);
    SO_ENABLE(SoGLRenderAction, SoGLTextureCoordinate3Element);
    SO_ENABLE(SoPickAction,     SoTextureCoordinate3Element);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Destructor
//
// Use: private

SoTextureCoordinate3::~SoTextureCoordinate3()
//
////////////////////////////////////////////////////////////////////////
{
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Handles actions.
//
// Use: extender

void
SoTextureCoordinate3::doAction(SoAction *action)
//
////////////////////////////////////////////////////////////////////////
{
    SoState *state = action->getState();

    if (! point.isIgnored() && point.getNum() > 0)
    SoTextureCoordinate3Element::set3(state, this,
                     point.getNum(), point.getValues(0));
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Does callback action thing.
//
// Use: extender

void
SoTextureCoordinate3::callback(SoCallbackAction *action)
//
////////////////////////////////////////////////////////////////////////
{
    SoTextureCoordinate3::doAction(action);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Does GL render action.
//
// Use: extender

void
SoTextureCoordinate3::GLRender(SoGLRenderAction *action)
//
////////////////////////////////////////////////////////////////////////
{
    SoState *state = action->getState();

    if (! point.isIgnored() && point.getNum() > 0){
    SoGLTextureCoordinate3Element::setTexGen(state, this, NULL);
    SoTextureCoordinate3Element::set3(state, this,
                     point.getNum(), point.getValues(0));
    }
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Does pick action...
//
// Use: protected

void
SoTextureCoordinate3::pick(SoPickAction *action)
//
////////////////////////////////////////////////////////////////////////
{
    SoTextureCoordinate3::doAction(action);
}

