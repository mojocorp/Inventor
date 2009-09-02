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

//  -*- C++ -*-

/*
 * Copyright (C) 1990,91,92,93   Silicon Graphics, Inc.
 *
 _______________________________________________________________________
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 |
 |   $Revision: 1.1 $
 |
 |   Description:
 |	This file contains the definition of the SoOffscreenRenderer class.
 |
 |   Classes:
 |	SoOffscreenRenderer
 |
 |   Author(s)		: Dave Immel
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

#ifndef  _SO_OFFSCREEN_RENDERER_
#define  _SO_OFFSCREEN_RENDERER_

#include <stdio.h>
#include <Inventor/misc/SoGL.h>
#include <Inventor/SbColor.h>
#include <Inventor/SbViewportRegion.h>
#include <Inventor/SbVec2s.h>
#ifdef SB_HAS_X11
#   include <X11/Xlib.h>
#endif
class SoNode;
class SoPath;
class SoGLRenderAction;

/// Renders to an off-screen buffer for printing or generating textures.
/// \ingroup General
/// This class is used to render into an off-screen buffer to create a
/// printable image or to generate a texture image. It uses X Pixmaps for
/// rendering. Methods are provided to write the buffer to a file, either
/// as an RGB image or an encapsulated PostScript description.
class SoOffscreenRenderer {
 public:

    /// Constructor. An internal instance of an SoGLRenderAction will be maintained with a
    /// viewport region set to the on passed in by the constructor.
    SoOffscreenRenderer( const SbViewportRegion &viewportRegion );

    /// Constructor. If a render action is passed to the contructor,
    /// that action will be used in all subsequent offscreen renderings.
    SoOffscreenRenderer( SoGLRenderAction *ra );

    /// Destructor
    ~SoOffscreenRenderer();

    enum Components {
        LUMINANCE = 1,
        LUMINANCE_TRANSPARENCY = 2,
        RGB = 3,                      // The default
        RGB_TRANSPARENCY = 4
    };

    /// Returns the number of pixels per inch (in the horizontal direction) of
    /// the current X device screen.
    static float	getScreenPixelsPerInch();

    /// Sets the components to be rendered.
    void		setComponents( Components components )
				{comps = components;}

    /// Returns the components to be rendered.
    Components		getComponents() const
				{return comps;}

    /// Sets the viewport region used for rendering.  This will NOT
    /// be applied to the viewport region of any render action passed in.
    void		setViewportRegion( const SbViewportRegion &region );

    /// Returns the viewport region used for rendering.  This will NOT
    /// be applied to the viewport region of any render action passed in.
    const SbViewportRegion  &getViewportRegion() const;

    /// Get the maximum supported resolution of the viewport.
    static SbVec2s	getMaximumResolution();

    /// Sets the background color for rendering.
    void		setBackgroundColor( const SbColor &c )
				{backgroundColor = c;}

    /// Returns the background color for rendering.
    const SbColor & getBackgroundColor() const
				{return backgroundColor;}

    /// Set the render action to use for rendering.  This will return
    /// any render action passed in by the caller.
    void                      setGLRenderAction(SoGLRenderAction *ra);

    /// Get the render action to use for rendering.  This will return
    /// any render action passed in by the caller.
    SoGLRenderAction *        getGLRenderAction() const;

    /// Renders the given scene, specified as a node or a path, into an off-screen buffer.
    SbBool		render( SoNode *scene );

    /// Renders the given scene, specified as a node or a path, into an off-screen buffer.
    SbBool		render( SoPath *scene );

    /// Returns the buffer containing the rendered image.  The buffer is an
    /// array of unsigned characters.  Each pixel is stored sequentially by
    /// scanline, starting with the lower left corner.  The data stored for
    /// each pixel is determined by the Components set before rendering.
    /// Pixels are stored in RGBA order and are packed without any padding
    /// between pixels or scanlines.  The buffer is allocated by the offscreen
    /// renderer class and the space is deleted when the instance is destructed.
    unsigned char *     getBuffer() const;

    /// Writes the buffer as a .rgb file to the given file pointer.
    SbBool		writeToRGB( FILE *fp ) const;

    /// Writes the buffer as encapsulated PostScript. If a print size is not
    /// given, the size of the image in the buffer is adjusted so it is the
    /// same as the apparent size of the viewport region on the current device.
    SbBool		writeToPostScript( FILE *fp ) const;
    SbBool		writeToPostScript( FILE *fp, const SbVec2f &printSize ) const;

  private:
    unsigned char *	pixelBuffer;
    Components          comps;
    SbColor		backgroundColor;
    SoGLRenderAction	*userAction, *offAction;
    SbViewportRegion    renderedViewport;
#ifdef SB_HAS_X11
    // These are used for rendering to the offscreen pixmap
    Display 		*display;
    XVisualInfo  	*visual;
    GLXContext 		context;
    GLXPixmap 		pixmap;
    Pixmap 		pmap;
#else
    typedef int Window;
    typedef int Display;
    typedef int XVisualInfo;
    typedef int GLXContext;
    typedef int GLXPixmap;
    typedef int Pixmap;

    Display 		*display;
    XVisualInfo  	*visual;
    GLXContext 		context;
    GLXPixmap 		pixmap;
    Pixmap 		pmap;
#endif
    // Setup the offscreen pixmap
    SbBool		setupPixmap();

    // Initialize an offscreen pixmap
    static SbBool 	initPixmap( Display * &dpy, XVisualInfo * &vi,
                                GLXContext &cx, const SbVec2s &sz,
                                GLXPixmap &glxPmap, Pixmap &xpmap );

    // Read pixels back from the Pixmap
    void		readPixels();

    // Set the graphics context
    SbBool		setContext() const;

    // Return the format used in the rendering
    void		getFormat( GLenum &format ) const;

    static void		putHex( FILE *fp, char val, int &hexPos );
};

#endif /* _SO_OFFSCREEN_RENDERER_ */

