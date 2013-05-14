#ifndef _SO_GL_
#define _SO_GL_

#include <Inventor/SbBasic.h>

#ifdef SB_OS_WIN
#   ifndef WINGDIAPI
#       define SO_WINGDIAPI_DEFINED
#       define WINGDIAPI __declspec(dllimport)
#   endif
#   ifndef APIENTRY
#       define SO_APIENTRY_DEFINED
#       define APIENTRY __stdcall
#   endif
#endif

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

#ifdef SO_WINGDIAPI_DEFINED
#   undef SO_WINGDIAPI_DEFINED
#   undef WINGDIAPI
#endif

#ifdef SO_APIENTRY_DEFINED
#   undef SO_APIENTRY_DEFINED
#   undef APIENTRY
#endif

#endif //_SO_GL_

