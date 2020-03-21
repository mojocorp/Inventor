#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glad/wgl.h>

#ifndef GLAD_IMPL_UTIL_C_
#define GLAD_IMPL_UTIL_C_

#ifdef _MSC_VER
#define GLAD_IMPL_UTIL_SSCANF sscanf_s
#else
#define GLAD_IMPL_UTIL_SSCANF sscanf
#endif

#endif /* GLAD_IMPL_UTIL_C_ */



static GLADloadfunc glad_global_on_demand_wgl_loader_func = NULL;

void gladSetWGLOnDemandLoader(GLADloadfunc loader) {
    glad_global_on_demand_wgl_loader_func = loader;
}

static GLADapiproc glad_wgl_on_demand_loader(const char *name) {
    GLADapiproc result = NULL;
    if (result == NULL && glad_global_on_demand_wgl_loader_func != NULL) {
        result = glad_global_on_demand_wgl_loader_func(name);
    }
    /* this provokes a segmentation fault if there was no loader or no loader returned something useful */
    return result;
}


static const char * GLAD_API_PTR glad_on_demand_impl_wglGetExtensionsStringARB(HDC hdc) {
    glad_wglGetExtensionsStringARB = (PFNWGLGETEXTENSIONSSTRINGARBPROC) glad_wgl_on_demand_loader("wglGetExtensionsStringARB");
    return glad_wglGetExtensionsStringARB(hdc);
}
PFNWGLGETEXTENSIONSSTRINGARBPROC glad_wglGetExtensionsStringARB = glad_on_demand_impl_wglGetExtensionsStringARB;
static const char * GLAD_API_PTR glad_on_demand_impl_wglGetExtensionsStringEXT(void) {
    glad_wglGetExtensionsStringEXT = (PFNWGLGETEXTENSIONSSTRINGEXTPROC) glad_wgl_on_demand_loader("wglGetExtensionsStringEXT");
    return glad_wglGetExtensionsStringEXT();
}
PFNWGLGETEXTENSIONSSTRINGEXTPROC glad_wglGetExtensionsStringEXT = glad_on_demand_impl_wglGetExtensionsStringEXT;


 

