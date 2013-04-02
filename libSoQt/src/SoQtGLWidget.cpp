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
 |   $Revision: 1.6 $
 |
 |   Classes:
 |	SoQtGLWidget
 |
 |   Author(s): Alain Dumesny, David Mott
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

/*
 * Ported to Qt by Morgan Leborgne, 2010
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <Inventor/elements/SoGLCacheContextElement.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/Qt/SoQt.h>

#include <Inventor/Qt/SoQtGLWidget.h>

#include <QVBoxLayout>
#include <QtOpenGL>
#include <QStackedWidget>
#include <QMap>

/*
 * macros
 */

#define SET_BIT(mode, mask, flag) \
    if (flag) mode |= mask; \
    else mode &= ~mask;

class SoQGLWidgetProxy : public QGLWidget
{
public:
    // Create an SoQGLWidgetProxy and share the context when possible.
    static SoQGLWidgetProxy *create(const QGLFormat & format, SoQtGLWidget* parent)
    {
        QMap<SoQGLWidgetProxy*, int>::const_iterator it = contextMap.constBegin();
        while (it != contextMap.constEnd()) {
            SoQGLWidgetProxy *glx = new SoQGLWidgetProxy(format, parent, it.key());
            if (glx->isSharing()) {
                contextMap[glx] = it.value();
                return glx;
            }
            delete glx;
            ++it;
        }

        // If we got to here, we counldn't share. Create a new one.
        SoQGLWidgetProxy *glx = new SoQGLWidgetProxy(format, parent);

        contextMap[glx] = SoGLCacheContextElement::getUniqueCacheContext();

        return glx;
    }

    // Returns unique integers for contexts.  Used
    // to determine when display lists can/can't be shared (display lists
    // are shared between contexts).
    static int getDisplayListShareGroup(QGLContext *ctx)
    {
        QMap<SoQGLWidgetProxy*, int>::const_iterator it = contextMap.constBegin();
        while (it != contextMap.constEnd()) {
            if (it.key()->context() == ctx) {
                return it.value();
            }
        }
#ifdef DEBUG
        SoDebugError::post("SoQGLWidgetProxy::getDisplayListShareGroup",
                           "Context not found!");
#endif
        return -1;
    }

    virtual ~SoQGLWidgetProxy(){
        contextMap.remove(this);
    }

    virtual void initializeGL()
    {
        // enable zbuffer if this visual supports it....
        if (glWidget->glModes & SO_GLX_ZBUFFER)
            glEnable(GL_DEPTH_TEST);

        glWidget->initGraphic();
    }

    virtual void initializeOverlayGL()
    {
        glWidget->initOverlayGraphic();
    }

    virtual void paintGL()
    {
        // clear this flag now that we have an expose event
        glWidget->waitForExpose = FALSE;
        glWidget->redraw();
    }

    virtual void paintOverlayGL()
    {
        glWidget->redrawOverlay();
    }

    virtual void resizeGL (int width, int height)
    {
        glWidget->glxSize = SbVec2s(width, height);
        glWidget->sizeChanged(glWidget->glxSize);
    }

    virtual bool event( QEvent * e )
    {
        QGLWidget::event(e);

        if (!e->isAccepted())
            glWidget->processEvent(e);

        return e->isAccepted();
    }

    void contextMenuEvent(QContextMenuEvent *event)
    {
        glWidget->contextMenuEvent(event);
    }
protected:
    SoQGLWidgetProxy (const QGLFormat& format, SoQtGLWidget* parent, const QGLWidget* shareWidget = NULL)
        : QGLWidget (format, parent->mgrWidget, shareWidget)
    {
        glWidget = parent;
        setMouseTracking (true);
        setFocusPolicy(Qt::StrongFocus);
        setAutoBufferSwap(false);
    }

    virtual void setVisible(bool visible)
    {
        QGLWidget::setVisible(visible);

        glWidget->visibilityChanged(visible);
    }

private:
    SoQtGLWidget* glWidget;

    static QMap<SoQGLWidgetProxy*, int> contextMap;
};

QMap<SoQGLWidgetProxy*, int> SoQGLWidgetProxy::contextMap;

//
////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////
//
// Constructor
//
SoQtGLWidget::SoQtGLWidget(
    QWidget *parent,
    const char *name, 
    SbBool buildInsideParent, 
    int modes, 
    SbBool buildNow) : SoQtComponent(parent, name, buildInsideParent)
//
////////////////////////////////////////////////////////////////////////
{
    // window related variables
    mgrWidget = NULL;
    singleBufferWidget = doubleBufferWidget = NULL;
    attribList = QGLFormat::defaultFormat();
    glModes = modes;
    glxSize.setValue(0, 0);
    borderSize = 0;
    waitForExpose = TRUE;
    drawToFrontBuffer = TRUE; // used by RA to draw new scenes to front buffer
    enableDrawToFrontBuffer = TRUE;
    
    //
    // window attribute list
    //
    attribList.setDoubleBuffer(glModes & SO_GLX_DOUBLE);
    attribList.setStereo(glModes & SO_GLX_STEREO);
    attribList.setRgba(glModes & SO_GLX_RGB);
    attribList.setStencil(glModes & SO_GLX_STENCIL);
    attribList.setOverlay(glModes & SO_GLX_OVERLAY);
    attribList.setDepth(glModes & SO_GLX_ZBUFFER);
    
    // Build the widget tree, and let SoQtComponent know about our base widget.
    if (buildNow)
	setBaseWidget(buildWidget(getParentWidget()));
}

////////////////////////////////////////////////////////////////////////
//
// virtual destructor
//
SoQtGLWidget::~SoQtGLWidget()
//
////////////////////////////////////////////////////////////////////////
{

}

////////////////////////////////////////////////////////////////////////
//
// Description:
//  	sets the normal window visual
//
// Use: public
void
SoQtGLWidget::setNormalVisual(const QGLFormat & format)
//
////////////////////////////////////////////////////////////////////////
{
    //
    // delete the current set of windows and create a new widget
    // with the given visual.
    //
    destroyNormalWindows();
    buildNormalGLXWidget(format); // this MUST be called after we destroy the old contexes
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//  	sets the overlay window visual
//
// Use: public
void
SoQtGLWidget::setOverlayVisual(const QGLFormat& format)
//
////////////////////////////////////////////////////////////////////////
{
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//  	gets the normal window visual
//
// Use: public
QGLFormat
SoQtGLWidget::getNormalVisual()
//
////////////////////////////////////////////////////////////////////////
{
    if (! getNormalWidget())
	return QGLFormat();
    
    return getNormalWidget()->format();
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//  	gets the overlay window visual
//
// Use: public
QGLFormat
SoQtGLWidget::getOverlayVisual()
//
////////////////////////////////////////////////////////////////////////
{
    if (! getOverlayWidget())
	return QGLFormat();
    
    return getOverlayWidget()->format();
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//  	Dynamically changes between single and double buffering.
//
// Use: virtual public
void
SoQtGLWidget::setDoubleBuffer(SbBool flag)
//
////////////////////////////////////////////////////////////////////////
{
    if (flag == isDoubleBuffer())
	return;
 
   // set the gl mode and update the attribute list
    attribList.setDoubleBuffer(flag);
    destroyNormalWindows();
    buildNormalGLXWidget(attribList);
	
    // prevent a redraw a redraw from happening until we receive an
    // expose event - this will prevent too many redraws from happening
    // if the scene graph also changes at the same time.
    waitForExpose = TRUE;
}


////////////////////////////////////////////////////////////////////////
//
// Description:
//  	Dynamically changes stereo/mono buffering type.
//
// Use: protected
void
SoQtGLWidget::setStereoBuffer(SbBool flag)
//
////////////////////////////////////////////////////////////////////////
{
    if (flag == isStereoBuffer())
	return;
    
    // update the attribute list
	// now set the flag and create the new window with given visual
    attribList.setStereo(flag);
    destroyNormalWindows();
    buildNormalGLXWidget(attribList); // this MUST be called after we destroy the old contexes
}


////////////////////////////////////////////////////////////////////////
//
// Description:
//  	Returns different integers for contexts that cannot share GL
//  display lists.
//
// Use: protected
int
SoQtGLWidget::getDisplayListShareGroup(QGLContext *ctx)
//
////////////////////////////////////////////////////////////////////////
{
    return SoQGLWidgetProxy::getDisplayListShareGroup(ctx);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//  	destroys the given widget (free the context, the visual and resets
//  those pointers).
//
// Use: private
void
SoQtGLWidget::destroyGLXWidget(QWidget *w, const QGLContext *ctx, SbBool normalWindow)
//
////////////////////////////////////////////////////////////////////////
{   
    // delete the widget and reset the passed pointers
    //w->deleteLater();
    delete w;
    w = NULL;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//  	destroys the current set of windows (called when visual changes)
//  This doesn't affect the overly window thought.
//
// Use: private
void
SoQtGLWidget::destroyNormalWindows()
//
////////////////////////////////////////////////////////////////////////
{  
    if (doubleBufferWidget) {
        destroyGLXWidget(doubleBufferWidget, doubleBufferWidget->context(), TRUE);
        doubleBufferWidget = NULL;
    }
    
    if (singleBufferWidget) {
        destroyGLXWidget(singleBufferWidget, singleBufferWidget->context(), TRUE);
        singleBufferWidget = NULL;
    }
    
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//  	This builds a form around the GLX widget. Subclasses should
// register their own event interest.
//
// Use: protected
//
QWidget*
SoQtGLWidget::buildWidget(QWidget *parent)
//
////////////////////////////////////////////////////////////////////////
{
    //
    // build a top level form to hold the GLX widgets (single/double/overlay)
    //
    mgrWidget = new QFrame(parent);
    mgrWidget->setLayout(new QVBoxLayout(mgrWidget));
    mgrWidget->layout()->setMargin(0);
    mgrWidget->layout()->setSpacing(0);
	
    // pick an initial size if set
    SbVec2s size = (glxSize[0] && glxSize[1]) ? glxSize : getSize();
    if (size[0] && size[1]) {
        mgrWidget->resize(size[0], size[1]);
    }
   
    buildNormalGLXWidget(attribList);
    if (glModes & SO_GLX_OVERLAY) // make this the top window
	buildOverlayGLXWidget(attribList);
    
    return mgrWidget;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//  	This builds a GLX widget of the correct current type.
//
// Use: protected
void
SoQtGLWidget::buildNormalGLXWidget(const QGLFormat & format)
//
////////////////////////////////////////////////////////////////////////
{
    
    QGLWidget* glx = SoQGLWidgetProxy::create(format, this);

    mgrWidget->layout()->addWidget(glx);

    attribList = glx->format();

    // now set the flag and create the new window with given visual
    SET_BIT(glModes, SO_GLX_DOUBLE, attribList.doubleBuffer());
    SET_BIT(glModes, SO_GLX_STEREO, attribList.stereo());

    // assign the right widget var and manage widget
    if (isDoubleBuffer())
	doubleBufferWidget = glx;
    else
	singleBufferWidget = glx;
    
    
    // prevent a redraw a redraw from happening until we receive an
    // expose event - this will prevent too many redraws from happening
    // if the scene graph also changes at the same time.
    waitForExpose = TRUE;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//  	This builds the overly GLX widget (which is static)
//
// Use: protected
void
SoQtGLWidget::buildOverlayGLXWidget(const QGLFormat & format)
//
////////////////////////////////////////////////////////////////////////
{

}

////////////////////////////////////////////////////////////////////////
//
// Description:
//  	turns border on/off
//
// Use: public
void
SoQtGLWidget::setBorder(SbBool flag)
//
////////////////////////////////////////////////////////////////////////
{
    if (flag == isBorder())
	return;
    
    // set the border size and update the widgets
    borderSize = flag ? 2 : 0;
    
	if (mgrWidget) {
		mgrWidget->setLineWidth(borderSize);
		mgrWidget->setFrameStyle(flag ? QFrame::Box : QFrame::NoFrame);
	}
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//  	enables/disabled the drawToFrontBuffer functionality
//
// Use: public
void
SoQtGLWidget::setDrawToFrontBufferEnable(SbBool flag)
//
////////////////////////////////////////////////////////////////////////
{
    enableDrawToFrontBuffer = flag;
    
    // clear the flag is disabled
    if (! enableDrawToFrontBuffer)
	drawToFrontBuffer = FALSE;
}

SbBool
SoQtGLWidget::bindNormalContext()
{
    getNormalWidget()->makeCurrent();
    return TRUE;
}

SbBool 
SoQtGLWidget::unbindNormalContext()
{
    getNormalWidget()->doneCurrent();
    return TRUE;
}

SbBool
SoQtGLWidget::bindOverlayContext()
{
    getNormalWidget()->makeOverlayCurrent();
    return TRUE;
}

SbBool
SoQtGLWidget::unbindOverlayContext()
{
    getNormalWidget()->doneCurrent();
    return TRUE;
}

SbBool
SoQtGLWidget::swapNormalBuffers()
{
    getNormalWidget()->swapBuffers();
    return TRUE;
}

SbBool
SoQtGLWidget::swapOverlayBuffers()
{
    getNormalWidget()->swapBuffers();
    return TRUE;
}

void 
SoQtGLWidget::setCursor( const QCursor& newCursor )
{
    if (getNormalWidget())
        getNormalWidget()->setCursor(newCursor);
}

void 
SoQtGLWidget::unsetCursor()
{
    if (getNormalWidget())
        getNormalWidget()->unsetCursor();
}

QCursor
SoQtGLWidget::getCursor() const
{
    return ((SoQtGLWidget*)this)->getNormalWidget()->cursor();
}

void
SoQtGLWidget::scheduleRedraw()
{
    getNormalWidget()->update();
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//  	sets the size of the glx widgets
//
// Use: protected
void
SoQtGLWidget::setGlxSize(SbVec2s newSize)
//
////////////////////////////////////////////////////////////////////////
{
    if (newSize == glxSize)
	return;
    
    // now set the container widget size (children will resize automatically)
    if (mgrWidget)
		mgrWidget->resize(glxSize[0], glxSize[1]);
    // else we havn't built yet, so cache this initial build size
    else
	glxSize = newSize;
}

void
SoQtGLWidget::eventHandler(QWidget *w, SoQtGLWidget *p, QEvent *qe, bool *)
{
   p->processEvent(qe);
}

//
////////////////////////////////////////////////////////////////////////
// dummy virtual functions
////////////////////////////////////////////////////////////////////////
//

void SoQtGLWidget::redrawOverlay() {}
void SoQtGLWidget::processEvent(QEvent *) {}
void SoQtGLWidget::initGraphic() {}
void SoQtGLWidget::initOverlayGraphic() {}
void SoQtGLWidget::sizeChanged(const SbVec2s &) {}
void SoQtGLWidget::widgetChanged(QWidget*) {}
