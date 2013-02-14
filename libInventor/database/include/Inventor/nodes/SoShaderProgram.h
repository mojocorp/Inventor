#ifndef  _SO_SHADER_PROGRAM_
#define  _SO_SHADER_PROGRAM_

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/fields/SoMFString.h>

class SbGLShaderProgram;

class INVENTOR_API SoShaderProgram : public SoNode {
    SO_NODE_HEADER(SoShaderProgram);

public:
    /// Constructor
    SoShaderProgram();

    SoMFString vertex;
    SoMFString geometry;
    SoMFString fragment;

SoEXTENDER public:
    virtual void GLRender(SoGLRenderAction *action);

SoINTERNAL public:
    static void	initClass();

protected:
    virtual ~SoShaderProgram();

private:
    SbGLShaderProgram *shaderProgram;
};

#endif /* _SO_SHADER_PROGRAM_ */
