#ifdef __APPLE__
#   define GL_SILENCE_DEPRECATION
#   define APIENTRY
#   include <OpenGL/gl.h>
#else
#   error "This is a forwarding header for MacOSX only!"
#endif
