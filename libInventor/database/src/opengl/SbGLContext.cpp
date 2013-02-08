#include "SbGLContext.h"
#include <Inventor/errors/SoDebugError.h>

SbGLContext::SbGLContext()
{
#if defined(SB_OS_WIN)
    m_canvasWindow = 0;
    m_canvasDC = 0;
    m_contextObj = 0;

    WNDCLASS wc;
    if (!GetClassInfo(GetModuleHandle(0), L"CANVASGL", &wc)) {
        ZeroMemory(&wc, sizeof(WNDCLASS));
        wc.style = CS_OWNDC;
        wc.hInstance = GetModuleHandle(0);
        wc.lpfnWndProc = DefWindowProc;
        wc.lpszClassName = L"CANVASGL";

        if (!RegisterClass(&wc)) {
            SoDebugError::post("SbGLContext", "RegisterClass failed\n");
            return;
        }
    }

    m_canvasWindow = CreateWindow(L"CANVASGL", L"CANVASGL",
                                  WS_CAPTION,
                                  CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
                                  CW_USEDEFAULT, 0, 0, GetModuleHandle(0), 0);
    if (!m_canvasWindow) {
        SoDebugError::post("SbGLContext", "CreateWindow failed\n");
        return;
    }

    // get the device context
    m_canvasDC = GetDC(m_canvasWindow);
    if (!m_canvasDC) {
        SoDebugError::post("SbGLContext", "GetDC failed\n");
        return;
    }

    // find default pixel format
    PIXELFORMATDESCRIPTOR pfd;
    ZeroMemory(&pfd, sizeof(PIXELFORMATDESCRIPTOR));
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL;

    int pixelformat = ChoosePixelFormat(m_canvasDC, &pfd);

    // set the pixel format for the dc
    if (!SetPixelFormat(m_canvasDC, pixelformat, &pfd)) {
        SoDebugError::post("SbGLContext", "SetPixelFormat failed\n");
        return;
    }

    // create rendering context
    m_contextObj = wglCreateContext(m_canvasDC);
    if (!m_contextObj) {
        SoDebugError::post("SbGLContext", "wglCreateContext failed\n");
        return;
    }

    if (!wglMakeCurrent(m_canvasDC, m_contextObj)) {
        SoDebugError::post("SbGLContext", "wglMakeCurrent failed\n");
        return;
    }
#elif defined(SB_OS_MACX)
    m_contextObj = 0;
    // Create a 1x1 pbuffer and associated context to bootstrap things
    CGLPixelFormatAttribute attribs[] = {
        (CGLPixelFormatAttribute) kCGLPFAPBuffer,
        (CGLPixelFormatAttribute) 0
    };
    CGLPixelFormatObj pixelFormat;
    GLint numPixelFormats;
    if (CGLChoosePixelFormat(attribs, &pixelFormat, &numPixelFormats) != kCGLNoError) {
        SoDebugError::post("SbGLContext", "error choosing pixel format");
        return;
    }
    if (!pixelFormat) {
        SoDebugError::post("SbGLContext", "no pixel format selected");
        return;
    }
    CGLContextObj context;
    CGLError res = CGLCreateContext(pixelFormat, 0, &context);
    CGLDestroyPixelFormat(pixelFormat);
    if (res != kCGLNoError) {
        SoDebugError::post("SbGLContext", "error creating context\n");
        return;
    }
    if (CGLSetCurrentContext(context) != kCGLNoError) {
        CGLDestroyContext(context);
        //CGLDestroyPBuffer(pbuffer);
        SoDebugError::post("SbGLContext", "error making context current\n");
        return;
    }
    m_contextObj = context;
#elif defined(SB_OS_LINUX)
    m_contextObj = 0;
    m_pbuffer = 0;
    m_display = XOpenDisplay(0);
    if (!m_display) {
        SoDebugError::post("SbGLContext", "error opening X display\n");
        return;
    }

    int configAttrs[] = {
        GLX_DRAWABLE_TYPE,
        GLX_PBUFFER_BIT,
        GLX_RENDER_TYPE,
        GLX_RGBA_BIT,
        GLX_DOUBLEBUFFER,
        0,
        0
    };
    int nelements = 0;
    GLXFBConfig* config = glXChooseFBConfig(m_display, 0, configAttrs, &nelements);
    if (!config) {
        SoDebugError::post("SbGLContext", "glXChooseFBConfig failed\n");
        return;
    }
    if (!nelements) {
        SoDebugError::post("SbGLContext", "glXChooseFBConfig returned 0 elements\n");
        XFree(config);
        return;
    }
    GLXContext context = glXCreateNewContext(m_display, config[0], GLX_RGBA_TYPE, 0, True);
    if (!context) {
        SoDebugError::post("SbGLContext", "glXCreateNewContext failed\n");
        XFree(config);
        return;
    }
    int pbufferAttrs[] = {
        GLX_PBUFFER_WIDTH,
        1,
        GLX_PBUFFER_HEIGHT,
        1,
        0
    };
    GLXPbuffer pbuffer = glXCreatePbuffer(m_display, config[0], pbufferAttrs);
    XFree(config);
    if (!pbuffer) {
        SoDebugError::post("SbGLContext", "glxCreatePbuffer failed\n");
        return;
    }
    if (!glXMakeCurrent(m_display, pbuffer, context)) {
        SoDebugError::post("SbGLContext", "glXMakeCurrent failed\n");
        return;
    }
    m_contextObj = context;
    m_pbuffer = pbuffer;
#else
#  error "SbGLContext has not been ported to this OS"
#endif
}

SbGLContext::~SbGLContext()
{
#if defined(SB_OS_WIN)
    wglMakeCurrent(0, 0);
    wglDeleteContext(m_contextObj);
    ReleaseDC(m_canvasWindow, m_canvasDC);
    DestroyWindow(m_canvasWindow);
#elif defined(SB_OS_MACX)
    CGLSetCurrentContext(0);
    CGLDestroyContext(m_contextObj);
#elif defined(SB_OS_LINUX)
    glXMakeCurrent(m_display, 0, 0);
    glXDestroyContext(m_display, m_contextObj);
    glXDestroyPbuffer(m_display, m_pbuffer);
    XCloseDisplay(m_display);
#else
#  error "SbGLContext has not been ported to this OS"
#endif
}

bool SbGLContext::makeContextCurrent()
{
#if defined(SB_OS_WIN)
    if (wglGetCurrentContext() == m_contextObj)
        return true;

    return (wglMakeCurrent(m_canvasDC, m_contextObj)==TRUE);

#elif defined(SB_OS_MACX)
    if (CGLGetCurrentContext() == m_contextObj)
        return true;

    return (CGLSetCurrentContext(m_contextObj) == kCGLNoError);

#elif defined(SB_OS_LINUX)
    if (glXGetCurrentContext() == m_contextObj)
        return true;

    return glXMakeCurrent(m_display, m_pbuffer, m_contextObj);

#else
#  error "SbGLContext has not been ported to this OS"
#endif
}
