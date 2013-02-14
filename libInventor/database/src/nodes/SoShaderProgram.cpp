#include <Inventor/nodes/SoShaderProgram.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/elements/SoGLShaderProgramElement.h>
#include "SbGLShader.h"
#include "SbGLShaderProgram.h"

SO_NODE_SOURCE(SoShaderProgram);

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Constructor
//
// Use: public

SoShaderProgram::SoShaderProgram()
//
////////////////////////////////////////////////////////////////////////
{
    SO_NODE_CONSTRUCTOR(SoShaderProgram);

    SO_NODE_ADD_FIELD(vertex, (""));
    SO_NODE_ADD_FIELD(geometry, (""));
    SO_NODE_ADD_FIELD(fragment, (""));
    shaderProgram = NULL;

    isBuiltIn = TRUE;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Destructor
//
// Use: private

SoShaderProgram::~SoShaderProgram()
//
////////////////////////////////////////////////////////////////////////
{
    delete shaderProgram;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    This initializes the SoShaderProgram class.
//
// Use: internal

void
SoShaderProgram::initClass()
//
////////////////////////////////////////////////////////////////////////
{
    SO__NODE_INIT_CLASS(SoShaderProgram, "ShaderProgram", SoNode);

    // Enable shader program element
    SO_ENABLE(SoGLRenderAction, SoGLShaderProgramElement);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Performs GL rendering.
//
// Use: extender
const char vertex_shader[] = {
    "void main(void)\n"
    "{\n"
    "    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;\n"
    "}"
};

const char fragment_shader[] = {
    "void main(void)\n"
    "{\n"
    "    gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);\n"
    "}"
};

void
SoShaderProgram::GLRender(SoGLRenderAction *action)
//
////////////////////////////////////////////////////////////////////////
{
#if 0
    if (!shaderProgram) {
        SbGLShader *vs = new SbGLShader(SbGLShader::Vertex);
        vs->compileSourceCode(vertex_shader);
        if (!vs->isCompiled()) SoDebugError::post("SoShaderProgram::Vertex", vs->log().getString());

        SbGLShader *fs = new SbGLShader(SbGLShader::Fragment);
        fs->compileSourceCode(fragment_shader);
        if (!fs->isCompiled()) SoDebugError::post("SoShaderProgram::Fragment", fs->log().getString());

        shaderProgram = new SbGLShaderProgram;
        shaderProgram->addShader(vs);
        shaderProgram->addShader(fs);
        shaderProgram->link();
        if(!shaderProgram->isLinked()) SoDebugError::post("SoShaderProgram::GLRender", shaderProgram->log().getString());
    }
#else
    if (!shaderProgram) {
        shaderProgram = new SbGLShaderProgram;

        for (int i=0; i<vertex.getNum(); i++) {
            SbGLShader *vs = new SbGLShader(SbGLShader::Vertex);
            vs->compileSourceCode(vertex[i]);
            if (!vs->isCompiled()) SoDebugError::post("SoShaderProgram::Vertex", vs->log().getString());
            else shaderProgram->addShader(vs);
        }

        for (int i=0; i<fragment.getNum(); i++) {
            SbGLShader *fs = new SbGLShader(SbGLShader::Fragment);
            fs->compileSourceCode(fragment[i]);
            if (!fs->isCompiled()) SoDebugError::post("SoShaderProgram::Fragment", fs->log().getString());
            else shaderProgram->addShader(fs);
        }
        shaderProgram->link();
        if(!shaderProgram->isLinked()) SoDebugError::post("SoShaderProgram::GLRender", shaderProgram->log().getString());
    }

#endif
    SoGLShaderProgramElement::set(action->getState(), this, shaderProgram);

    shaderProgram->setUniformValue("tex0", 0);
    shaderProgram->setUniformValue("tex1", 1);
}
