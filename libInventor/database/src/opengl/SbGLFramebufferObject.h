#ifndef _SB_GL_FRAMEBUFFER_OBJECT_
#define _SB_GL_FRAMEBUFFER_OBJECT_

#include <Inventor/SbBasic.h>
#include <Inventor/SbImage.h>
#include <Inventor/SbVec2s.h>
#include <Inventor/misc/SoGL.h>

class INVENTOR_API SbGLFramebufferObject {
public:
    ///
    SbGLFramebufferObject(const SbVec2s & size, GLenum format = GL_RGBA8);

    ///
    virtual	~SbGLFramebufferObject();

    ///
    void resize(const SbVec2s & size);

    ///
    void bind();

    ///
    GLuint objectId() const;

    ///
    bool isValid() const;

    ///
    void release();

    ///
    SbImage toImage(SbImage::Format fmt = SbImage::Format_RGB24) const;
private:
    GLuint  m_framebuffer;
    GLuint  m_colorbuffer;
    GLuint  m_depthbuffer;
    SbVec2s m_size;
    GLenum  m_format;
};

#endif /* _SB_GL_FRAMEBUFFER_OBJECT_ */
