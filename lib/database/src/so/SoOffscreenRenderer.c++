/*
 *
 *  Copyright (C) 2000 Silicon Graphics, Inc.  All Rights Reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  Further, this software is distributed without any warranty that it is
 *  free of the rightful claim of any third person regarding infringement
 *  or the like.  Any license provided herein, whether implied or
 *  otherwise, applies only to this software file.  Patent licenses, if
 *  any, provided herein do not apply to combinations of this program with
 *  other software, or any other product whatsoever.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *  Contact information: Silicon Graphics, Inc., 1600 Amphitheatre Pkwy,
 *  Mountain View, CA  94043, or:
 *
 *  http://www.sgi.com
 *
 *  For further information regarding this notice, see:
 *
 *  http://oss.sgi.com/projects/GenInfo/NoticeExplan/
 *
 */

/*
 * Copyright (C) 1990,91,92,93   Silicon Graphics, Inc.
 *
 _______________________________________________________________________
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 |
 |   $Revision: 1.3 $
 |
 |   Classes:
 |	SoOffscreenRenderer
 |
 |   Author(s)		: Dave Immel
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */
#include <machine.h>
#include <glad/gl.h>
#include <Inventor/SoOffscreenRenderer.h>
#include <Inventor/SoPath.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/elements/SoGLCacheContextElement.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/nodes/SoNode.h>
#include <image/image-sgi.h>
#include <image/image-eps.h>

#if defined(SB_OS_WIN)
#include <windows.h>
#elif defined(SB_OS_MACX)
#include <OpenGL/OpenGL.h>
#include <CoreGraphics/CGDisplayConfiguration.h>
#elif defined(SB_OS_LINUX)
#include <glad/glx.h>
#else
#error "SoOffscreenRenderer has not been ported to this OS"
#endif

class SbGLContext {
  public:
    SbGLContext() {
#if defined(SB_OS_WIN)
        m_canvasWindow = 0;
        m_canvasDC = 0;
        m_contextObj = 0;

        WNDCLASS wc;
        if (!GetClassInfo(GetModuleHandle(0), "CANVASGL", &wc)) {
            ZeroMemory(&wc, sizeof(WNDCLASS));
            wc.style = CS_OWNDC;
            wc.hInstance = GetModuleHandle(0);
            wc.lpfnWndProc = DefWindowProc;
            wc.lpszClassName = "CANVASGL";

            if (!RegisterClass(&wc)) {
                SoDebugError::post("SbGLContext", "RegisterClass failed\n");
                return;
            }
        }

        m_canvasWindow = CreateWindow(
            "CANVASGL", "CANVASGL", WS_CAPTION, CW_USEDEFAULT, CW_USEDEFAULT,
            CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, GetModuleHandle(0), 0);
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
            (CGLPixelFormatAttribute)kCGLPFAPBuffer,
            (CGLPixelFormatAttribute)0};
        CGLPixelFormatObj pixelFormat;
        GLint             numPixelFormats;
        if (CGLChoosePixelFormat(attribs, &pixelFormat, &numPixelFormats) !=
            kCGLNoError) {
            SoDebugError::post("SbGLContext", "error choosing pixel format");
            return;
        }
        if (!pixelFormat) {
            SoDebugError::post("SbGLContext", "no pixel format selected");
            return;
        }
        CGLContextObj context;
        CGLError      res = CGLCreateContext(pixelFormat, 0, &context);
        CGLDestroyPixelFormat(pixelFormat);
        if (res != kCGLNoError) {
            SoDebugError::post("SbGLContext", "error creating context\n");
            return;
        }
        if (CGLSetCurrentContext(context) != kCGLNoError) {
            CGLDestroyContext(context);
            // CGLDestroyPBuffer(pbuffer);
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

        int          configAttrs[] = {GLX_DRAWABLE_TYPE,
                             GLX_PBUFFER_BIT,
                             GLX_RENDER_TYPE,
                             GLX_RGBA_BIT,
                             GLX_DOUBLEBUFFER,
                             0,
                             0};
        int          nelements = 0;
        GLXFBConfig *config =
            glXChooseFBConfig(m_display, 0, configAttrs, &nelements);
        if (!config) {
            SoDebugError::post("SbGLContext", "glXChooseFBConfig failed\n");
            return;
        }
        if (!nelements) {
            SoDebugError::post("SbGLContext",
                               "glXChooseFBConfig returned 0 elements\n");
            XFree(config);
            return;
        }
        GLXContext context =
            glXCreateNewContext(m_display, config[0], GLX_RGBA_TYPE, 0, True);
        if (!context) {
            SoDebugError::post("SbGLContext", "glXCreateNewContext failed\n");
            XFree(config);
            return;
        }
        int pbufferAttrs[] = {GLX_PBUFFER_WIDTH, 1, GLX_PBUFFER_HEIGHT, 1, 0};
        GLXPbuffer pbuffer =
            glXCreatePbuffer(m_display, config[0], pbufferAttrs);
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
#error "SbGLContext has not been ported to this OS"
#endif
    }

    ~SbGLContext() {
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
#error "SbGLContext has not been ported to this OS"
#endif
    }

    bool makeContextCurrent() {
#if defined(SB_OS_WIN)
        if (wglGetCurrentContext() == m_contextObj)
            return true;

        return (wglMakeCurrent(m_canvasDC, m_contextObj) == TRUE);

#elif defined(SB_OS_MACX)
        if (CGLGetCurrentContext() == m_contextObj)
            return true;

        return (CGLSetCurrentContext(m_contextObj) == kCGLNoError);

#elif defined(SB_OS_LINUX)
        if (glXGetCurrentContext() == m_contextObj)
            return true;

        return glXMakeCurrent(m_display, m_pbuffer, m_contextObj);

#else
#error "SbGLContext has not been ported to this OS"
#endif
    }

  private:
#if defined(SB_OS_WIN)
    HWND  m_canvasWindow;
    HDC   m_canvasDC;
    HGLRC m_contextObj;
#elif defined(SB_OS_MACX)
    CGLContextObj m_contextObj;
#elif defined(SB_OS_LINUX)
    Display *m_display;
    GLXContext m_contextObj;
    GLXPbuffer m_pbuffer;
#else
#error "SbGLContext has not been ported to this OS"
#endif
};

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Constructor.
//
// Use: public

SoOffscreenRenderer::SoOffscreenRenderer(const SbViewportRegion &viewportRegion)
    : cacheContext(SoGLCacheContextElement::getUniqueCacheContext())

//
////////////////////////////////////////////////////////////////////////
{
    offAction = new SoGLRenderAction(viewportRegion);
    userAction = NULL;
    comps = SoOffscreenRenderer::RGB;
    backgroundColor.setValue(0.0, 0.0, 0.0);
    ctx = new SbGLContext;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Constructor.
//
// Use: public

SoOffscreenRenderer::SoOffscreenRenderer(SoGLRenderAction *act)
    : cacheContext(SoGLCacheContextElement::getUniqueCacheContext())

//
////////////////////////////////////////////////////////////////////////
{
    offAction = new SoGLRenderAction(act->getViewportRegion());
    userAction = act;
    comps = SoOffscreenRenderer::RGB;
    backgroundColor.setValue(0.0, 0.0, 0.0);
    ctx = new SbGLContext;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Destructor.
//
// Use: public

SoOffscreenRenderer::~SoOffscreenRenderer()

//
////////////////////////////////////////////////////////////////////////
{
    delete offAction;
    delete ctx;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Calculate and return the number of pixels per inch on the current
//    device horizontally.
//
// Use: static, public

float
SoOffscreenRenderer::getScreenPixelsPerInch()

//
////////////////////////////////////////////////////////////////////////
{
#if defined(SB_OS_WIN)

    HDC hdc = GetDC(NULL);
    if (hdc == NULL)
        return 75.0;

    // Get the dimensions of the screen
    float pix = (float)GetDeviceCaps(hdc, LOGPIXELSX);

    ReleaseDC(NULL, hdc);

    return pix;

#elif defined(SB_OS_MACX)

    CGDirectDisplayID display = CGMainDisplayID();

    // Get the dimensions of the screen (25.4mm = 1 inch)
    return CGDisplayPixelsWide(display) * 25.4f /
           (float)CGDisplayScreenSize(display).width;
#elif defined(SB_OS_LINUX)

    // Create an X Display
    Display *tmpDisplay = XOpenDisplay(NULL);

    if (tmpDisplay == NULL)
        return 75.0;

    // Get the dimensions of the screen (25.4mm = 1 inch)
    float pix = DisplayWidth(tmpDisplay, 0) * 25.4f /
                (float)DisplayWidthMM(tmpDisplay, 0);

    XCloseDisplay(tmpDisplay);

    return pix;
#else
#error "SoOffscreenRenderer has not been ported to this OS"
#endif
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Return the maximum allowable resolution of a viewport in which
//    to render a scene.
//
// Use: static, public

SbVec2s
SoOffscreenRenderer::getMaximumResolution()

//
////////////////////////////////////////////////////////////////////////
{
    SbGLContext context;
    context.makeContextCurrent();

    GLint params[2];
    glGetIntegerv(GL_MAX_VIEWPORT_DIMS, params);

    return SbVec2s((short)params[0], (short)params[1]);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Set the viewport region.
//
// Use: public

void
SoOffscreenRenderer::setViewportRegion(const SbViewportRegion &viewportRegion)

//
////////////////////////////////////////////////////////////////////////
{
    offAction->setViewportRegion(viewportRegion);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Get the viewport region.
//
// Use: public

const SbViewportRegion &
SoOffscreenRenderer::getViewportRegion() const

//
////////////////////////////////////////////////////////////////////////
{
    return offAction->getViewportRegion();
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Set the render action.
//
// Use: public

void
SoOffscreenRenderer::setGLRenderAction(SoGLRenderAction *act)

//
////////////////////////////////////////////////////////////////////////
{
    userAction = act;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Get the render action.
//
// Use: public

SoGLRenderAction *
SoOffscreenRenderer::getGLRenderAction() const

//
////////////////////////////////////////////////////////////////////////
{
    return userAction;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Render the given scene graph into a buffer.
//
// Use: public

SbBool
SoOffscreenRenderer::render(SoNode *scene)
//
////////////////////////////////////////////////////////////////////////
{
    return renderGeneric(scene);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Render the given scene graph into a buffer.
//
// Use: public

SbBool
SoOffscreenRenderer::render(SoPath *scene)

//
////////////////////////////////////////////////////////////////////////
{
    return renderGeneric(scene);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Render
//
// Use: private

bool
SoOffscreenRenderer::renderGeneric(SoBase *base)

//
////////////////////////////////////////////////////////////////////////
{
    if (!ctx->makeContextCurrent()) {
#ifdef DEBUG
        SoDebugError::post("SoOffscreenRenderer::renderGeneric",
                           "Could not setup context");
#endif
        return false;
    }

    // Delete the pixel buffer if it has been previously used.
    pixelBuffer = SbImage();

    // Set the render action to use.
    SoGLRenderAction *act = userAction ? userAction : offAction;

    renderedViewport = act->getViewportRegion();

    // Get the offscreen pixmap ready for rendering
    const SbVec2s &vpSize = renderedViewport.getViewportSizePixels();

    GLenum format = GL_INVALID_VALUE;
    size_t numBytes = 0;
    switch (comps) {
    case SoOffscreenRenderer::LUMINANCE:
        format = GL_LUMINANCE;
        numBytes = vpSize[0] * vpSize[1];
        break;
    case SoOffscreenRenderer::LUMINANCE_TRANSPARENCY:
        format = GL_LUMINANCE_ALPHA;
        numBytes = vpSize[0] * vpSize[1] * 2;
        break;
    case SoOffscreenRenderer::RGB:
        format = GL_RGB;
        numBytes = vpSize[0] * vpSize[1] * 3;
        break;
    case SoOffscreenRenderer::RGB_TRANSPARENCY:
        format = GL_RGBA;
        numBytes = vpSize[0] * vpSize[1] * 4;
        break;
    }

    // Generate the framebuffer object
    GLuint framebuffer;
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    // Generate the render buffers
    GLuint colorbuffer;
    glGenRenderbuffers(1, &colorbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, colorbuffer);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                              GL_RENDERBUFFER, colorbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, format, vpSize[0], vpSize[1]);

    GLuint depthbuffer;
    glGenRenderbuffers(1, &depthbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthbuffer);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                              GL_RENDERBUFFER, depthbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32, vpSize[0],
                          vpSize[1]);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
#ifdef DEBUG
        SoDebugError::post("SoOffscreenRenderer::renderGeneric",
                           "Framebuffer status is incomplete");
#endif
        return false;
    }
    // Clear the pixmap to the backgroundColor
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glClearColor(backgroundColor[0], backgroundColor[1], backgroundColor[2], 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Render the scene into the new graphics context

    // Set the GL cache context for the action to a unique number
    // (we'll use and increment SoContextIncrement), so that it doesn't try
    // to use display lists from other contexts.
    const uint32_t oldContext = act->getCacheContext();
    act->setCacheContext(cacheContext);
    if (base->isOfType(SoNode::getClassTypeId()))
        act->apply((SoNode *)base);
    else if (base->isOfType(SoPath::getClassTypeId()))
        act->apply((SoPath *)base);
    act->setCacheContext(oldContext);

    pixelBuffer = SbImage(vpSize, SbImage::Format(comps), numBytes, NULL);

    glPushAttrib(GL_PIXEL_MODE_BIT);
    glReadBuffer(GL_COLOR_ATTACHMENT0);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glReadPixels(0, 0, vpSize[0], vpSize[1], format, GL_UNSIGNED_BYTE,
                 (GLvoid *)pixelBuffer.getBytes());
    glPopAttrib();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glDeleteRenderbuffers(1, &colorbuffer);
    glDeleteRenderbuffers(1, &depthbuffer);
    glDeleteFramebuffers(1, &framebuffer);

    return !pixelBuffer.isNull();
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Return the rendered buffer.
//
// Use: public

const unsigned char *
SoOffscreenRenderer::getBuffer() const

//
////////////////////////////////////////////////////////////////////////
{
    return pixelBuffer.getConstBytes();
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Write the buffer into a .rgb file.
//
// Use: public

SbBool
SoOffscreenRenderer::writeToRGB(FILE *fp) const

//
////////////////////////////////////////////////////////////////////////
{
    // Open an image file for writing
    if (comps == (SoOffscreenRenderer::LUMINANCE_TRANSPARENCY) ||
        (comps == SoOffscreenRenderer::RGB_TRANSPARENCY)) {
#ifdef DEBUG
        SoDebugError::post("SoOffscreenRenderer::writeToRGB",
                           "The RGB file format does not support transparency");
#endif
        return FALSE;
    }

    // Get the correct image size.
    const int width = pixelBuffer.getSize()[0];
    const int height = pixelBuffer.getSize()[1];
    const int components = pixelBuffer.getNumComponents();

    sgi_t *image;

    if ((image = sgiOpenFile(fp, SGI_WRITE, SGI_COMP_RLE, 1, width, height,
                             components)) == NULL) {
#ifdef DEBUG
        SoDebugError::post("SoOffscreenRenderer::writeToRGB",
                           "could not open image file");
#endif
        return FALSE;
    }

    // For each row in the pixel buffer, write the row into the image file
    std::vector<unsigned short> rowBuf(width);
    unsigned char const *       pBuf = pixelBuffer.getConstBytes();

    for (int row = 0; row < height; row++) {
        // The pixel in the pixel buffer store pixel information arranged
        // by pixel, whereas the .rgb file stores pixel information arranged
        // by color component.  So scanlines of component data must be
        // accumulated before a row can be written.

        // Convert each color component
        for (int comp = 0; comp < components; comp++) {
            unsigned short *trow = rowBuf.data();

            // Convert a row
            const unsigned char *tbuf = pBuf + comp;
            for (int j = 0; j < width; j++, tbuf += components)
                *trow++ = (short)*tbuf;
            sgiPutRow(image, rowBuf.data(), row, comp);
        }
        pBuf += components * width;
    }
    sgiClose(image);
    return TRUE;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Write the buffer into Encapsulated PostScript.
//
// Use: public

SbBool
SoOffscreenRenderer::writeToPostScript(FILE *fp) const

//
////////////////////////////////////////////////////////////////////////
{
    const SbVec3s &vpSize = pixelBuffer.getSize();
    const float    ppi = renderedViewport.getPixelsPerInch();
    const SbVec2f  printSize(vpSize[0] / ppi, vpSize[1] / ppi);

    return writeToPostScript(fp, printSize);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Write the buffer into Encapsulated PostScript at the given
//    print size in inches.
//
// Use: public

SbBool
SoOffscreenRenderer::writeToPostScript(FILE *fp, const SbVec2f &printSize) const

//
////////////////////////////////////////////////////////////////////////
{
    if ((comps == LUMINANCE_TRANSPARENCY) || (comps == RGB_TRANSPARENCY)) {
#ifdef DEBUG
        SoDebugError::post("SoOffscreenRenderer::writeToRGB",
                           "PostScript does not support image transparency");
#endif
        return FALSE;
    }

    const int width = pixelBuffer.getSize()[0];
    const int height = pixelBuffer.getSize()[1];
    const int components = pixelBuffer.getNumComponents();

    return (writeEps(fp, width, height, components, pixelBuffer.getConstBytes(),
                     printSize[0], printSize[1]) == 0);
}
