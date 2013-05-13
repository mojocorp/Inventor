#include "SbGLFramebufferObject.h"
#include <Inventor/errors/SoDebugError.h>

SbGLFramebufferObject::SbGLFramebufferObject(const SbVec2s & size, GLenum format)
    : m_size(size), m_format(format)
{
    // Generate the framebuffer object
    glGenFramebuffersEXT(1, &m_framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);

    // Generate the render buffers
    glGenRenderbuffersEXT(1, &m_colorbuffer);
    glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, m_colorbuffer);
    glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_RENDERBUFFER_EXT, m_colorbuffer);
    glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, m_format, m_size[0], m_size[1]);

    glGenRenderbuffersEXT(1, &m_depthbuffer);
    glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, m_depthbuffer);
    glFramebufferRenderbufferEXT(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthbuffer);
    glRenderbufferStorageEXT(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32, m_size[0], m_size[1]);
}

SbGLFramebufferObject::~SbGLFramebufferObject()
{
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_framebuffer);

    glDeleteRenderbuffersEXT(1, &m_colorbuffer);
    glDeleteRenderbuffersEXT(1, &m_depthbuffer);
    glDeleteFramebuffersEXT(1, &m_framebuffer);
}

void
SbGLFramebufferObject::resize(const SbVec2s & size)
{
    if (size == m_size)
        return;

    m_size = size;

    glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, m_colorbuffer);
    glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, m_format, m_size[0], m_size[1]);

    glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, m_depthbuffer);
    glRenderbufferStorageEXT(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32, m_size[0], m_size[1]);
}

void
SbGLFramebufferObject::bind()
{
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_framebuffer);
}

GLuint
SbGLFramebufferObject::objectId() const
{
    return m_framebuffer;
}

bool
SbGLFramebufferObject::isValid() const
{
    GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
    switch(status) {
    case GL_FRAMEBUFFER_COMPLETE_EXT:
        return true;
    case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT:
        SoDebugError::post("SbGLFramebufferObject::isValid", "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT");
        return false;
    case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT:
        SoDebugError::post("SbGLFramebufferObject::isValid", "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT");
        return false;
    case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
        SoDebugError::post("SbGLFramebufferObject::isValid", "GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT");
        return false;
    case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
        SoDebugError::post("SbGLFramebufferObject::isValid", "GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT");
        return false;
    case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT:
        SoDebugError::post("SbGLFramebufferObject::isValid", "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT");
        return false;
    case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT:
        SoDebugError::post("SbGLFramebufferObject::isValid", "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT");
        return false;
    case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
        SoDebugError::post("SbGLFramebufferObject::isValid", "GL_FRAMEBUFFER_UNSUPPORTED_EXT");
        return false;
    default:
        SoDebugError::post("SbGLFramebufferObject::isValid", "Unknown ERROR");
        return false;
    }
    return false;
}

void
SbGLFramebufferObject::release()
{
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}

SbImage
SbGLFramebufferObject::toImage(SbImage::Format fmt) const
{
    SbImage img(m_size, fmt, NULL);

    // Format in memory
    GLenum format = GL_INVALID_VALUE;
    switch(fmt)
    {
    case SbImage::Format_Luminance:
        format = GL_LUMINANCE;
        break;
    case SbImage::Format_Luminance_Alpha:
        format = GL_LUMINANCE_ALPHA;
        break;
    case SbImage::Format_RGB24:
        format = GL_RGB;
        break;
    case SbImage::Format_RGBA32:
        format = GL_RGBA;
        break;
    default:
        SoDebugError::post("SbGLFramebufferObject::toImage",
                           "Invalid image format");
        break;
    }

    glPushAttrib( GL_PIXEL_MODE_BIT );
    glReadBuffer( GL_COLOR_ATTACHMENT0_EXT);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glReadPixels(0, 0, m_size[0], m_size[1], (GLenum)format, GL_UNSIGNED_BYTE, (GLvoid *)img.getBytes());
    glPopAttrib();

    return img;
}
