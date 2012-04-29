#ifndef _SO_DEPTH_BUFFER_
#define _SO_DEPTH_BUFFER_

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/nodes/SoNode.h>
#include <Inventor/fields/SoSFBool.h>
#include <Inventor/fields/SoSFEnum.h>
#include <Inventor/fields/SoSFVec2f.h>
#include <Inventor/elements/SoDepthBufferElement.h>

class INVENTOR_API SoDepthBuffer : public SoNode {
    SO_NODE_HEADER(SoDepthBuffer);
public:
    SoDepthBuffer();

    enum DepthWriteFunction {
        NEVER = SoDepthBufferElement::NEVER,
        ALWAYS = SoDepthBufferElement::ALWAYS,
        LESS = SoDepthBufferElement::LESS,
        LEQUAL = SoDepthBufferElement::LEQUAL,
        EQUAL = SoDepthBufferElement::EQUAL,
        GEQUAL = SoDepthBufferElement::GEQUAL,
        GREATER = SoDepthBufferElement::GREATER,
        NOTEQUAL = SoDepthBufferElement::NOTEQUAL
    };

    SoSFBool    test;
    SoSFBool    write;
    SoSFEnum    function;
    SoSFVec2f   range;
    SoSFBool    clearBuffer;

SoINTERNAL public:
        static void initClass();
protected:
    virtual void GLRender(SoGLRenderAction * action);

    virtual ~SoDepthBuffer();
};

#endif /* _SO_DEPTH_BUFFER_ */

