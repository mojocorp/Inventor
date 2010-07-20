#ifndef _SO_GL_CONTEXT_H
#define _SO_GL_CONTEXT_H

#include <Inventor/misc/GL/glew.h>
#include <Inventor/misc/GL/wglew.h>

namespace SoGLEW
{
    GLenum glewContextInit (GLEWContext* ctx)
    {
        return ::glewContextInit(ctx);
    }
}

namespace SoWGLEW
{
    GLenum wglewContextInit (WGLEWContext* ctx)
    {
        return ::wglewContextInit(ctx);
    }
}

#endif
