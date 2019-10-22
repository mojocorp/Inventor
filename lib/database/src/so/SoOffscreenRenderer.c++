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
#include <stdio.h>
#include <assert.h>
#include <Inventor/SoOffscreenRenderer.h>
#include <Inventor/SoPath.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/elements/SoGLCacheContextElement.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/nodes/SoNode.h>
#include <image/image-sgi.h>
#include <image/image-eps.h>

// Offscreen renderings will always be rendered as RGB images
// Currently the OpenGL does not support storing alpha channel information
// in the offscreen pixmap.
static int attributeList[] = { GLX_RGBA, GLX_RED_SIZE, 1, GLX_GREEN_SIZE, 1,
                               GLX_BLUE_SIZE, 1, GLX_DEPTH_SIZE, 1, None };


////////////////////////////////////////////////////////////////////////
//
// Description:
//    Constructor.
//
// Use: public

SoOffscreenRenderer::SoOffscreenRenderer(
    const SbViewportRegion &viewportRegion ) :
    cacheContext(SoGLCacheContextElement::getUniqueCacheContext())

//
////////////////////////////////////////////////////////////////////////
{
    offAction   = new SoGLRenderAction(viewportRegion);
    userAction  = NULL;
    display     = NULL;
    comps       = SoOffscreenRenderer::RGB;
    backgroundColor.setValue(0.0, 0.0, 0.0);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Constructor.
//
// Use: public

SoOffscreenRenderer::SoOffscreenRenderer(
    SoGLRenderAction *act ) :
    cacheContext(SoGLCacheContextElement::getUniqueCacheContext())

//
////////////////////////////////////////////////////////////////////////
{
    offAction   = new SoGLRenderAction(act->getViewportRegion());
    userAction  = act;
    display     = NULL;
    comps       = SoOffscreenRenderer::RGB;
    backgroundColor.setValue(0.0, 0.0, 0.0);
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

    // Delete the pixmap, window, and context, as it is no longer needed
    if (display != NULL) {
        glXDestroyGLXPixmap( display, pixmap );
        glXDestroyContext( display, context );
        XCloseDisplay( display );
    }
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
    Display *tmpDisplay;
    float   pix;

    // Create an X Display
    if ((tmpDisplay=XOpenDisplay(NULL)) == NULL)
        return 75.0;

    // Get the dimensions of the screen
    pix = DisplayWidth(tmpDisplay, 0) * 25.4 /
          (float)DisplayWidthMM(tmpDisplay, 0);
    XCloseDisplay(tmpDisplay);

    return pix;
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
    Display             *dpy = NULL;
    XVisualInfo         *vi;
    GLXContext          cx;
    GLXPixmap           glxpmap;
    Pixmap              xpmap;
    SbVec2s             tmpvp(2, 2);
    GLint params[2];

    if (!initPixmap(dpy, vi, cx, tmpvp, glxpmap, xpmap)) {
#ifdef DEBUG
        SoDebugError::post("SoOffscreenRenderer::getMaximumResolution",
                           "Could not initialize Pixmap");
#endif
        return SbVec2s(-1, -1);
    }

    glGetIntegerv(GL_MAX_VIEWPORT_DIMS, params);
    glXDestroyGLXPixmap( dpy, glxpmap );
    glXDestroyContext( dpy, cx );
    XCloseDisplay(dpy);

    return (SbVec2s((short)params[0], (short)params[1]));
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Set the viewport region.
//
// Use: public

void
SoOffscreenRenderer::setViewportRegion(
    const SbViewportRegion &viewportRegion )


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
SoOffscreenRenderer::setGLRenderAction(
    SoGLRenderAction *act )


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
    // Delete the pixel buffer if it has been previously used.
    pixelBuffer = SbImage();

    // Set the render action to use.
    SoGLRenderAction *act;
    if (userAction != NULL)
        act = userAction;
    else
        act = offAction;
    renderedViewport = act->getViewportRegion();

    // Get the offscreen pixmap ready for rendering
    if (!setupPixmap()) {
#ifdef DEBUG
        SoDebugError::post("SoOffscreenRenderer::render",
                           "Could not setup Pixmap");
#endif
        return FALSE;
    }

    // Set the GL cache context for the action to a unique number
    // (we'll use and increment SoContextIncrement), so that it doesn't try
    // to use display lists from other contexts.
    const uint32_t oldContext = act->getCacheContext();
    act->setCacheContext(cacheContext);
    act->apply(scene);
    act->setCacheContext(oldContext);

    readPixels();

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
    // Delete the pixel buffer if it has been previously used.
    pixelBuffer = SbImage();

    // Set the render action to use.
    SoGLRenderAction *act;
    if (userAction != NULL)
        act = userAction;
    else
        act = offAction;
    renderedViewport = act->getViewportRegion();

    // Get the offscreen pixmap ready for rendering
    if (!setupPixmap()) {
#ifdef DEBUG
        SoDebugError::post("SoOffscreenRenderer::writeToRGB",
                           "Could not setup Pixmap");
#endif
        return FALSE;
    }

    // Render the scene into the new graphics context

    // Set the GL cache context for the action to a unique number
    // (we'll use and increment SoContextIncrement), so that it doesn't try
    // to use display lists from other contexts.
    const uint32_t oldContext = act->getCacheContext();
    act->setCacheContext(cacheContext);
    act->apply(scene);
    act->setCacheContext(oldContext);

    readPixels();

    return TRUE;
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
SoOffscreenRenderer::writeToRGB( FILE *fp ) const


//
////////////////////////////////////////////////////////////////////////
{
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

    // Get the correct viewport size.
    const SbVec2s &vpSize = renderedViewport.getViewportSizePixels();

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

    // Get the format of the pixmap data
    GLenum format = getFormat();

    // For each row in the pixel buffer, write the row into the image file
    unsigned short *rowBuf = new unsigned short[vpSize[0]];
    unsigned char *pBuf = new unsigned char[vpSize[0]*components*2];

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    for (int row=0; row<vpSize[1]; row++) {
        
        // Read the next scanline of pixels from the offscreen pixmap
        glReadPixels(0, row, vpSize[0], 1, format,
                 GL_UNSIGNED_BYTE, (GLvoid *)pBuf);

        // The pixel in the pixel buffer store pixel information arranged
        // by pixel, whereas the .rgb file stores pixel information arranged
        // by color component.  So scanlines of component data must be
        // accumulated before a row can be written.

        // Convert each color component
        for (int comp=0; comp<components; comp++) {
            unsigned short *trow = rowBuf;

            // Convert a row
            const unsigned char *tbuf = pBuf + comp;
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
    const SbVec2s &vpSize = renderedViewport.getViewportSizePixels();
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

    return (writeEps(fp, pixelBuffer.getSize()[0], pixelBuffer.getSize()[1], components, pixelBuffer.getConstBytes(), printSize[0], printSize[1]) == 0);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Setup a pixmap into which the rendering will occur.
//
// Use: private

SbBool
SoOffscreenRenderer::setupPixmap()

//
////////////////////////////////////////////////////////////////////////
{
    const SbVec2s &vpSize = renderedViewport.getViewportSizePixels();
 
    if (!initPixmap(display, visual, context, vpSize, pixmap, pmap))
        return FALSE;

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
//    Setup a pixmap into which the rendering will occur.
//
// Use: static, private

SbBool
SoOffscreenRenderer::initPixmap(
    Display *     &dpy,
    XVisualInfo * &vi,
    GLXContext    &cx,
    const SbVec2s &sz,
    GLXPixmap     &glxpmap,
    Pixmap        &xpmap )

//
////////////////////////////////////////////////////////////////////////
{
    // If a display is already open, and is the same size, use it.
    // Otherwise, close it and open up a fresh one.
    if (dpy != NULL) {

        // Get the size of the passed pixmap.  If it is the same size
        // as the passed size, just return.
        Window root;
        int x, y;
        unsigned int width, height, border_width, depth;
        if (!XGetGeometry(dpy, xpmap, &root, &x, &y, &width, &height,
                &border_width, &depth)) {

            // Bad pixmap.  Destroy it.
            glXDestroyGLXPixmap( dpy, glxpmap );
            glXDestroyContext( dpy, cx );
            XCloseDisplay(dpy);
        }
        else if ((width == sz[0]) && (height == sz[1])) {

            // This pixmap is the right size to use for this rendering.
            // Make it the current context for rendering.
            if (!glXMakeCurrent(dpy, glxpmap, cx)) {
                glXDestroyGLXPixmap( dpy, glxpmap );
                glXDestroyContext( dpy, cx );
                XCloseDisplay(dpy);
                dpy = NULL;
                return FALSE;
            }

            // Store the pixels as byte aligned rows in the Pixmap.
            glPixelStorei(GL_PACK_ALIGNMENT, 1);

            return TRUE;
        }
        else {
   
            // The pixmap is not the right size.  Destroy it.
            glXDestroyGLXPixmap( dpy, glxpmap );
            glXDestroyContext( dpy, cx );
            XCloseDisplay(dpy);
        }
    }

    // Create an X Display
    if ((dpy = XOpenDisplay(NULL)) == NULL)
        return FALSE;

    // Create a GLX Visual
    if ((vi = glXChooseVisual(dpy, DefaultScreen(dpy),
            attributeList)) == NULL) {
        XCloseDisplay(dpy);
        dpy = NULL;
        return FALSE;
    }

    // Create a GLX Context
    if ((cx = glXCreateContext(dpy, vi, NULL, FALSE)) == NULL) {
        XCloseDisplay(dpy);
        dpy = NULL;
        return FALSE;
    }

    // Create X and GLX Pixmap
    xpmap = XCreatePixmap( dpy, DefaultRootWindow(dpy),
            (unsigned int)sz[0], (unsigned int)sz[1], vi->depth );
    glxpmap = glXCreateGLXPixmap( dpy, vi, xpmap );

    if (!glXMakeCurrent(dpy, glxpmap, cx)) {
        glXDestroyGLXPixmap( dpy, glxpmap );
        glXDestroyContext( dpy, cx );
        XCloseDisplay(dpy);
        dpy = NULL;
        return FALSE;
    }

    // Store the pixels as byte aligned rows in the Pixmap
    glPixelStorei(GL_PACK_ALIGNMENT, 1);

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
    // Set the graphics context to be the offscreen pixmap
    if (!setContext())
        return;

    const SbVec2s &vpSize = renderedViewport.getViewportSizePixels();

    int allocSize;
    switch (comps) {
        case LUMINANCE: 
            allocSize = vpSize[0] * vpSize[1] * 1;
            break;
        case LUMINANCE_TRANSPARENCY:
            allocSize = vpSize[0] * vpSize[1] * 2;
            break;
        case RGB:
            allocSize = vpSize[0] * vpSize[1] * 3;
            break;
        case RGB_TRANSPARENCY:
            allocSize = vpSize[0] * vpSize[1] * 4;
            break;
    }

    pixelBuffer = SbImage(vpSize, SbImage::Format(comps), allocSize, NULL);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glReadPixels(0, 0, vpSize[0], vpSize[1], getFormat(),
                 GL_UNSIGNED_BYTE, (GLvoid *)pixelBuffer.getBytes());
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Set the graphics context to point to the offscreen pixmap
//
// Use: private

SbBool
SoOffscreenRenderer::setContext() const

//
////////////////////////////////////////////////////////////////////////
{
    if (!glXMakeCurrent(display, pixmap, context)) {
        glXDestroyGLXPixmap( display, pixmap );
        glXDestroyContext( display, context );
        XCloseDisplay(display);
        return FALSE;
    }

    return TRUE;
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
        case LUMINANCE_TRANSPARENCY:
            return GL_LUMINANCE_ALPHA;
        case RGB:
            return GL_RGB;
        case RGB_TRANSPARENCY:
            return GL_RGBA;
    }
    return GL_INVALID_ENUM;
}

