#ifndef _SO_GL_
#define _SO_GL_

#include <machine.h>

#ifdef SB_OS_MACX
#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#endif /* _SO_GL_ */
