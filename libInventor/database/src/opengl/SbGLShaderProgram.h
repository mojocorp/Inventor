#ifndef _SB_GLSHADERPROGRAM_
#define _SB_GLSHADERPROGRAM_

#include "SbGLShader.h"

#include <Inventor/SbBasic.h>
#include <Inventor/SbString.h>
#include <Inventor/misc/SoGL.h>

#include <vector>

class INVENTOR_API SbGLShaderProgram {
public:
    /// Constructs a new shader program.
    SbGLShaderProgram();

    /// Deletes this shader program.
    virtual	~SbGLShaderProgram();

    /// Adds a compiled shader to this shader program.
    bool addShader(SbGLShader * shader);
    bool addShaderFromSourceCode (SbGLShader::ShaderType type, const SbString & source);
    bool addShaderFromSourceFile (SbGLShader::ShaderType type, const SbString & fileName);

    void removeAllShaders();
    void removeShader(SbGLShader * shader );

    const std::vector<SbGLShader*> & shaders() const;

    /// Binds this shader program to the active context and makes it the current shader program.
    bool bind();
    void release();

    /// Returns true if this shader program has been linked.
    bool isLinked () const;
    virtual bool link();

    SbString log() const;

    /// Returns the OpenGL identifier associated with this shader program.
    GLuint programId() const;

    void setGeometryInputType( GLenum inputType );
    GLenum geometryInputType () const;

    void setGeometryOutputType( GLenum outputType );
    GLenum geometryOutputType () const;

    int	maxGeometryOutputVertices() const;
    void setGeometryOutputVertexCount( int count );
    int	geometryOutputVertexCount () const;

    ///
    int	getUniformLocation(const SbString & name) const;

    ///
    void setUniformValue(const SbString & name, GLfloat value);
    //void setUniformValue(const SbString & name, const GLfloat[2][2] value);
    //void setUniformValue(const SbString & name, const GLfloat[3][3] value);
    //void setUniformValue(const SbString & name, const GLfloat[4][4] value);
    void setUniformValue(const SbString & name, GLint value);

    ///
    int	getAttributeLocation(const SbString & name) const;
    void bindAttributeLocation(const SbString & name, int location);
private:
    GLuint                   mProgramId;
    bool                     mLinked;
    std::vector<SbGLShader*> mShaders;

    GLint mGeometryInputType;
    GLint mGeometryOutputType;
    GLint mGeometryVertexCount;
};

#endif /* _SB_GLSHADERPROGRAM_ */
