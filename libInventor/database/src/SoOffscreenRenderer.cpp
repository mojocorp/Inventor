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

#include <Inventor/misc/SoGL.h>

#include <stdio.h>
#include <assert.h>
#include <Inventor/SbLinear.h>
#include <Inventor/SoOffscreenRenderer.h>
#include <Inventor/SoPath.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/nodes/SoNode.h>
#include <Inventor/elements/SoGLCacheContextElement.h>

#include <image-sgi.h>
#include <image-eps.h>

#if defined(SB_OS_WIN)
#   include <windows.h>
#elif defined(SB_OS_MACX)
#   include <ApplicationServices/ApplicationServices.h>
#   include <OpenGL/OpenGL.h>
#elif defined(SB_OS_LINUX)
#   include <X11/Xlib.h>
#   include <GL/glx.h>
#else
#   error "SoOffscreenRenderer has not been ported to this OS"
#endif

class SoOffscreenRendererInternal
{
public:
    SoOffscreenRendererInternal()
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
                SoDebugError::post("SoOffscreenRenderer", "RegisterClass failed\n");
                return;
            }
        }

        m_canvasWindow = CreateWindow(L"CANVASGL", L"CANVASGL",
            WS_CAPTION,
            CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
            CW_USEDEFAULT, 0, 0, GetModuleHandle(0), 0);
        if (!m_canvasWindow) {
            SoDebugError::post("SoOffscreenRenderer", "CreateWindow failed\n");
            return;
        }

        // get the device context
        m_canvasDC = GetDC(m_canvasWindow);
        if (!m_canvasDC) {
            SoDebugError::post("SoOffscreenRenderer", "GetDC failed\n");
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
            SoDebugError::post("SoOffscreenRenderer", "SetPixelFormat failed\n");
            return;
        }

        // create rendering context
        m_contextObj = wglCreateContext(m_canvasDC);
        if (!m_contextObj) {
            SoDebugError::post("SoOffscreenRenderer", "wglCreateContext failed\n");
            return;
        }

        if (!wglMakeCurrent(m_canvasDC, m_contextObj)) {
            SoDebugError::post("SoOffscreenRenderer", "wglMakeCurrent failed\n");
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
            SoDebugError::post("SoOffscreenRenderer", "error choosing pixel format");
            return;
        }
        if (!pixelFormat) {
            SoDebugError::post("SoOffscreenRenderer", "no pixel format selected");
            return;
        }
        CGLContextObj context;
        CGLError res = CGLCreateContext(pixelFormat, 0, &context);
        CGLDestroyPixelFormat(pixelFormat);
        if (res != kCGLNoError) {
            SoDebugError::post("SoOffscreenRenderer", "error creating context\n");
            return;
        }
        if (CGLSetCurrentContext(context) != kCGLNoError) {
            CGLDestroyContext(context);
            //CGLDestroyPBuffer(pbuffer);
            SoDebugError::post("SoOffscreenRenderer", "error making context current\n");
            return;
        }
        m_contextObj = context;
#elif defined(SB_OS_LINUX)
        m_contextObj = 0;
        m_pbuffer = 0;
	m_display = XOpenDisplay(0);
	if (!m_display) {
            SoDebugError::post("SoOffscreenRenderer", "error opening X display\n");
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
            SoDebugError::post("SoOffscreenRenderer", "glXChooseFBConfig failed\n");
            return;
        }
        if (!nelements) {
            SoDebugError::post("SoOffscreenRenderer", "glXChooseFBConfig returned 0 elements\n");
            XFree(config);
            return;
        }
        GLXContext context = glXCreateNewContext(m_display, config[0], GLX_RGBA_TYPE, 0, True);
        if (!context) {
            SoDebugError::post("SoOffscreenRenderer", "glXCreateNewContext failed\n");
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
            SoDebugError::post("SoOffscreenRenderer", "glxCreatePbuffer failed\n");
            return;
        }
        if (!glXMakeCurrent(m_display, pbuffer, context)) {
            SoDebugError::post("SoOffscreenRenderer", "glXMakeCurrent failed\n");
            return;
        }
        m_contextObj = context;
        m_pbuffer = pbuffer;
#else
#  error "SoOffscreenRenderer has not been ported to this OS"
#endif
    }

    ~SoOffscreenRendererInternal()
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
#  error "SoOffscreenRenderer has not been ported to this OS"
#endif
    }

    bool makeContextCurrent()
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
#  error "SoOffscreenRenderer has not been ported to this OS"
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
    Display*   m_display;
    GLXContext m_contextObj;
    GLXPbuffer m_pbuffer;
#else
#  error "SoOffscreenRenderer has not been ported to this OS"
#endif
};

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Constructor.
//
// Use: public

SoOffscreenRenderer::SoOffscreenRenderer( const SbViewportRegion &viewportRegion )
    : pixelBuffer(NULL),
      comps(SoOffscreenRenderer::RGB),
      backgroundColor(SbColor(0.0, 0.0, 0.0)),
      userAction(NULL),
      offAction(new SoGLRenderAction(viewportRegion)),
      cacheContext(SoGLCacheContextElement::getUniqueCacheContext()),
      framebuffer(0),
      renderbuffer(0),
      depthbuffer(0),
      internal(new SoOffscreenRendererInternal)
//
////////////////////////////////////////////////////////////////////////
{
    internal->makeContextCurrent();

    SoGLContext::setCurrentContext(cacheContext);

    // Generate the framebuffer object
    glGenFramebuffersEXT(1, &framebuffer);

    // Generate the render buffer
    glGenRenderbuffersEXT(1, &renderbuffer);
    glGenRenderbuffersEXT(1, &depthbuffer);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Constructor.
//
// Use: public

SoOffscreenRenderer::SoOffscreenRenderer( SoGLRenderAction *act )
    : pixelBuffer(NULL),
      comps(SoOffscreenRenderer::RGB),
      backgroundColor(SbColor(0.0, 0.0, 0.0)),
      userAction(act),
      offAction(new SoGLRenderAction(act->getViewportRegion())),
      cacheContext(SoGLCacheContextElement::getUniqueCacheContext()),
      framebuffer(0),
      renderbuffer(0),
      depthbuffer(0),
      internal(new SoOffscreenRendererInternal)
//
////////////////////////////////////////////////////////////////////////
{
    internal->makeContextCurrent();

    SoGLContext::setCurrentContext(cacheContext);

    // Generate the framebuffer object
    glGenFramebuffersEXT(1, &framebuffer);

    // Generate the render buffer
    glGenRenderbuffersEXT(1, &renderbuffer);
    glGenRenderbuffersEXT(1, &depthbuffer);
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


    if( pixelBuffer != NULL )
        delete pixelBuffer;

    internal->makeContextCurrent();

    glDeleteRenderbuffersEXT(1, &renderbuffer);
    glDeleteRenderbuffersEXT(1, &depthbuffer);
    glDeleteFramebuffersEXT(1, &framebuffer);

    // Delete the pixmap, window, and context, as it is no longer needed
    delete internal;
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
{
#if defined(SB_OS_WIN)

    HDC hdc = GetDC(NULL);
    if (hdc == NULL)
        return 75.0;

    // Get the dimensions of the screen
    float pix =  (float)GetDeviceCaps(hdc, LOGPIXELSX);

    ReleaseDC(NULL,hdc);

    return pix;

#elif defined(SB_OS_MACX)
    
    CGDirectDisplayID display = CGMainDisplayID();
    CGSize size = CGDisplayScreenSize(display);

    // Get the dimensions of the screen (25.4mm = 1 inch)
    return CGDisplayPixelsWide(display) * 25.4f / (float)CGDisplayScreenSize(display).width;
#elif defined(SB_OS_LINUX)
    
    // Create an X Display
    Display *tmpDisplay = XOpenDisplay(NULL);
    
    if (tmpDisplay == NULL)
        return 75.0;

    // Get the dimensions of the screen (25.4mm = 1 inch)
    float pix = DisplayWidth(tmpDisplay, 0) * 25.4f / (float)DisplayWidthMM(tmpDisplay, 0);
    
    XCloseDisplay(tmpDisplay);

    return pix;
#else
#  error "SoOffscreenRenderer has not been ported to this OS"
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
    SoOffscreenRendererInternal p;

    p.makeContextCurrent();

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
SoOffscreenRenderer::setViewportRegion( const SbViewportRegion &viewportRegion )


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

const SbViewportRegion  &
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
SoOffscreenRenderer::setGLRenderAction( SoGLRenderAction *act )

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
    // Set the render action to use.
    SoGLRenderAction *act;
    if (userAction != NULL)
        act = userAction;
    else
        act = offAction;

    renderedViewport = act->getViewportRegion();

    if (!resize(renderedViewport.getViewportSizePixels()))
        return FALSE;

    // Set the GL cache context for the action to a unique number,
    // so that it doesn't try to use display lists from other contexts.
    uint32_t oldContext = act->getCacheContext();
    act->setCacheContext(cacheContext);
    act->apply(scene);
    act->setCacheContext(oldContext);

    return TRUE;
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
    // Set the render action to use.
    SoGLRenderAction *act;
    if (userAction != NULL)
        act = userAction;
    else
        act = offAction;

    renderedViewport = act->getViewportRegion();

    if (!resize(renderedViewport.getViewportSizePixels()))
        return FALSE;

    // Set the GL cache context for the action to a unique number, 
    // so that it doesn't try to use display lists from other contexts.
    uint32_t oldContext = act->getCacheContext();
    act->setCacheContext(SoGLCacheContextElement::getUniqueCacheContext());
    act->apply(scene);
    act->setCacheContext(oldContext);

    return TRUE;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Return the rendered buffer.
//
// Use: public

unsigned char *
SoOffscreenRenderer::getBuffer() const


//
////////////////////////////////////////////////////////////////////////
{
    // If the buffer has not been gotten yet, read the pixels into
    // the buffer.  Return the buffer to the user.
    if (pixelBuffer == NULL) {    
        ((SoOffscreenRenderer *)this)->readPixels();
    }
    return pixelBuffer;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Write the buffer into a .rgb file.
//
// Use: public

SbBool
SoOffscreenRenderer::writeToRGB( FILE *fp ) const

//
////////////////////////////////////////////////////////////////////////
{
    if (!getBuffer())
        return FALSE;

    // Open an image file for writing
    int dimensions, components;
    if (comps == (SoOffscreenRenderer::LUMINANCE_TRANSPARENCY) ||
       (comps == SoOffscreenRenderer::RGB_TRANSPARENCY)) {
#ifdef DEBUG
        SoDebugError::post("SoOffscreenRenderer::writeToRGB",
                           "The RGB file format does not support transparency");
#endif
        return FALSE;
    }
    else if (comps == SoOffscreenRenderer::LUMINANCE) {
        dimensions = 2;
        components = 1;
    }
    else {
        dimensions = 3;
        components = 3;
    }

    sgi_t *image;

    if ((image = sgiOpenFile( fp, SGI_WRITE, SGI_COMP_RLE, dimensions,
            (unsigned int)vpSize[0], (unsigned int)vpSize[1],
            components )) == NULL)
    {
#ifdef DEBUG
        SoDebugError::post("SoOffscreenRenderer::writeToRGB",
                           "could not open image file");
#endif
        return FALSE;
    }

    // For each row in the pixel buffer, write the row into the image file
    unsigned short *rowBuf = new unsigned short[vpSize[0]];
    unsigned char *pBuf = new unsigned char[vpSize[0]*components*2];

    for (int row=0; row<vpSize[1]; row++) {
        
        // The pixel in the pixel buffer store pixel information arranged
        // by pixel, whereas the .rgb file stores pixel information arranged
        // by color component.  So scanlines of component data must be
        // accumulated before a row can be written.
        unsigned char *tbuf = pixelBuffer + row * vpSize[0]*components*2;

        // Convert each color component
        for (int comp=0; comp<components; comp++) {
            unsigned short *trow = rowBuf;

            // Convert a row
            tbuf = pBuf + comp;
            for (int j=0; j<vpSize[0]; j++, tbuf += components)
                *trow++ = (short)*tbuf;
            sgiPutRow( image, rowBuf, row, comp );
        }
    }
    sgiClose( image );
    delete [] pBuf;
    delete [] rowBuf;
    return TRUE;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Write the buffer into Encapsulated PostScript.
//
// Use: public

SbBool
SoOffscreenRenderer::writeToPostScript( FILE *fp ) const

//
////////////////////////////////////////////////////////////////////////
{
    SbVec2f printSize;
    float   ppi = renderedViewport.getPixelsPerInch();

    printSize[0] = vpSize[0] / ppi;
    printSize[1] = vpSize[1] / ppi;

    return (writeToPostScript( fp, printSize ));
}


////////////////////////////////////////////////////////////////////////
//
// Description:
//    Write the buffer into Encapsulated PostScript at the given
//    print size in inches.
//
// Use: public

SbBool
SoOffscreenRenderer::writeToPostScript(
        FILE *fp,
        const SbVec2f &printSize ) const

//
////////////////////////////////////////////////////////////////////////
{
    if (!getBuffer())
        return FALSE;

    int components;
    if ((comps == LUMINANCE_TRANSPARENCY) ||
        (comps == RGB_TRANSPARENCY)) {
#ifdef DEBUG
        SoDebugError::post("SoOffscreenRenderer::writeToRGB",
                           "PostScript does not support image transparency");
#endif
        return FALSE;
    }
    else if (comps == SoOffscreenRenderer::LUMINANCE) {
        components = 1;
    }
    else {
        components = 3;
    }

    return (writeEps(fp, vpSize[0], vpSize[1], components, getBuffer(), printSize[0], printSize[1]) == 0);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Setup the framebuffer
//
// Use: private

bool
SoOffscreenRenderer::resize(const SbVec2s & size)

//
////////////////////////////////////////////////////////////////////////
{
    if (size == vpSize)
        return true;

    vpSize = size;

    // Delete the pixel buffer if it has been previously used.
    if (pixelBuffer != NULL) {
        delete pixelBuffer;
        pixelBuffer = NULL;
    }

    if (!internal->makeContextCurrent())
        return false;

    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, framebuffer);

    glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, renderbuffer);
    glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, getFormat(), vpSize[0], vpSize[1]);
    glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_RENDERBUFFER_EXT, renderbuffer);

    glBindRenderbufferEXT(GL_RENDERBUFFER, depthbuffer);
    glRenderbufferStorageEXT(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32, vpSize[0], vpSize[1]);
    glFramebufferRenderbufferEXT(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthbuffer);

    GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
    if (status != GL_FRAMEBUFFER_COMPLETE_EXT) {
#ifdef DEBUG
        SoDebugError::post("SoOffscreenRenderer::render",
            "framebuffer was incomplete");
#endif
        return FALSE;
    }

    // Clear the pixmap to the backgroundColor
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glClearColor(backgroundColor[0], backgroundColor[1], backgroundColor[2], 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    return TRUE;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Read the pixels from the Pixmap
//
// Use: private

void
SoOffscreenRenderer::readPixels()

//
////////////////////////////////////////////////////////////////////////
{
    if (!internal->makeContextCurrent())
        return;

    int bpp;
    switch (comps) 
    {
    case LUMINANCE:
        bpp = 1;
        break;
    case LUMINANCE_TRANSPARENCY:
        bpp = 2;
        break;
    case RGB:
        bpp = 3;
        break;
    case RGB_TRANSPARENCY:
        bpp = 4;
        break;
    default:
        break;
    }

    pixelBuffer = new unsigned char[vpSize[0] * vpSize[1] * bpp];
    
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glReadPixels(0, 0, vpSize[0], vpSize[1], getFormat(), GL_UNSIGNED_BYTE, (GLvoid *)pixelBuffer);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Get format information
//
// Use: private

GLenum
SoOffscreenRenderer::getFormat() const

//
////////////////////////////////////////////////////////////////////////
{
    switch (comps) {
        case LUMINANCE:
            return GL_LUMINANCE;
            break;
        case LUMINANCE_TRANSPARENCY:
            return GL_LUMINANCE_ALPHA;
            break;
        case RGB:
            return GL_RGB;
            break;
        case RGB_TRANSPARENCY:
            return GL_RGBA;
            break;
    }
    return GL_RGB;
}

