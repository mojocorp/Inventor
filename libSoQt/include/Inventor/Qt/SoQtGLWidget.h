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
 * Copyright (C) 1990-93   Silicon Graphics, Inc.
 *
 _______________________________________________________________________
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 |
 |   $Revision: 1.1 $
 |
 |   Description:
 |	This is a c++ wrapper for the Glx mixed mode widget.
 |
 |   Author(s): Alain Dumesny, David Mott
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

/*
 * Ported to Qt by Morgan Leborgne, 2010
 */

#ifndef _SO_QT_GL_WIDGET_H_
#define _SO_QT_GL_WIDGET_H_

#include <Inventor/Qt/SoQt.h>
#include <Inventor/Qt/SoQtComponent.h>
#include <Inventor/SoType.h>
#include <Inventor/SbLinear.h>
#include <QCursor>
#include <QGLFormat>

/*
 * Defines used when specifying the glModes flag to the constructor.
 * (used instead of the glx.h defines which do overlap)
 */
#define SO_GLX_RGB	(1<<0)
#define SO_GLX_DOUBLE	(1<<1)
#define SO_GLX_ZBUFFER	(1<<2)
#define SO_GLX_OVERLAY	(1<<3)
#define SO_GLX_STEREO	(1<<4)
#define SO_GLX_STENCIL  (1<<5)

class QFrame;
class QGLFormat;
class QGLWidget;
class QGLContext;
class QGridLayout;
class QContextMenuEvent;

//////////////////////////////////////////////////////////////////////////////
//
//  Class: SoQtGLWidget
//
//
//////////////////////////////////////////////////////////////////////////////

class SOQT_EXPORT SoQtGLWidget : public SoQtComponent {
    Q_OBJECT

public:
    
    //
    // Get the current normal and overlay GLX windows, which are needed
    // as arguments to glXMakeCurrent() when doing drawing in the normal
    // or overlay planes.
    //
    // NOTE: These should NOT be cached by users because these will
    // change dynamically when the visual is changed (like when
    // setDoubleBuffer() is called).
    //
    //Window  	getNormalWindow();
    //Window   	getOverlayWindow();
    
    QGLContext*	getNormalContext() { return (QGLContext*)getNormalWidget()->context(); }
    QGLContext*	getOverlayContext() { return (QGLContext*)getOverlayWidget()->context(); }

    QGLWidget* getNormalWidget()     	    { return (glModes & SO_GLX_DOUBLE) ? doubleBufferWidget : singleBufferWidget; }
    QGLWidget* getOverlayWidget()    	    { return getNormalWidget(); }

    //
    // Specify exactly what the visual should be for the normal and overlay
    // window. This allows the user to create all possible visuals supported
    // by OpenGL. The XVisualInfo structure should be a valid OpenGL visual
    // returned by glXChooseVisual().
    //
    virtual void setNormalVisual (const QGLFormat& format);
    QGLFormat getNormalVisual();
    virtual void    setOverlayVisual(const QGLFormat& format);
    QGLFormat getOverlayVisual();
    
    //
    // Routine which allows to dynamically change between single and
    // double buffering.
    //
    virtual void    setDoubleBuffer(SbBool onOrOff);
    SbBool  	    isDoubleBuffer()  	    { return (glModes & SO_GLX_DOUBLE); }
    
    //
    // shows/hides a border of thickness 3 around the glx widgets. This
    // if OFF by default, but turned on by the render area.
    //
    void	setBorder(SbBool onOrOff);
    int     getBorderSize() const { return borderSize; }
    SbBool	isBorder() const	    { return borderSize != 0; }
    
    //
    // Inventor 2.1 changed the behavior of double buffered windows to
    // redraw temporary to the front buffer when an expose event is received
    // or when a new sceneGraph() is set on an SoQtRenderArea. This does not
    // apply for general interactive rendering. The idea is that it is
    // better to see something redraw (especially if it is slow) than
    // to see garbage in the window (in the expose case) or an old invalid
    // scene graph (in the SoQtRenderArea::setSceneGraph() case).
    //
    // This API enables you to turn this functionality OFF if for some reason
    // you are unable to prevent repeated expose from causing a redraw (which
    // is bad and should be fixed, as it gets worse for large scenes).
    // This will be the case for example if you can't create a pulldown
    // menu in the popup planes.
    //
    // NOTE: this api might be removed in some future releases and is only
    // intended as a porting help for Inventor 2.0 to 2.1
    //
    void	setDrawToFrontBufferEnable(SbBool enableFlag);
    SbBool	isDrawToFrontBufferEnable() const   { return enableDrawToFrontBuffer; }

    virtual SbBool bindNormalContext();
    virtual SbBool unbindNormalContext();

    virtual SbBool makeOverlayCurrent() { return bindOverlayContext(); }
    virtual SbBool bindOverlayContext();
    virtual SbBool unbindOverlayContext();

    virtual SbBool swapNormalBuffers();
    virtual SbBool swapOverlayBuffers();

    virtual void setCursor( const QCursor& newCursor );
    virtual void unsetCursor();
    QCursor getCursor() const;

    virtual void scheduleRedraw();

protected:
    
    // Subclasses can pass in a bitwise OR specifying the GL modes
    // (e.g. SO_GLX_RGB | SO_GLX_DOUBLE | SO_GLX_ZBUFFER | SO_GLX_OVERLAY)
    // If buildNow is FALSE, this will not build its widget tree until
    // buildWidget() is explicity called; else, buildWidget() is called here.
    SoQtGLWidget(
            QWidget *parent = NULL,
            const char *name = NULL,
            SbBool buildInsideParent = TRUE,
            int glModes = SO_GLX_RGB,
            SbBool buildNow = TRUE);
    virtual ~SoQtGLWidget();

    // subclasses MUST redefine redraw() to draw in the normal bit planes.
    // redrawOverlay() should be defined if the overlay planes are being
    // used, and processEvent() should be defined if X events are being
    // received (see eventMask).
    virtual void	redraw() = 0;
    virtual void redrawOverlay();
    virtual void processEvent (QEvent *anyEvent);
    virtual void visibilityChanged (bool){}
    virtual void contextMenuEvent(QContextMenuEvent *){}

    // subclasses can redefine these to do something useful.
    // initGraphic() is called whenever a GLX window gets created
    // sizeChanged() is called whenever the window changes size
    // widgetChanged() is called whenever the widget is changed (i.e. at
    //    initialization or after switching from single->double buffer)
    virtual void initGraphic();
    virtual void	initOverlayGraphic();
    virtual void	sizeChanged(const SbVec2s &newSize);
    virtual void widgetChanged (QWidget* newWidget);
    
    // sets/gets the size of the qgl widget(s) - Note this size could be
    // different from the SoQtComponent::getSize() method which return
    // the size of the component, not necessary the same as the glx widget
    // window (because of extra stuff like the decoration in the viewers).
    void		setGlxSize(SbVec2s newSize);
    const SbVec2s &	getGlxSize() const			{ return glxSize; }
    
    // subclasses can use this routine as the event handler for any
    // devices they wish to get input from.
    static void eventHandler( QWidget* , SoQtGLWidget* , QEvent* , bool* );
    
    // set/get stereo buffering visual. This routine (like setDoubleBuffer)
    // can be called interactively, althought slower since a new window
    // with the needed visual is always created on the fly.
    void		setStereoBuffer(SbBool flag);
    SbBool		isStereoBuffer()  { return (glModes & SO_GLX_STEREO); }
    
    // returns TRUE if main window is in rgb mode (FALSE if color index)
    SbBool		isRGBMode()	    { return (glModes & SO_GLX_RGB); }
    
    // returns the display lists share group for given context:
    int			getDisplayListShareGroup(QGLContext* ctx);

    SbBool		waitForExpose;	// prevent redraws until an expose is received
    SbBool		drawToFrontBuffer; // used by RA to draw new scenes to front buffer
    
    // make those methods protected to enable the SoQtRenderArea to use them
    // directly when it needs to build the widget and get its resources.
    QWidget* buildWidget(QWidget* parent);
    QWidget* getGlxMgrWidget()		{ return (QWidget*)mgrWidget; }
private:
    // local vars
    QFrame		*mgrWidget;
    QGLWidget           *doubleBufferWidget, *singleBufferWidget;
    SbVec2s		glxSize; // size of glx widgets which is always up to date
    QGLFormat           attribList;
    int			glModes;
    int			borderSize;
    SbBool		enableDrawToFrontBuffer;
    
    
    // creates a GLX widget of the correct current type and get the current
    // set of windows, color maps, etc...
    void                buildNormalGLXWidget(const QGLFormat& format);
    void		buildOverlayGLXWidget(const QGLFormat& format);
    void		destroyNormalWindows();
    void		destroyGLXWidget(QWidget *w, const QGLContext *ctx, SbBool normalWindow);
    friend class SoQGLWidgetProxy;
};


#endif // _SO_QT_GL_WIDGET_H_

