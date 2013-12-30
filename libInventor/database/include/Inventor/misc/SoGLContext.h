#ifndef _SO_GL_CONTEXT_
#define _SO_GL_CONTEXT_

#include <Inventor/SbBasic.h>

#ifndef GLEW_MX
#   define GLEW_MX
#endif
#define GLEW_NO_GLU

#include <Inventor/misc/GL/glew.h>

SoEXTENDER class INVENTOR_API SoGLContext
{
public:
    ///
    static const GLEWContext* getCurrentContext() {
        return s_glewCurrentContext;
    }

    ///
    static void setCurrentContext(uint32_t cacheId);

    ///
    static const GLEWContext* getContext(uint32_t cacheId);

private:
    static GLEWContext *s_glewCurrentContext;
};

#endif /* _SO_GL_CONTEXT_ */
