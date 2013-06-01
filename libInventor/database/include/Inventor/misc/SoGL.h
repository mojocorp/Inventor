#ifndef _SO_GL_
#define _SO_GL_

//#include <Inventor/misc/SoGLContext.h>

//#define glewGetContext() SoGLContext::getCurrentContext()

#include <Inventor/misc/GL/gl.h>

#ifdef SB_OS_MACX
#   include <OpenGL/glu.h>
#else
#   include <GL/glu.h>
#endif

#endif /* _SO_GL_ */

