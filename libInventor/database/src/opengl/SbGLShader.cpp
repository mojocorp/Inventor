#include "SbGLShader.h"

#include <Inventor/SbFile.h>
#include <Inventor/errors/SoDebugError.h>

SbGLShader::SbGLShader(SbGLShader::ShaderType type)
    : mShaderType(type),
      mCompiled(false)
{
    switch (mShaderType) {
    case SbGLShader::Vertex:
        mShaderId = glCreateShader(GL_VERTEX_SHADER);
        break;
    case SbGLShader::Geometry:
        mShaderId = glCreateShader(GL_GEOMETRY_SHADER_EXT);
        break;
    case SbGLShader::Fragment:
        mShaderId = glCreateShader(GL_FRAGMENT_SHADER);
        break;
    default:
        SoDebugError::post("SbGLShader::SbGLShader", "Logic error");
    }

    if (!mShaderId) {
        SoDebugError::post("SbGLShader::SbGLShader", "could not create shader");
    }
}

SbGLShader::~SbGLShader()
{
    glDeleteShader(mShaderId);
}

bool
SbGLShader::compileSourceCode( const SbString & source )
{
    if (!mShaderId)
        return false;

    if (!mCompiled) {
        const char * src = source.getString();
        glShaderSource(mShaderId, 1, &src, NULL);
        glCompileShader(mShaderId);

        GLint value = 0;
        glGetShaderiv(mShaderId, GL_COMPILE_STATUS, &value);
        mCompiled = (value != 0);
    }

    return mCompiled;
}

bool
SbGLShader::compileSourceFile( const SbString & fileName )
{
    SbFile file;
    if (!file.open(fileName, "rt")) {
        SoDebugError::post("SbGLShader::compileSourceFile", "Unable to open file %s", fileName.getString());
        return false;
    }
    size_t size = SbFile::size(fileName);
    char *source = new char [size];
    file.read(source, 1, size);
    compileSourceCode(SbString::fromLatin1(source));
    delete [] source;
    return true;
}

bool
SbGLShader::isCompiled() const
{
    return mCompiled;
}

SbString
SbGLShader::log() const
{
    GLint value = 0;
    glGetShaderiv(mShaderId, GL_INFO_LOG_LENGTH, &value);

    SbString log;
    if (value > 1) {
        char *logbuf = new char [value];

        GLint len;
        glGetShaderInfoLog(mShaderId, value, &len, logbuf);

        log = SbString::fromLatin1(logbuf);

        delete [] logbuf;
    }
    return log;
}

GLuint
SbGLShader::shaderId() const
{
    return mShaderId;
}

SbGLShader::ShaderType
SbGLShader::shaderType() const
{
    return mShaderType;
}

SbString
SbGLShader::sourceCode() const
{
    SbString src;

    if (mShaderId) {
        GLint size = 0;
        glGetShaderiv(mShaderId, GL_SHADER_SOURCE_LENGTH, &size);

        if (size > 0) {
            GLint len = 0;
            char *source = new char [size];

            glGetShaderSource(mShaderId, size, &len, source);

            src = SbString::fromLatin1(source);

            delete [] source;
        }
    }
    return src;
}
