#include <Inventor/nodes/SoDepthBuffer.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/elements/SoGLDepthBufferElement.h>

SO_NODE_SOURCE(SoDepthBuffer)

////////////////////////////////////////////////////////////////////////
//
// Description:
//    This initializes the SoDepthBuffer class.
//
// Use: internal

void
SoDepthBuffer::initClass()
//
////////////////////////////////////////////////////////////////////////
{
    SO__NODE_INIT_CLASS(SoDepthBuffer,"DepthBuffer", SoNode);

    SO_ENABLE(SoGLRenderAction, SoGLDepthBufferElement);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Constructor
//
// Use: public

SoDepthBuffer::SoDepthBuffer()
//
////////////////////////////////////////////////////////////////////////
{
    SO_NODE_CONSTRUCTOR(SoDepthBuffer);

    SO_NODE_ADD_FIELD(test,
                      (SoDepthBufferElement::getDefaultTest()));
    SO_NODE_ADD_FIELD(write,
                      (SoDepthBufferElement::getDefaultWrite()));
    SO_NODE_ADD_FIELD(function,
                      (SoDepthBufferElement::getDefaultFunction()));
    SO_NODE_ADD_FIELD(range,
                      (SoDepthBufferElement::getDefaultRange()));

    //
    // Set up static info for enumerated type fields
    //
    SO_NODE_DEFINE_ENUM_VALUE(DepthWriteFunction, NEVER);
    SO_NODE_DEFINE_ENUM_VALUE(DepthWriteFunction, ALWAYS);
    SO_NODE_DEFINE_ENUM_VALUE(DepthWriteFunction, LESS);
    SO_NODE_DEFINE_ENUM_VALUE(DepthWriteFunction, LEQUAL);
    SO_NODE_DEFINE_ENUM_VALUE(DepthWriteFunction, EQUAL);
    SO_NODE_DEFINE_ENUM_VALUE(DepthWriteFunction, GEQUAL);
    SO_NODE_DEFINE_ENUM_VALUE(DepthWriteFunction, GREATER);
    SO_NODE_DEFINE_ENUM_VALUE(DepthWriteFunction, NOTEQUAL);

    SO_NODE_SET_SF_ENUM_TYPE(function, DepthWriteFunction);

    isBuiltIn = TRUE;
}

////////////////////////////////////////////////////////////////////////
//
// Destructor
//

SoDepthBuffer::~SoDepthBuffer()
//
////////////////////////////////////////////////////////////////////////
{
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Traversal for GL rendering
//
// Use: extender

void
SoDepthBuffer::GLRender(SoGLRenderAction * action)
//
////////////////////////////////////////////////////////////////////////
{
    SoState * state = action->getState();

}
