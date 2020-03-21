#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glad/glx.h>

#ifndef GLAD_IMPL_UTIL_C_
#define GLAD_IMPL_UTIL_C_

#ifdef _MSC_VER
#define GLAD_IMPL_UTIL_SSCANF sscanf_s
#else
#define GLAD_IMPL_UTIL_SSCANF sscanf
#endif

#endif /* GLAD_IMPL_UTIL_C_ */



static GLADloadfunc glad_global_on_demand_glx_loader_func = NULL;

void gladSetGLXOnDemandLoader(GLADloadfunc loader) {
    glad_global_on_demand_glx_loader_func = loader;
}

static GLADapiproc glad_glx_on_demand_loader(const char *name) {
    GLADapiproc result = NULL;
    if (result == NULL && glad_global_on_demand_glx_loader_func != NULL) {
        result = glad_global_on_demand_glx_loader_func(name);
    }
    /* this provokes a segmentation fault if there was no loader or no loader returned something useful */
    return result;
}


static GLXFBConfig * GLAD_API_PTR glad_on_demand_impl_glXChooseFBConfig(Display * dpy, int screen, const int * attrib_list, int * nelements) {
    glad_glXChooseFBConfig = (PFNGLXCHOOSEFBCONFIGPROC) glad_glx_on_demand_loader("glXChooseFBConfig");
    return glad_glXChooseFBConfig(dpy, screen, attrib_list, nelements);
}
PFNGLXCHOOSEFBCONFIGPROC glad_glXChooseFBConfig = glad_on_demand_impl_glXChooseFBConfig;
static XVisualInfo * GLAD_API_PTR glad_on_demand_impl_glXChooseVisual(Display * dpy, int screen, int * attribList) {
    glad_glXChooseVisual = (PFNGLXCHOOSEVISUALPROC) glad_glx_on_demand_loader("glXChooseVisual");
    return glad_glXChooseVisual(dpy, screen, attribList);
}
PFNGLXCHOOSEVISUALPROC glad_glXChooseVisual = glad_on_demand_impl_glXChooseVisual;
static void GLAD_API_PTR glad_on_demand_impl_glXCopyContext(Display * dpy, GLXContext src, GLXContext dst, unsigned long mask) {
    glad_glXCopyContext = (PFNGLXCOPYCONTEXTPROC) glad_glx_on_demand_loader("glXCopyContext");
    glad_glXCopyContext(dpy, src, dst, mask);
}
PFNGLXCOPYCONTEXTPROC glad_glXCopyContext = glad_on_demand_impl_glXCopyContext;
static GLXContext GLAD_API_PTR glad_on_demand_impl_glXCreateContext(Display * dpy, XVisualInfo * vis, GLXContext shareList, Bool direct) {
    glad_glXCreateContext = (PFNGLXCREATECONTEXTPROC) glad_glx_on_demand_loader("glXCreateContext");
    return glad_glXCreateContext(dpy, vis, shareList, direct);
}
PFNGLXCREATECONTEXTPROC glad_glXCreateContext = glad_on_demand_impl_glXCreateContext;
static GLXPixmap GLAD_API_PTR glad_on_demand_impl_glXCreateGLXPixmap(Display * dpy, XVisualInfo * visual, Pixmap pixmap) {
    glad_glXCreateGLXPixmap = (PFNGLXCREATEGLXPIXMAPPROC) glad_glx_on_demand_loader("glXCreateGLXPixmap");
    return glad_glXCreateGLXPixmap(dpy, visual, pixmap);
}
PFNGLXCREATEGLXPIXMAPPROC glad_glXCreateGLXPixmap = glad_on_demand_impl_glXCreateGLXPixmap;
static GLXContext GLAD_API_PTR glad_on_demand_impl_glXCreateNewContext(Display * dpy, GLXFBConfig config, int render_type, GLXContext share_list, Bool direct) {
    glad_glXCreateNewContext = (PFNGLXCREATENEWCONTEXTPROC) glad_glx_on_demand_loader("glXCreateNewContext");
    return glad_glXCreateNewContext(dpy, config, render_type, share_list, direct);
}
PFNGLXCREATENEWCONTEXTPROC glad_glXCreateNewContext = glad_on_demand_impl_glXCreateNewContext;
static GLXPbuffer GLAD_API_PTR glad_on_demand_impl_glXCreatePbuffer(Display * dpy, GLXFBConfig config, const int * attrib_list) {
    glad_glXCreatePbuffer = (PFNGLXCREATEPBUFFERPROC) glad_glx_on_demand_loader("glXCreatePbuffer");
    return glad_glXCreatePbuffer(dpy, config, attrib_list);
}
PFNGLXCREATEPBUFFERPROC glad_glXCreatePbuffer = glad_on_demand_impl_glXCreatePbuffer;
static GLXPixmap GLAD_API_PTR glad_on_demand_impl_glXCreatePixmap(Display * dpy, GLXFBConfig config, Pixmap pixmap, const int * attrib_list) {
    glad_glXCreatePixmap = (PFNGLXCREATEPIXMAPPROC) glad_glx_on_demand_loader("glXCreatePixmap");
    return glad_glXCreatePixmap(dpy, config, pixmap, attrib_list);
}
PFNGLXCREATEPIXMAPPROC glad_glXCreatePixmap = glad_on_demand_impl_glXCreatePixmap;
static GLXWindow GLAD_API_PTR glad_on_demand_impl_glXCreateWindow(Display * dpy, GLXFBConfig config, Window win, const int * attrib_list) {
    glad_glXCreateWindow = (PFNGLXCREATEWINDOWPROC) glad_glx_on_demand_loader("glXCreateWindow");
    return glad_glXCreateWindow(dpy, config, win, attrib_list);
}
PFNGLXCREATEWINDOWPROC glad_glXCreateWindow = glad_on_demand_impl_glXCreateWindow;
static void GLAD_API_PTR glad_on_demand_impl_glXDestroyContext(Display * dpy, GLXContext ctx) {
    glad_glXDestroyContext = (PFNGLXDESTROYCONTEXTPROC) glad_glx_on_demand_loader("glXDestroyContext");
    glad_glXDestroyContext(dpy, ctx);
}
PFNGLXDESTROYCONTEXTPROC glad_glXDestroyContext = glad_on_demand_impl_glXDestroyContext;
static void GLAD_API_PTR glad_on_demand_impl_glXDestroyGLXPixmap(Display * dpy, GLXPixmap pixmap) {
    glad_glXDestroyGLXPixmap = (PFNGLXDESTROYGLXPIXMAPPROC) glad_glx_on_demand_loader("glXDestroyGLXPixmap");
    glad_glXDestroyGLXPixmap(dpy, pixmap);
}
PFNGLXDESTROYGLXPIXMAPPROC glad_glXDestroyGLXPixmap = glad_on_demand_impl_glXDestroyGLXPixmap;
static void GLAD_API_PTR glad_on_demand_impl_glXDestroyPbuffer(Display * dpy, GLXPbuffer pbuf) {
    glad_glXDestroyPbuffer = (PFNGLXDESTROYPBUFFERPROC) glad_glx_on_demand_loader("glXDestroyPbuffer");
    glad_glXDestroyPbuffer(dpy, pbuf);
}
PFNGLXDESTROYPBUFFERPROC glad_glXDestroyPbuffer = glad_on_demand_impl_glXDestroyPbuffer;
static void GLAD_API_PTR glad_on_demand_impl_glXDestroyPixmap(Display * dpy, GLXPixmap pixmap) {
    glad_glXDestroyPixmap = (PFNGLXDESTROYPIXMAPPROC) glad_glx_on_demand_loader("glXDestroyPixmap");
    glad_glXDestroyPixmap(dpy, pixmap);
}
PFNGLXDESTROYPIXMAPPROC glad_glXDestroyPixmap = glad_on_demand_impl_glXDestroyPixmap;
static void GLAD_API_PTR glad_on_demand_impl_glXDestroyWindow(Display * dpy, GLXWindow win) {
    glad_glXDestroyWindow = (PFNGLXDESTROYWINDOWPROC) glad_glx_on_demand_loader("glXDestroyWindow");
    glad_glXDestroyWindow(dpy, win);
}
PFNGLXDESTROYWINDOWPROC glad_glXDestroyWindow = glad_on_demand_impl_glXDestroyWindow;
static const char * GLAD_API_PTR glad_on_demand_impl_glXGetClientString(Display * dpy, int name) {
    glad_glXGetClientString = (PFNGLXGETCLIENTSTRINGPROC) glad_glx_on_demand_loader("glXGetClientString");
    return glad_glXGetClientString(dpy, name);
}
PFNGLXGETCLIENTSTRINGPROC glad_glXGetClientString = glad_on_demand_impl_glXGetClientString;
static int GLAD_API_PTR glad_on_demand_impl_glXGetConfig(Display * dpy, XVisualInfo * visual, int attrib, int * value) {
    glad_glXGetConfig = (PFNGLXGETCONFIGPROC) glad_glx_on_demand_loader("glXGetConfig");
    return glad_glXGetConfig(dpy, visual, attrib, value);
}
PFNGLXGETCONFIGPROC glad_glXGetConfig = glad_on_demand_impl_glXGetConfig;
static GLXContext GLAD_API_PTR glad_on_demand_impl_glXGetCurrentContext(void) {
    glad_glXGetCurrentContext = (PFNGLXGETCURRENTCONTEXTPROC) glad_glx_on_demand_loader("glXGetCurrentContext");
    return glad_glXGetCurrentContext();
}
PFNGLXGETCURRENTCONTEXTPROC glad_glXGetCurrentContext = glad_on_demand_impl_glXGetCurrentContext;
static Display * GLAD_API_PTR glad_on_demand_impl_glXGetCurrentDisplay(void) {
    glad_glXGetCurrentDisplay = (PFNGLXGETCURRENTDISPLAYPROC) glad_glx_on_demand_loader("glXGetCurrentDisplay");
    return glad_glXGetCurrentDisplay();
}
PFNGLXGETCURRENTDISPLAYPROC glad_glXGetCurrentDisplay = glad_on_demand_impl_glXGetCurrentDisplay;
static GLXDrawable GLAD_API_PTR glad_on_demand_impl_glXGetCurrentDrawable(void) {
    glad_glXGetCurrentDrawable = (PFNGLXGETCURRENTDRAWABLEPROC) glad_glx_on_demand_loader("glXGetCurrentDrawable");
    return glad_glXGetCurrentDrawable();
}
PFNGLXGETCURRENTDRAWABLEPROC glad_glXGetCurrentDrawable = glad_on_demand_impl_glXGetCurrentDrawable;
static GLXDrawable GLAD_API_PTR glad_on_demand_impl_glXGetCurrentReadDrawable(void) {
    glad_glXGetCurrentReadDrawable = (PFNGLXGETCURRENTREADDRAWABLEPROC) glad_glx_on_demand_loader("glXGetCurrentReadDrawable");
    return glad_glXGetCurrentReadDrawable();
}
PFNGLXGETCURRENTREADDRAWABLEPROC glad_glXGetCurrentReadDrawable = glad_on_demand_impl_glXGetCurrentReadDrawable;
static int GLAD_API_PTR glad_on_demand_impl_glXGetFBConfigAttrib(Display * dpy, GLXFBConfig config, int attribute, int * value) {
    glad_glXGetFBConfigAttrib = (PFNGLXGETFBCONFIGATTRIBPROC) glad_glx_on_demand_loader("glXGetFBConfigAttrib");
    return glad_glXGetFBConfigAttrib(dpy, config, attribute, value);
}
PFNGLXGETFBCONFIGATTRIBPROC glad_glXGetFBConfigAttrib = glad_on_demand_impl_glXGetFBConfigAttrib;
static GLXFBConfig * GLAD_API_PTR glad_on_demand_impl_glXGetFBConfigs(Display * dpy, int screen, int * nelements) {
    glad_glXGetFBConfigs = (PFNGLXGETFBCONFIGSPROC) glad_glx_on_demand_loader("glXGetFBConfigs");
    return glad_glXGetFBConfigs(dpy, screen, nelements);
}
PFNGLXGETFBCONFIGSPROC glad_glXGetFBConfigs = glad_on_demand_impl_glXGetFBConfigs;
static __GLXextFuncPtr GLAD_API_PTR glad_on_demand_impl_glXGetProcAddress(const GLubyte * procName) {
    glad_glXGetProcAddress = (PFNGLXGETPROCADDRESSPROC) glad_glx_on_demand_loader("glXGetProcAddress");
    return glad_glXGetProcAddress(procName);
}
PFNGLXGETPROCADDRESSPROC glad_glXGetProcAddress = glad_on_demand_impl_glXGetProcAddress;
static void GLAD_API_PTR glad_on_demand_impl_glXGetSelectedEvent(Display * dpy, GLXDrawable draw, unsigned long * event_mask) {
    glad_glXGetSelectedEvent = (PFNGLXGETSELECTEDEVENTPROC) glad_glx_on_demand_loader("glXGetSelectedEvent");
    glad_glXGetSelectedEvent(dpy, draw, event_mask);
}
PFNGLXGETSELECTEDEVENTPROC glad_glXGetSelectedEvent = glad_on_demand_impl_glXGetSelectedEvent;
static XVisualInfo * GLAD_API_PTR glad_on_demand_impl_glXGetVisualFromFBConfig(Display * dpy, GLXFBConfig config) {
    glad_glXGetVisualFromFBConfig = (PFNGLXGETVISUALFROMFBCONFIGPROC) glad_glx_on_demand_loader("glXGetVisualFromFBConfig");
    return glad_glXGetVisualFromFBConfig(dpy, config);
}
PFNGLXGETVISUALFROMFBCONFIGPROC glad_glXGetVisualFromFBConfig = glad_on_demand_impl_glXGetVisualFromFBConfig;
static Bool GLAD_API_PTR glad_on_demand_impl_glXIsDirect(Display * dpy, GLXContext ctx) {
    glad_glXIsDirect = (PFNGLXISDIRECTPROC) glad_glx_on_demand_loader("glXIsDirect");
    return glad_glXIsDirect(dpy, ctx);
}
PFNGLXISDIRECTPROC glad_glXIsDirect = glad_on_demand_impl_glXIsDirect;
static Bool GLAD_API_PTR glad_on_demand_impl_glXMakeContextCurrent(Display * dpy, GLXDrawable draw, GLXDrawable read, GLXContext ctx) {
    glad_glXMakeContextCurrent = (PFNGLXMAKECONTEXTCURRENTPROC) glad_glx_on_demand_loader("glXMakeContextCurrent");
    return glad_glXMakeContextCurrent(dpy, draw, read, ctx);
}
PFNGLXMAKECONTEXTCURRENTPROC glad_glXMakeContextCurrent = glad_on_demand_impl_glXMakeContextCurrent;
static Bool GLAD_API_PTR glad_on_demand_impl_glXMakeCurrent(Display * dpy, GLXDrawable drawable, GLXContext ctx) {
    glad_glXMakeCurrent = (PFNGLXMAKECURRENTPROC) glad_glx_on_demand_loader("glXMakeCurrent");
    return glad_glXMakeCurrent(dpy, drawable, ctx);
}
PFNGLXMAKECURRENTPROC glad_glXMakeCurrent = glad_on_demand_impl_glXMakeCurrent;
static int GLAD_API_PTR glad_on_demand_impl_glXQueryContext(Display * dpy, GLXContext ctx, int attribute, int * value) {
    glad_glXQueryContext = (PFNGLXQUERYCONTEXTPROC) glad_glx_on_demand_loader("glXQueryContext");
    return glad_glXQueryContext(dpy, ctx, attribute, value);
}
PFNGLXQUERYCONTEXTPROC glad_glXQueryContext = glad_on_demand_impl_glXQueryContext;
static void GLAD_API_PTR glad_on_demand_impl_glXQueryDrawable(Display * dpy, GLXDrawable draw, int attribute, unsigned int * value) {
    glad_glXQueryDrawable = (PFNGLXQUERYDRAWABLEPROC) glad_glx_on_demand_loader("glXQueryDrawable");
    glad_glXQueryDrawable(dpy, draw, attribute, value);
}
PFNGLXQUERYDRAWABLEPROC glad_glXQueryDrawable = glad_on_demand_impl_glXQueryDrawable;
static Bool GLAD_API_PTR glad_on_demand_impl_glXQueryExtension(Display * dpy, int * errorb, int * event) {
    glad_glXQueryExtension = (PFNGLXQUERYEXTENSIONPROC) glad_glx_on_demand_loader("glXQueryExtension");
    return glad_glXQueryExtension(dpy, errorb, event);
}
PFNGLXQUERYEXTENSIONPROC glad_glXQueryExtension = glad_on_demand_impl_glXQueryExtension;
static const char * GLAD_API_PTR glad_on_demand_impl_glXQueryExtensionsString(Display * dpy, int screen) {
    glad_glXQueryExtensionsString = (PFNGLXQUERYEXTENSIONSSTRINGPROC) glad_glx_on_demand_loader("glXQueryExtensionsString");
    return glad_glXQueryExtensionsString(dpy, screen);
}
PFNGLXQUERYEXTENSIONSSTRINGPROC glad_glXQueryExtensionsString = glad_on_demand_impl_glXQueryExtensionsString;
static const char * GLAD_API_PTR glad_on_demand_impl_glXQueryServerString(Display * dpy, int screen, int name) {
    glad_glXQueryServerString = (PFNGLXQUERYSERVERSTRINGPROC) glad_glx_on_demand_loader("glXQueryServerString");
    return glad_glXQueryServerString(dpy, screen, name);
}
PFNGLXQUERYSERVERSTRINGPROC glad_glXQueryServerString = glad_on_demand_impl_glXQueryServerString;
static Bool GLAD_API_PTR glad_on_demand_impl_glXQueryVersion(Display * dpy, int * maj, int * min) {
    glad_glXQueryVersion = (PFNGLXQUERYVERSIONPROC) glad_glx_on_demand_loader("glXQueryVersion");
    return glad_glXQueryVersion(dpy, maj, min);
}
PFNGLXQUERYVERSIONPROC glad_glXQueryVersion = glad_on_demand_impl_glXQueryVersion;
static void GLAD_API_PTR glad_on_demand_impl_glXSelectEvent(Display * dpy, GLXDrawable draw, unsigned long event_mask) {
    glad_glXSelectEvent = (PFNGLXSELECTEVENTPROC) glad_glx_on_demand_loader("glXSelectEvent");
    glad_glXSelectEvent(dpy, draw, event_mask);
}
PFNGLXSELECTEVENTPROC glad_glXSelectEvent = glad_on_demand_impl_glXSelectEvent;
static void GLAD_API_PTR glad_on_demand_impl_glXSwapBuffers(Display * dpy, GLXDrawable drawable) {
    glad_glXSwapBuffers = (PFNGLXSWAPBUFFERSPROC) glad_glx_on_demand_loader("glXSwapBuffers");
    glad_glXSwapBuffers(dpy, drawable);
}
PFNGLXSWAPBUFFERSPROC glad_glXSwapBuffers = glad_on_demand_impl_glXSwapBuffers;
static void GLAD_API_PTR glad_on_demand_impl_glXUseXFont(Font font, int first, int count, int list) {
    glad_glXUseXFont = (PFNGLXUSEXFONTPROC) glad_glx_on_demand_loader("glXUseXFont");
    glad_glXUseXFont(font, first, count, list);
}
PFNGLXUSEXFONTPROC glad_glXUseXFont = glad_on_demand_impl_glXUseXFont;
static void GLAD_API_PTR glad_on_demand_impl_glXWaitGL(void) {
    glad_glXWaitGL = (PFNGLXWAITGLPROC) glad_glx_on_demand_loader("glXWaitGL");
    glad_glXWaitGL();
}
PFNGLXWAITGLPROC glad_glXWaitGL = glad_on_demand_impl_glXWaitGL;
static void GLAD_API_PTR glad_on_demand_impl_glXWaitX(void) {
    glad_glXWaitX = (PFNGLXWAITXPROC) glad_glx_on_demand_loader("glXWaitX");
    glad_glXWaitX();
}
PFNGLXWAITXPROC glad_glXWaitX = glad_on_demand_impl_glXWaitX;


 

