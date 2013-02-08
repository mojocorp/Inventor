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
#include "SbGLFramebufferObject.h"
#include "SbGLContext.h"

#include <image-sgi.h>
#include <image-eps.h>

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Constructor.
//
// Use: public

SoOffscreenRenderer::SoOffscreenRenderer( const SbViewportRegion &viewportRegion )
    : comps(SoOffscreenRenderer::RGB),
      backgroundColor(SbColor(0.0, 0.0, 0.0)),
      userAction(NULL),
      offAction(new SoGLRenderAction(viewportRegion)),
      cacheContext(SoGLCacheContextElement::getUniqueCacheContext()),
      ctx(new SbGLContext())
//
////////////////////////////////////////////////////////////////////////
{
    SoGLContext::setCurrentContext(cacheContext);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Constructor.
//
// Use: public

SoOffscreenRenderer::SoOffscreenRenderer( SoGLRenderAction *act )
    : comps(SoOffscreenRenderer::RGB),
      backgroundColor(SbColor(0.0, 0.0, 0.0)),
      userAction(act),
      offAction(new SoGLRenderAction(act->getViewportRegion())),
      cacheContext(SoGLCacheContextElement::getUniqueCacheContext()),
      ctx(new SbGLContext())
//
////////////////////////////////////////////////////////////////////////
{
    SoGLContext::setCurrentContext(cacheContext);
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

    // Delete the context, as it is no longer needed
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
    SbGLContext p;

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
//    Return the rendered buffer.
//
// Use: public

const SbImage &
SoOffscreenRenderer::getImage() const

//
////////////////////////////////////////////////////////////////////////
{
    return framebuffer;
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
    return framebuffer.getConstBytes();
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
    if (framebuffer.isNull())
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
            (unsigned int)framebuffer.getSize()[0], (unsigned int)framebuffer.getSize()[1],
            components )) == NULL)
    {
#ifdef DEBUG
        SoDebugError::post("SoOffscreenRenderer::writeToRGB",
                           "could not open image file");
#endif
        return FALSE;
    }

    // For each row in the pixel buffer, write the row into the image file
    unsigned short *rowBuf = new unsigned short[framebuffer.getSize()[0]];
    unsigned char *pBuf = new unsigned char[framebuffer.getSize()[0]*components*2];

    for (int row=0; row<framebuffer.getSize()[1]; row++) {
        
        // The pixel in the pixel buffer store pixel information arranged
        // by pixel, whereas the .rgb file stores pixel information arranged
        // by color component.  So scanlines of component data must be
        // accumulated before a row can be written.
        const unsigned char *tbuf = framebuffer.getConstBytes() + row * framebuffer.getSize()[0]*components*2;

        // Convert each color component
        for (int comp=0; comp<components; comp++) {
            unsigned short *trow = rowBuf;

            // Convert a row
            tbuf = pBuf + comp;
            for (int j=0; j<framebuffer.getSize()[0]; j++, tbuf += components)
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
    // Set the render action to use.
    SoGLRenderAction *act = userAction ? userAction : offAction;

    SbViewportRegion renderedViewport = act->getViewportRegion();

    float ppi = renderedViewport.getPixelsPerInch();

    SbVec2f printSize(framebuffer.getSize()[0] / ppi,
                      framebuffer.getSize()[1] / ppi);

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
    if (framebuffer.isNull())
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

    return (writeEps(fp, framebuffer.getSize()[0], framebuffer.getSize()[1], components, framebuffer.getConstBytes(), printSize[0], printSize[1]) == 0);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Render
//
// Use: private

bool
SoOffscreenRenderer::renderGeneric( SoBase *base )

//
////////////////////////////////////////////////////////////////////////
{
    if (!ctx->makeContextCurrent())
        return false;

    // Set the render action to use.
    SoGLRenderAction *act = userAction ? userAction : offAction;

    const SbViewportRegion & renderedViewport = act->getViewportRegion();

    SbGLFramebufferObject fbo(renderedViewport.getViewportSizePixels());
    fbo.bind();

    if (!fbo.isValid()) {
#ifdef DEBUG
        SoDebugError::post("SoOffscreenRenderer::render", "framebuffer was incomplete");
#endif
        return false;
    }

    // Clear the pixmap to the backgroundColor
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glClearColor(backgroundColor[0], backgroundColor[1], backgroundColor[2], 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Set the GL cache context for the action to a unique number,
    // so that it doesn't try to use display lists from other contexts.
    uint32_t oldContext = act->getCacheContext();
    act->setCacheContext(cacheContext);

    if (base->isOfType(SoNode::getClassTypeId()))
        act->apply((SoNode *)base);
    else if (base->isOfType(SoPath::getClassTypeId()))
        act->apply((SoPath *)base);

    act->setCacheContext(oldContext);

    framebuffer = fbo.toImage((SbImage::Format)comps);

    fbo.release();

    return true;
}
