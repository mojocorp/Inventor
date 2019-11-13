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
#include <GL/glx.h>
#include <Inventor/SoOffscreenRenderer.h>
#include <Inventor/SoPath.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/elements/SoGLCacheContextElement.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/nodes/SoNode.h>
#include <image/image-sgi.h>
#include <image/image-eps.h>

struct SbGLXContext {
    SbGLXContext() : display(NULL), visual(NULL), context(NULL), pixmap(0) {
        // Create an X Display
        display = XOpenDisplay(NULL);

        if (!display)
            return;

        // Create a GLX Visual
        // Offscreen renderings will always be rendered as RGB images
        // Currently the OpenGL does not support storing alpha channel
        // information in the offscreen pixmap.
        static int attributeList[] = {
            GLX_RGBA, GLX_RED_SIZE,  1, GLX_GREEN_SIZE,
            1,        GLX_BLUE_SIZE, 1, GLX_DEPTH_SIZE,
            1,        None};
        visual =
            glXChooseVisual(display, DefaultScreen(display), attributeList);
        if (!visual) {
            XCloseDisplay(display);
            display = NULL;
            return;
        }

        // Create a GLX Context
        context = glXCreateContext(display, visual, NULL, TRUE);
        if (!context) {
            XCloseDisplay(display);
            display = NULL;
            return;
        }
    }
    ~SbGLXContext() {
        if (display) {
            glXMakeCurrent(display, 0, 0);
            glXDestroyContext(display, context);
            glXDestroyGLXPixmap(display, pixmap);
            XFreePixmap(display, pmap);
            XCloseDisplay(display);
        }
    }

    bool resize(const SbVec2s &sz) {
        if (!display) {
            return false;
        }

        if (size != sz) {
            // The pixmap is not the right size.  Destroy it.
            if (pixmap) {
                glXMakeCurrent(display, 0, 0);
                glXDestroyGLXPixmap(display, pixmap);
                XFreePixmap(display, pmap);
            }

            // Create X and GLX Pixmap
            pmap = XCreatePixmap(display, DefaultRootWindow(display),
                                 (unsigned int)sz[0], (unsigned int)sz[1],
                                 visual->depth);
            pixmap = glXCreateGLXPixmap(display, visual, pmap);
            size = sz;
        }

        // Set the graphics context to point to the offscreen pixmap
        if (glXMakeCurrent(display, pixmap, context) == 0)
            return false;

        // Store the pixels as byte aligned rows in the Pixmap
        glPixelStorei(GL_PACK_ALIGNMENT, 1);

        return true;
    }

    SbImage getImage(SoOffscreenRenderer::Components comps) {
        GLenum format;
        int    allocSize;
        switch (comps) {
        case SoOffscreenRenderer::LUMINANCE:
            format = GL_LUMINANCE;
            allocSize = size[0] * size[1] * 1;
            break;
        case SoOffscreenRenderer::LUMINANCE_TRANSPARENCY:
            format = GL_LUMINANCE_ALPHA;
            allocSize = size[0] * size[1] * 2;
            break;
        case SoOffscreenRenderer::RGB:
            format = GL_RGB;
            allocSize = size[0] * size[1] * 3;
            break;
        case SoOffscreenRenderer::RGB_TRANSPARENCY:
            format = GL_RGBA;
            allocSize = size[0] * size[1] * 4;
            break;
        }

        SbImage buffer = SbImage(size, SbImage::Format(comps), allocSize, NULL);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glReadPixels(0, 0, size[0], size[1], format, GL_UNSIGNED_BYTE,
                     (GLvoid *)buffer.getBytes());
        return buffer;
    }

    SbVec2s      size;
    Display *    display;
    XVisualInfo *visual;
    GLXContext   context;
    GLXPixmap    pixmap;
    Pixmap       pmap;
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
    ctx = new SbGLXContext;
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
    ctx = new SbGLXContext;
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
    Display *tmpDisplay = XOpenDisplay(NULL);

    // Create an X Display
    if (tmpDisplay == NULL)
        return 75.0;

    // Get the dimensions of the screen
    const float pix = DisplayWidth(tmpDisplay, 0) * 25.4 /
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
    SbGLXContext context;

    if (!context.resize(SbVec2s(2, 2))) {
#ifdef DEBUG
        SoDebugError::post("SoOffscreenRenderer::getMaximumResolution",
                           "Could not initialize Pixmap");
#endif
        return SbVec2s(-1, -1);
    }

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
    // Delete the pixel buffer if it has been previously used.
    pixelBuffer = SbImage();

    // Set the render action to use.
    SoGLRenderAction *act = userAction ? userAction : offAction;

    renderedViewport = act->getViewportRegion();

    // Get the offscreen pixmap ready for rendering
    const SbVec2s &vpSize = renderedViewport.getViewportSizePixels();

    if (!ctx->resize(vpSize)) {
#ifdef DEBUG
        SoDebugError::post("SoOffscreenRenderer::renderGeneric",
                           "Could not setup Pixmap");
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

    pixelBuffer = ctx->getImage(comps);

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
