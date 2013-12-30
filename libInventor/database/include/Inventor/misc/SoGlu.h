#ifndef _SO_GLU_
#define _SO_GLU_

#include <Inventor/SbBasic.h>

#if defined(SB_OS_MACX)
#   include <OpenGL/glu.h>
#elif defined(SB_OS_WIN)
#   ifndef APIENTRY
#       define GLEW_APIENTRY_DEFINED
#       if defined(__MINGW32__) || defined(__CYGWIN__)
#           define APIENTRY __stdcall
#       elif (_MSC_VER >= 800) || defined(_STDCALL_SUPPORTED) || defined(__BORLANDC__)
#           define APIENTRY __stdcall
#       else
#           define APIENTRY
#       endif
#   endif
#   include <GL/glu.h>
#   ifdef GLEW_APIENTRY_DEFINED
#       undef GLEW_APIENTRY_DEFINED
#       undef APIENTRY
#   endif
#else
#   include <GL/glu.h>
#endif

#endif /* _SO_GLU_ */
