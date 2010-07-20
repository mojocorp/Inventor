#ifndef _SO_SHADER_PROGRAM_
#define _SO_SHADER_PROGRAM_

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/nodes/SoNode.h>
#include <Inventor/fields/SoMFNode.h>

class SoShaderProgram : public SoNode {
    SO_NODE_HEADER(SoShaderProgram);
public:
    SoShaderProgram();

    SoMFNode shaderObject;
};

#endif /* _SO_SHADER_PROGRAM_ */
