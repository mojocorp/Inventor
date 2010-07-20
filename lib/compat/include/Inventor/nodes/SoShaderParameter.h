#ifndef _SO_SHADER_PARAMETER_
#define _SO_SHADER_PARAMETER_

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/nodes/SoNode.h>

#include <Inventor/fields/SoSFString.h>
#include <Inventor/fields/SoSFInt32.h>

class SoShaderParameter : public SoNode {

    SO_NODE_ABSTRACT_HEADER(SoShaderParameter);

public:
    SoSFString name;
    SoSFInt32 identifier;

protected:
    SoShaderParameter();
    virtual ~SoShaderParameter();
};

class  SoUniformShaderParameter : public SoShaderParameter {

    SO_NODE_ABSTRACT_HEADER(SoUniformShaderParameter);

public:

};

class  SoVertexShaderParameter : public SoShaderParameter {

    SO_NODE_ABSTRACT_HEADER(SoVertexShaderParameter);

public:

};


class  SoShaderParameter1i : public SoUniformShaderParameter {

    SO_NODE_HEADER(SoShaderParameter1i);

public:
    SoSFInt32 value;

    SoShaderParameter1i();
};

#endif /* _SO_SHADER_PARAMETER_ */
