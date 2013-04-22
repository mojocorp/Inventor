#include <Inventor/misc/SoGLContext.h>
#include <Inventor/errors/SoDebugError.h>

#include <map>

GLEWContext* SoGLContext::s_glewCurrentContext = NULL;

const GLEWContext* SoGLContext::getContext(uint32_t cacheId)
{
    static std::map<unsigned int, GLEWContext> s_glewContexts;

    if (s_glewContexts.find(cacheId) == s_glewContexts.end()) {
        GLenum err = glewContextInit(&s_glewContexts[cacheId]);
#ifdef DEBUG
        if (GLEW_OK != err)
        {
            /* Problem: glewInit failed, something is seriously wrong. */
            SoDebugError::post("SoGLContext:", "Error: %s", glewGetErrorString(err));
        }
#endif
    }
    return &s_glewContexts[cacheId];
}

void SoGLContext::setCurrentContext(uint32_t cacheId)
{
    static uint32_t s_cacheId = -1;

    if (s_cacheId == cacheId)
        return;

    s_glewCurrentContext = (GLEWContext*)getContext(cacheId);

    s_cacheId = cacheId;
}

#undef glewGetContext
#include "glew.c"
