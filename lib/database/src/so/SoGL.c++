#include <glad/gl.h>
#include <machine.h>
#include <cstddef>

#if defined(SB_OS_WIN)
#include <windows.h>
#include <glad/wgl.h>
#else
#include <glad/glx.h>
#include <dlfcn.h>
#endif

namespace {
void *
SoGlGetProcAddress(const char *name) {
#if defined(SB_OS_WIN)
    return wglGetProcAddress((LPCSTR)name);
#else
    static void *handle = dlopen(NULL, RTLD_LAZY | RTLD_LOCAL);

    if (handle) {
        return dlsym(handle, (const char *)name);
    }
    return NULL;
#endif
}
} // namespace

void
SoGlInit() {
    gladSetGLOnDemandLoader((GLADloadfunc)SoGlGetProcAddress);
#if defined(SB_OS_WIN)
    gladSetWGLOnDemandLoader((GLADloadfunc)SoGlGetProcAddress);
#endif
#if defined(SB_OS_MACX) || defined(SB_OS_LINUX)
    gladSetGLXOnDemandLoader((GLADloadfunc)SoGlGetProcAddress);
#endif
}
