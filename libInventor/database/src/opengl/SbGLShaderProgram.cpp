#include "SbGLShaderProgram.h"
#include "SbGLShader.h"

#include <Inventor/errors/SoDebugError.h>

#include <algorithm>

SbGLShaderProgram::SbGLShaderProgram()
    : mLinked(false),
      mGeometryInputType(0),
      mGeometryOutputType(0),
      mGeometryVertexCount(0)
{
    mProgramId = glCreateProgram();
    if (!mProgramId) {
        SoDebugError::post("SbGLShaderProgram::SbGLShaderProgram", "could not create shader program");
    }
}

SbGLShaderProgram::~SbGLShaderProgram()
{
    removeAllShaders();

    glDeleteProgram(mProgramId);
}

bool
SbGLShaderProgram::addShader(SbGLShader *shader)
{
    if (std::find(mShaders.begin(), mShaders.end(), shader) != mShaders.end())
        return false;

    mShaders.push_back(shader);

    shader->ref();

    glAttachShader(mProgramId, shader->shaderId());
    mLinked = false;

    return true;
}

bool
SbGLShaderProgram::addShaderFromSourceCode (SbGLShader::ShaderType type, const SbString & source)
{
    SbGLShader *shader = new SbGLShader(type);

    if (!shader->compileSourceCode(source)) {
        shader->unref();
        return false;

    }
    return addShader(shader);
}

bool
SbGLShaderProgram::addShaderFromSourceFile (SbGLShader::ShaderType type, const SbString & fileName)
{
    SbGLShader *shader = new SbGLShader(type);

    if (!shader->compileSourceFile(fileName)) {
        shader->unref();
        return false;

    }
    return addShader(shader);
}

void
SbGLShaderProgram::removeAllShaders()
{
    for (size_t i=0; i<mShaders.size(); i++) {
        glDetachShader(mProgramId, mShaders[i]->shaderId());

        mShaders[i]->unref();
    }
    mShaders.clear();

    mLinked = false;
}

void
SbGLShaderProgram::removeShader(SbGLShader *shader)
{
    if (!shader)
        return;

    std::remove(mShaders.begin(), mShaders.end(), shader);
    glDetachShader(mProgramId, shader->shaderId());
    shader->unref();
    mLinked = false;
}

const std::vector<SbGLShader*> &
SbGLShaderProgram::shaders() const
{
    return mShaders;
}

bool
SbGLShaderProgram::bind()
{
    if (!mProgramId)
        return false;

    if (!link()) {
#ifdef DEBUG
        SoDebugError::post("SbGLShaderProgram::bind", "The program must be linked before calling bind.");
#endif
        return false;
    }

    glUseProgram(mProgramId);
    return true;
}

void
SbGLShaderProgram::release()
{
    glUseProgram(0);
}

bool
SbGLShaderProgram::isLinked() const
{
    return mLinked;
}

bool
SbGLShaderProgram::link()
{
    if (!mProgramId || mShaders.empty())
        return false;

    if (!mLinked) {
        for (size_t i=0; i<mShaders.size(); i++) {
            if (mShaders[i]->shaderType() == SbGLShader::Geometry) {
                glProgramParameteriEXT(mProgramId, GL_GEOMETRY_INPUT_TYPE_EXT, mGeometryInputType);
                glProgramParameteriEXT(mProgramId, GL_GEOMETRY_OUTPUT_TYPE_EXT, mGeometryOutputType);
                glProgramParameteriEXT(mProgramId, GL_GEOMETRY_VERTICES_OUT_EXT, mGeometryVertexCount);
                break;
            }
        }

        glLinkProgram(mProgramId);

        GLint value = 0;
        glGetProgramiv(mProgramId, GL_LINK_STATUS, &value);

        mLinked = (value != 0);
    }

    return mLinked;
}

SbString
SbGLShaderProgram::log() const
{
    GLint value = 0;
    glGetProgramiv(mProgramId, GL_INFO_LOG_LENGTH, &value);

    SbString log;

    if (value > 1) {

        char *logbuf = new char [value];

        GLint len;
        glGetProgramInfoLog(mProgramId, value, &len, logbuf);

        log = SbString::fromLatin1(logbuf);

        delete [] logbuf;
    }
    return log;
}

GLuint
SbGLShaderProgram::programId() const
{
    return mProgramId;
}

void
SbGLShaderProgram::setGeometryInputType( GLenum inputType )
{
    mGeometryInputType = inputType;
}

GLenum
SbGLShaderProgram::geometryInputType () const
{
    return mGeometryInputType;
}

void
SbGLShaderProgram::setGeometryOutputType( GLenum outputType )
{
    mGeometryOutputType = outputType;
}

GLenum
SbGLShaderProgram::geometryOutputType() const
{
    return mGeometryOutputType;
}

int
SbGLShaderProgram::maxGeometryOutputVertices() const
{
    GLint n;
    glGetIntegerv(GL_MAX_GEOMETRY_OUTPUT_VERTICES_EXT, &n);

    return n;
}

void
SbGLShaderProgram::setGeometryOutputVertexCount( int count )
{
    mGeometryVertexCount = count;
}

int
SbGLShaderProgram::geometryOutputVertexCount() const
{
    return mGeometryVertexCount;
}

int
SbGLShaderProgram::getUniformLocation(const SbString & name) const
{
    if (!mLinked) {
#ifdef DEBUG
        SoDebugError::post("SbGLShaderProgram::getUniformLocation",
                           "The program must be linked before calling getUniformLocation.");
#endif
        return -1;
    }

    return glGetUniformLocation(mProgramId, name.getString());
}

void
SbGLShaderProgram::setUniformValue(const SbString & name, GLint value)
{
#ifdef DEBUG
    GLuint program;
    glGetIntegerv(GL_CURRENT_PROGRAM, (GLint*) &program);
    if (program != mProgramId)
        SoDebugError::post("SbGLShaderProgram::setUniformValue",
                           "The program must be bound before calling setUniformValue.");
#endif
    glUniform1i(getUniformLocation(name), value);
}

void
SbGLShaderProgram::setUniformValue(const SbString & name, GLfloat value)
{
    glUniform1fv(getUniformLocation(name), 1, &value);
}

int
SbGLShaderProgram::getAttributeLocation(const SbString & name) const
{
    if (mLinked) {
        return glGetAttribLocation(mProgramId, name.getString());
    }
    return -1;
}

void
SbGLShaderProgram::bindAttributeLocation(const SbString & name, int location)
{
    glBindAttribLocation(mProgramId, location, name.getString());
    mLinked = false;
}
