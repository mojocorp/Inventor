#ifndef _SB_GL_CONTEXT_
#define _SB_GL_CONTEXT_

#include <Inventor/SbBasic.h>

#if defined(SB_OS_WIN)
#   include <windows.h>
#elif defined(SB_OS_MACX)
#   include <ApplicationServices/ApplicationServices.h>
#   include <OpenGL/OpenGL.h>
#elif defined(SB_OS_LINUX)
#   include <X11/Xlib.h>
#   include <GL/glx.h>
#else
#   error "SoOffscreenRenderer has not been ported to this OS"
#endif

class SbGLContext
{
public:
    /// Initializes a newly allocated rendering context.
    /// To issue OpenGL commands to this context, you must first make it the current drawing context by calling makeContextCurrent.
    SbGLContext();

    /// Delete the context.
    ~SbGLContext();

    /// Makes this context the current rendering context for the calling thread.
    bool makeContextCurrent();

private:

#if defined(SB_OS_WIN)
    HWND  m_canvasWindow;
    HDC   m_canvasDC;
    HGLRC m_contextObj;
#elif defined(SB_OS_MACX)
    CGLContextObj m_contextObj;
#elif defined(SB_OS_LINUX)
    Display*   m_display;
    GLXContext m_contextObj;
    GLXPbuffer m_pbuffer;
#else
#  error "SbGLContext has not been ported to this OS"
#endif
};

#endif /* _SB_GL_CONTEXT_ */
