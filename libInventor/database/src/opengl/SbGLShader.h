#ifndef _SB_GLSHADER_
#define _SB_GLSHADER_

#include <Inventor/SbBasic.h>
#include <Inventor/SbString.h>
#include <Inventor/SbRefPtr.h>
#include <Inventor/misc/SoGL.h>

class INVENTOR_API SbGLShader : public SbRefCounted {
public:
    enum ShaderType
    {
        Vertex,
        Fragment,
        Geometry
    };

    ///
    SbGLShader(SbGLShader::ShaderType type);

    ///
    bool compileSourceCode(const SbString & source);

    ///
    bool compileSourceFile(const SbString & fileName);

    ///
    bool isCompiled() const;

    ///
    SbString log() const;

    ///
    GLuint shaderId() const;

    ///
    SbGLShader::ShaderType shaderType() const;

    ///
    SbString sourceCode() const;

private:
    ///
    virtual	~SbGLShader();

    // Disables the use of copy constructors and assignment operators
    SbGLShader(const SbGLShader &);
    SbGLShader &operator=(const SbGLShader &);

    SbGLShader::ShaderType mShaderType;
    GLuint                 mShaderId;
    bool                   mCompiled;
};

#endif /* _SB_GLSHADER_ */
