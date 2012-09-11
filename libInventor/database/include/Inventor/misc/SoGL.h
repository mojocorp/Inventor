#ifndef _SO_GL_
#define _SO_GL_

#include <Inventor/misc/SoGLContext.h>

#define glewGetContext() SoGLContext::getCurrentContext()

#ifdef SB_OS_MACX
#   include <OpenGL/glu.h>
#else
#   include <GL/glu.h>
#endif

#ifdef SB_HAS_X11
#   include <GL/glx.h>
#endif

#endif /* _SO_GL_ */

