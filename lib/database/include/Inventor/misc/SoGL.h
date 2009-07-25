#ifndef _SO_GL_
#define _SO_GL_

#include <Inventor/SbBasic.h>

#ifdef SB_OS_MACX
#   include <OpenGL/gl.h>
#   include <OpenGL/glu.h>
#else
#   include <GL/gl.h>
#   include <GL/glu.h>
#endif

#ifdef SB_HAS_X11
#   include <GL/glx.h>
#endif

#endif //_SO_GL_

