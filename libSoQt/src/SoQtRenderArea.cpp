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
 |   $Revision: 1.5 $
 |
 |   Classes:
 |	SoQtRenderArea
 |
 |   Author(s): David Mott, Alain Dumesny
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

/*
 * Ported to Qt by Morgan Leborgne, 2010
 */

#include <Inventor/Qt/SoQt.h>
#include <Inventor/Qt/SoQtRenderArea.h>
#include <Inventor/Qt/devices/SoQtDevice.h>
#include <QEvent>
#include <QMouseEvent>
#include <QDate>
#include <QTime>

#include <Inventor/SoDB.h>
#include <Inventor/SbLinear.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/nodes/SoNode.h>
#include <Inventor/nodes/SoSelection.h>
#include <Inventor/nodes/SoLocateHighlight.h>
#include <Inventor/actions/SoHandleEventAction.h>
#include <Inventor/Qt/SoQtRenderArea.h>
#include <Inventor/Qt/devices/SoQtKeyboard.h>
#include <Inventor/Qt/devices/SoQtMouse.h>
#include <stdlib.h>
#include <stdio.h>

#if defined(__linux__)
#include <QX11Info>
#endif

static const char *thisClassName = "SoQtRenderArea";


////////////////////////////////////////////////////////////////////////
//
// Public constructor - build the widget right now
//
SoQtRenderArea::SoQtRenderArea (
    QWidget *parent,
    const char *name, 
    SbBool buildInsideParent, 
    SbBool getMouseInput,
    SbBool getKeyboardInput)
	: SoQtGLWidget(
	    parent,
	    name, 
	    buildInsideParent, 
            SO_GLX_RGB | SO_GLX_DOUBLE | SO_GLX_ZBUFFER | SO_GLX_OVERLAY | SO_GLX_STENCIL,
	    FALSE) // tell GLWidget not to build just yet 
//
////////////////////////////////////////////////////////////////////////
{
    // In this case, render area is what the app wants, so buildNow = TRUE
    constructorCommon(getMouseInput, getKeyboardInput, TRUE);
}

////////////////////////////////////////////////////////////////////////
//
// SoEXTENDER constructor - the subclass tells us whether to build or not
//
SoQtRenderArea::SoQtRenderArea(
    QWidget *parent,
    const char *name, 
    SbBool buildInsideParent, 
    SbBool getMouseInput,
    SbBool getKeyboardInput, 
    SbBool buildNow)
	: SoQtGLWidget(
	    parent,
	    name, 
	    buildInsideParent, 
            SO_GLX_RGB | SO_GLX_DOUBLE | SO_GLX_ZBUFFER | SO_GLX_OVERLAY | SO_GLX_STENCIL,
	    FALSE) // tell GLWidget not to build just yet 
//
////////////////////////////////////////////////////////////////////////
{
    // In this case, render area may be what the app wants, 
    // or it may want a subclass of render area. Pass along buildNow
    // as it was passed to us.
    constructorCommon(getMouseInput, getKeyboardInput, buildNow);
}

////////////////////////////////////////////////////////////////////////
//
// Called by the constructors
//
// private
//
void
SoQtRenderArea::constructorCommon(
    SbBool getMouseInput,
    SbBool getKeyboardInput, 
    SbBool buildNow)
//
////////////////////////////////////////////////////////////////////////
{    


    setClassName(thisClassName);
    
    // set up the device list
    deviceList = new SbPList;

    // default devices
    if (getMouseInput) {
	mouseDevice = new SoQtMouse();
	deviceList->append(mouseDevice);
    }
    else mouseDevice = NULL;
    
    if (getKeyboardInput) {
	keybdDevice = new SoQtKeyboard();
	deviceList->append(keybdDevice);
    }
    else keybdDevice = NULL;
    
    // local vars
    clearFirst = TRUE;
    clearOverlayFirst = TRUE;
    clearZFirst = TRUE;
    autoRedraw = TRUE;
    selection = overlaySelection = NULL;

    setSize(SbVec2s(400, 400));  // default size
    setBorder (TRUE);

    // inventor specific variables
    appEventHandler = NULL;
    
    // the scene manager can be supplied by a subclass.,
    sceneMgr = new SoSceneManager(); 
    sceneMgr->setRenderCallback(renderCB, this);
    
    // the overlay scene manager is supplied right here.
   
    overlaySceneMgr = new SoSceneManager(); 
    overlaySceneMgr->setRenderCallback(renderOverlayCB, this);
    
    if (buildNow)
	setBaseWidget(buildWidget(getParentWidget()));
}

////////////////////////////////////////////////////////////////////////
//
// Destructor
//
SoQtRenderArea::~SoQtRenderArea()
//
////////////////////////////////////////////////////////////////////////
{
    // Remove our callback from old selection node
    if (selection != NULL) {
        selection->removeChangeCallback (SoQtRenderArea::selectionChangeCB, this);

        // Unref the old selection node
        selection->unref();
    }


    if (mouseDevice != NULL) delete mouseDevice;
    if (keybdDevice != NULL) delete keybdDevice;
    delete sceneMgr;
    delete overlaySceneMgr;
    delete deviceList;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//  	Make the new user supplied scene graph the rendering root.
//
// use: virtual public
//
void
SoQtRenderArea::setSceneGraph(SoNode *newScene)
//
////////////////////////////////////////////////////////////////////////
{
    // Deactivate while we change the scene so that our sensors
    // get unhooked before the data changes beneath us.
    sceneMgr->deactivate();
    
    sceneMgr->setSceneGraph(newScene);
    
    // draw new scene graphs to the front buffer by default since
    // the scene will be different (we might has well see something
    // happening for the first redraw).
    if (isDrawToFrontBufferEnable())
	drawToFrontBuffer = TRUE;
    
    // we activate only if we are visible.
    // after all, if we're not on screen, the visibility change
    // callback will invoke activate() once we are on screen.
    if (isVisible() && autoRedraw) {
	sceneMgr->activate();
	sceneMgr->scheduleRedraw();
    }
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//  	sets the overlay plane scene graph.
//
// use: public
//
void
SoQtRenderArea::setOverlaySceneGraph(SoNode *newScene)
//
////////////////////////////////////////////////////////////////////////
{
    if (! getOverlayWidget())
	return;

    // Deactivate while we change the scene so that our sensors
    // get unhooked before the data changes beneath us.
    overlaySceneMgr->deactivate();
    
    overlaySceneMgr->setSceneGraph(newScene);
    
    // we activate only if we are visible.
    // after all, if we're not on screen, the visibility change
    // callback will invoke activate() once we are on screen.
    if (isVisible() && autoRedraw) {
	overlaySceneMgr->activate();
	overlaySceneMgr->scheduleRedraw();
    }
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//  	Return the rendering root.
//
// use: public, virtual
//
SoNode *
SoQtRenderArea::getSceneGraph()
//
////////////////////////////////////////////////////////////////////////
{
    return sceneMgr->getSceneGraph();
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//  	Loads the color map with the given colors.
//
// use: public
//
void
SoQtRenderArea::setColorMap(int startIndex, int num, const SbColor *colors)
//
////////////////////////////////////////////////////////////////////////
{
#if 0
    // save those colors for future uses (if the widget hasn't been
    // built yet, or next time it gets built)
    if (mapColors != NULL)
	free(mapColors);
    mapColors = (XColor *) malloc(sizeof(XColor) * num);
    mapColorNum = num;
    XColor *xcol = mapColors;
    for (int i=0; i<num; i++, xcol++) {
	xcol->red   = (unsigned short) (colors[i][0] * 65535);
	xcol->green = (unsigned short) (colors[i][1] * 65535);
	xcol->blue  = (unsigned short) (colors[i][2] * 65535);
	xcol->flags = DoRed|DoGreen|DoBlue;
	xcol->pixel = startIndex + i;
    }

    // now load those colors into the color map
    if (colorMap != 0)
	XStoreColors(getDisplay(), colorMap, mapColors, mapColorNum);
#endif
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//  	Loads the overlay color map with the given colors.
//
// use: public
//
void
SoQtRenderArea::setOverlayColorMap(int startIndex, int num, const SbColor *colors)
//
////////////////////////////////////////////////////////////////////////
{
#if 0
   if (! getOverlayWidget())
	return;

    // save those colors for future uses (if the widget hasn't been
    // built yet, or next time it gets built)
    if (overlayMapColors != NULL)
	free(overlayMapColors);
    overlayMapColors = (XColor *) malloc(sizeof(XColor) * num);
    overlayMapColorNum = num;
    XColor *xcol = overlayMapColors;
    for (int i=0; i<num; i++, xcol++) {
	xcol->red   = (unsigned short) (colors[i][0] * 65535);
	xcol->green = (unsigned short) (colors[i][1] * 65535);
	xcol->blue  = (unsigned short) (colors[i][2] * 65535);
	xcol->flags = DoRed|DoGreen|DoBlue;
	xcol->pixel = startIndex + i;
    }

    // now load those colors into the color map
    if (overlayColorMap != 0)
	XStoreColors(getDisplay(), overlayColorMap, overlayMapColors, overlayMapColorNum);
#endif
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//  	Sets the transparency type
//
// use: public
//
void
SoQtRenderArea::setTransparencyType(SoGLRenderAction::TransparencyType type)
//
////////////////////////////////////////////////////////////////////////
{
    sceneMgr->getGLRenderAction()->setTransparencyType(type);
    sceneMgr->scheduleRedraw();
}

////////////////////////////////////////////////////////////////////////
//
// Just call buildWidget on the base class (SoQtGLWidget).
//
// use: protected
//
QWidget*
SoQtRenderArea::buildWidget(QWidget* parent)
//
////////////////////////////////////////////////////////////////////////
{
    // Have the parent class build the mgrWidget.
    // Register the class here, though, since this is where resources
    // are needed (other GL widgets don't have resources)
    SoQtGLWidget::buildWidget(parent);

    return getGlxMgrWidget();
}

////////////////////////////////////////////////////////////////////////
//
// Register interest to handle events for the passed device.
// If device is already in the list, then make sure it's enabled.
//
// use: public
//
void SoQtRenderArea::registerDevice (SoQtDevice *device)
//
////////////////////////////////////////////////////////////////////////
{
    // Add the device if it's not already in the list of devices
    int index = deviceList->find(device);
    // ??? mott, shouldn't we return if the device is found ?
    if (index == -1)
	deviceList->append(device);

    // tell the device the window size
    device->setWindowSize(getGlxSize());
#if 0
    // Tell the device to register event interest for our widget
    Widget w = getOverlayWidget() ? getOverlayWidget() : getNormalWidget();
    if ((w != NULL) && (XtWindow(w) != (Window) NULL))
	device->enable(
	    w,
	    (XtEventHandler) SoXtGLWidget::eventHandler,
	    (XtPointer) this,
	    XtWindow(w));
#endif
}

////////////////////////////////////////////////////////////////////////
//
// Unregister interest in the passed device.
//
// use: public
//
void 
SoQtRenderArea::unregisterDevice (SoQtDevice *device)
//
////////////////////////////////////////////////////////////////////////
{
    // Find the device in our device list
    int index = deviceList->find(device);
    if (index == -1)
	return;

    // Remove the device from the list of devices
    deviceList->remove(index);

    // Tell the device to register event interest for our widget
#if 0
Widget w = getOverlayWidget() ? getOverlayWidget() : getNormalWidget();
    if (w)
	device->disable(
	    w,
	    (XtEventHandler) SoXtGLWidget::eventHandler,
	    (XtPointer) this);
#endif
}

#if 0
////////////////////////////////////////////////////////////////////////
//
// disable devices on the old widget, and enables them on the new widget.
//
// use: private
//
void
SoQtRenderArea::reinstallDevices(Widget newWidget)
//
////////////////////////////////////////////////////////////////////////
{
    for (int i = 0; i < deviceList->getLength(); i++) {
	SoXtDevice *device = (SoXtDevice *) (*deviceList)[i];
	
	// disable on the old widget...
	if (deviceWidget)
	    device->disable(
		deviceWidget, 
		(XtEventHandler) SoXtGLWidget::eventHandler,
		(XtPointer) this);
	
	// enable on the new widget....
	if (newWidget) {
	    device->setWindowSize(getGlxSize());
	    device->enable(
		newWidget, 
		(XtEventHandler) SoXtGLWidget::eventHandler,
		(XtPointer) this,
		XtWindow(newWidget));
	}
    }
    
    // ??? add an event handler to receive EnterNotify events to make
    // ??? sure the render area has the current keyboard focus, otherwise
    // ??? keyboard events will go to whichever text field has focus (black
    // ??? outline around widget). This is the prefered behavior since we
    // ??? don't want to force the keyboardFocus to be POINTER or require
    // ??? the user to press a mouse button down.
    //
    // We also need a EnterNotify and LeaveNotify to correctly update
    // stuff for SoLocateHighlight (keep track of current window).
    //
    if (deviceWidget)
	XtRemoveEventHandler(deviceWidget, (EnterWindowMask | LeaveWindowMask), FALSE,
	    (XtEventHandler) SoXtRenderArea::windowEventCB, 
	    (XtPointer) this);
    if (newWidget)
	XtAddEventHandler(newWidget, (EnterWindowMask | LeaveWindowMask), FALSE,
	    (XtEventHandler) SoXtRenderArea::windowEventCB, 
	    (XtPointer) this);
    
    deviceWidget = newWidget;
}
#endif


////////////////////////////////////////////////////////////////////////
//
// Description:
//  	Invoke the app callback and return what the callback returns.
//
// use: protected
//
SbBool
SoQtRenderArea::invokeAppCB(QEvent *anyevent)
//
////////////////////////////////////////////////////////////////////////
{
    // if app wants event, send event to application
    if (appEventHandler != NULL)
	return (*appEventHandler)(appEventHandlerData, anyevent);
    else
	return FALSE;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//  	Process the passed X event.
//
// use: virtual protected
//
void
SoQtRenderArea::processEvent(QEvent *anyevent)
//
////////////////////////////////////////////////////////////////////////
{
    // check if app wants the event...
    if (invokeAppCB(anyevent))
	return;
    
    // ...else send it to the scene
    
    // to do this, translate the X event to an SoEvent
    // by finding a device to perform the translation
    const SoEvent *soevent = NULL;
    for (int i = 0; (soevent == NULL) && (i < deviceList->getLength()); i++) {
        SoQtDevice *device = (SoQtDevice *) (*deviceList)[i];
        soevent = device->translateEvent(anyevent);
    }
    
    // no device found, so return...
    if (! soevent)
	return;
    
    // now send the event first to the overlay scene graph, elses to
    // the regular scene graph.
    SbBool handled = overlaySceneMgr->processEvent(soevent);
    if (! handled) {
        handled = sceneMgr->processEvent (soevent);
    }
    // consume event if it was handled
    anyevent->setAccepted (handled);
}

////////////////////////////////////////////////////////////////////////
//
// Initialize the GLX window for rendering.
// glXMakeCurrent() should have been called before invoking this routine.
//
// use: virtual protected
//
void
SoQtRenderArea::initGraphic()
//
////////////////////////////////////////////////////////////////////////
{
    SoQtGLWidget::initGraphic();
    sceneMgr->reinitialize();
    sceneMgr->setRGBMode(isRGBMode());
    
    SoGLRenderAction *ra = sceneMgr->getGLRenderAction();
    ra->setCacheContext(getDisplayListShareGroup(getNormalContext()));
    ra->setRenderingIsRemote(false);
}

////////////////////////////////////////////////////////////////////////
//
// Initialize the GLX window for rendering.
// glXMakeCurrent() should have been called before invoking this routine.
//
// use: virtual protected
//
void
SoQtRenderArea::initOverlayGraphic()
//
////////////////////////////////////////////////////////////////////////
{
    overlaySceneMgr->reinitialize();
    overlaySceneMgr->setRGBMode(FALSE);
    
    SoGLRenderAction *ra = overlaySceneMgr->getGLRenderAction();
    ra->setCacheContext(getDisplayListShareGroup(getOverlayContext()));
    ra->setRenderingIsRemote(false);
}

////////////////////////////////////////////////////////////////////////
//
// Called by the base class (SoQtGLWidget) when the widget has changed size.
//
// use: virtual protected
//
void
SoQtRenderArea::sizeChanged(const SbVec2s &newSize)
//
////////////////////////////////////////////////////////////////////////
{
    sceneMgr->setWindowSize(newSize);
    overlaySceneMgr->setWindowSize(newSize);
    
    // tell each device the new window size
    for (int i = 0; i < deviceList->getLength(); i++) {
        SoQtDevice *device = (SoQtDevice *) (*deviceList)[i];
        device->setWindowSize(newSize);
    }
}

////////////////////////////////////////////////////////////////////////
//
// Attach the sensor (if necessary).
//
// use: private
//
void
SoQtRenderArea::activate()
//
////////////////////////////////////////////////////////////////////////
{
    // if autoRedraw is off, then don't attach the scene sensor
    if (! autoRedraw)
	return;
    
    // Activate the scene manager
    sceneMgr->activate();
    overlaySceneMgr->activate();
}

////////////////////////////////////////////////////////////////////////
//
// Detach the sensor.
//
// use: private
//
void
SoQtRenderArea::deactivate()
//
////////////////////////////////////////////////////////////////////////
{
    // detach sensors
    sceneMgr->deactivate();
    overlaySceneMgr->deactivate();
}

////////////////////////////////////////////////////////////////////////
//
// Turn auto-redraw on or off. If turning on, then activate the scene
// sensor.
//
// use: public
//
void
SoQtRenderArea::setAutoRedraw(SbBool flag)
//
////////////////////////////////////////////////////////////////////////
{
    if (flag == autoRedraw)
	return;

    autoRedraw = flag;

    if (autoRedraw) {
	if (isVisible())
	    activate();
    }
    else
	deactivate();
}

////////////////////////////////////////////////////////////////////////
//
// Have the scene manager redraw the scene.
//
// use: virtual protected
//
void
SoQtRenderArea::actualRedraw()
//
////////////////////////////////////////////////////////////////////////
{
    sceneMgr->render(clearFirst, clearZFirst);
}

////////////////////////////////////////////////////////////////////////
//
// Have the scene manager redraw the overlay scene.
//
// use: virtual protected
//
void
SoQtRenderArea::actualOverlayRedraw()
//
////////////////////////////////////////////////////////////////////////
{
    overlaySceneMgr->render(clearOverlayFirst);    
}

////////////////////////////////////////////////////////////////////////
//
// This routine draws the scene graph (called by expose events and the
// scene graph sensor)
//
// use: virtual protected
//
void
SoQtRenderArea::redraw()
//
////////////////////////////////////////////////////////////////////////
{
    // return if we are not visible or if we need to wait for an expose
    // event (case when the sensor triggers but we know that an expose
    // event will come along and we don't want 2 redraws).
    if (!isVisible())
	return;
    
    // see if we need to temporary draw to the front buffer (which
    // is set when we display a new scene or get an expose event)
    if (drawToFrontBuffer && isDrawToFrontBufferEnable() && isDoubleBuffer()) {
	
	// ??? workaround bug 301010 - it seems that redrawing to the front
	// ??? of a window that might not be on the screen ( isVisible() might
	// ??? be incorect in a few cases) will confuse the GFX hardware
	// ??? of some machine (Xtrem and Indy24).
	// 
	// ??? Note: this also fixes 298058 (redrawing to the front window
	// ??? on indigo starter gfx, which happened all the time).
	//
	if (isRGBMode()) {
	    SbColor color = getBackgroundColor();
	    glClearColor(color[0], color[1], color[2], 0);
	} else
	    glClearIndex(getBackgroundIndex());
	glClear(GL_COLOR_BUFFER_BIT);
	swapNormalBuffers();
	// ??? end of BUG workaround
	
	glReadBuffer(GL_FRONT); // Needed for acbuf antialiasing
	glDrawBuffer(GL_FRONT);
    }
    
    // draw that scene! (subclasses may redefine...)
    actualRedraw();
    
    // swap those buffers!
    if (isDoubleBuffer()) {
	if (drawToFrontBuffer && isDrawToFrontBufferEnable()) {
	    // no need to swap here - instead restore the buffer and 
	    // clear the flag now that we have drawn to the front buffer
	    glReadBuffer(GL_BACK); // Needed for acbuf antialiasing
	    glDrawBuffer(GL_BACK);
	    glFlush();
	}
	else
	    swapNormalBuffers();
    }
    else
	glFlush();
    
    // clear this flag now that we have drawn
    drawToFrontBuffer = FALSE;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//	Sets antialiasing on GL rendering action.
//
// use: public
//
void
SoQtRenderArea::setAntialiasing (SbBool smoothing, int numPasses)
//
////////////////////////////////////////////////////////////////////////
{
    SoGLRenderAction *ra = getGLRenderAction();

    if (smoothing != ra->isSmoothing() || numPasses != ra->getNumPasses()) {

        // this must probably set first (don't know when the redraw happens
        // from the format change:
        sceneMgr->setAntialiasing(smoothing, numPasses);

        QGLFormat format = this->getNormalVisual();
        // TODO: is this the right thing to do? (usi)
        if (format.accum() != (bool)smoothing) {
            format.setAccum ((bool)smoothing);
            setNormalVisual (format);
        }
        /* or is this the right thing:
        if (format.sampleBuffers() != (bool)smoothing || format.samples != numPasses) {
            format.setSampleBuffers ((bool)smoothing);
            format.setSamples (numPasses);
            setFormat (format);
        }
        */
    }
}



////////////////////////////////////////////////////////////////////////
//
// This routine draws the scene graph for the overlay bit planes (called
// by the expose event and scene graph sensor).
//
// use: virtual protected
//
void
SoQtRenderArea::redrawOverlay()
//
////////////////////////////////////////////////////////////////////////
{
    if (!isVisible())
	return;
    
    // draw that scene! (subclasses may redefine...)
    actualOverlayRedraw();
    
    glFlush();
}

////////////////////////////////////////////////////////////////////////
//
// Schedule a redraw to happen sometime soon.
//
// use: public
//
void
SoQtRenderArea::scheduleRedraw()
//
////////////////////////////////////////////////////////////////////////
{
    if (isAutoRedraw())
        SoQtGLWidget::scheduleRedraw();
}

////////////////////////////////////////////////////////////////////////
//
// Schedule a redraw to happen sometime soon.
//
// use: public
//
void
SoQtRenderArea::scheduleOverlayRedraw()
//
////////////////////////////////////////////////////////////////////////
{
    if (isAutoRedraw())
	overlaySceneMgr->scheduleRedraw();
    else
	redrawOverlay(); 
}

////////////////////////////////////////////////////////////////////////
//
// Convenience to set up the redraw mechansim whenever the selection
// changes.
//
// public
//
void
SoQtRenderArea::redrawOnSelectionChange (SoSelection *s)
//
////////////////////////////////////////////////////////////////////////
{
    // Ref the new selection node
    if (s != NULL)
	s->ref();
	
    // Remove our callback from old selection node
    if (selection != NULL) {
	selection->removeChangeCallback(SoQtRenderArea::selectionChangeCB, this);
	
	// Unref the old selection node
	selection->unref(); 
    }

    selection = s;
    
    // Add our callback to this selection node. (We've already ref'd this new sel node)
    if (selection != NULL)
	selection->addChangeCallback(SoQtRenderArea::selectionChangeCB, this);
}

////////////////////////////////////////////////////////////////////////
//
// Convenience to set up the redraw mechansim whenever the selection
// changes.
//
// public
//
void
SoQtRenderArea::redrawOverlayOnSelectionChange(SoSelection *s)
//
////////////////////////////////////////////////////////////////////////
{
    // Remove our callback from old selection node
    if (overlaySelection != NULL)
	overlaySelection->removeChangeCallback(SoQtRenderArea::overlaySelectionChangeCB, this);

    overlaySelection = s;
    
    // Add our callback to this selection node
    if (overlaySelection != NULL)
	overlaySelection->addChangeCallback(SoQtRenderArea::overlaySelectionChangeCB, this);
}

//
// called whenever the component becomes visible or not
//
void
SoQtRenderArea::visibilityChanged(bool visible)
{
    
    if (visible)
	activate();
    else {
	deactivate();
    }
}
void
SoQtRenderArea::selectionChangeCB(void *p, SoSelection *)
{
    ((SoQtRenderArea *)p)->scheduleRedraw();
}

void
SoQtRenderArea::overlaySelectionChangeCB(void *p, SoSelection *)
{
    ((SoQtRenderArea *)p)->scheduleOverlayRedraw();
}

void
SoQtRenderArea::renderCB(void *p, SoSceneManager *)
{
    ((SoQtRenderArea *)p)->scheduleRedraw();
}

void
SoQtRenderArea::renderOverlayCB(void *p, SoSceneManager *)
{
    ((SoQtRenderArea *)p)->redrawOverlay();
}
