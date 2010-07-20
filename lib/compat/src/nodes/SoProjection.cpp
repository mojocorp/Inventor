#include <Inventor/nodes/SoProjection.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/elements/SoProjectionElement.h>

SO_NODE_SOURCE(SoProjection)

void
SoProjection::initClass()
{
    SO__NODE_INIT_CLASS(SoProjection,"Projection", SoNode);

    SO_ENABLE(SoGLRenderAction, SoProjectionElement);

}

SoProjection::SoProjection()
{
    SO_NODE_CONSTRUCTOR(SoProjection);

    SO_NODE_ADD_FIELD(floatParameters,(0.f));
}

SoProjection::~SoProjection()
{

}

SbVec3f*
SoProjection::checkProjection( SoState *state, SoNode *node, int32_t numCoords, const SbVec3f *coords)
{
    return (SbVec3f*)coords;
}

void
SoProjection::checkProjection( SoState* state, SbVec3f &point, SbBool inverse)
{

}

bool
SoProjection::checkProjection( SoNode *node, SoAction *action, SoMFVec3f &coords )
{
    return true;
}

void
SoProjection::resetProjection( SoAction *action, SoMFVec3f &coords)
{

}

