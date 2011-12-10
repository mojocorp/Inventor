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
 * Copyright (C) 1990,91   Silicon Graphics, Inc.
 *
 _______________________________________________________________________
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 |
 |   $Revision: 1.3 $
 |
 |   Classes	: SoQtPlaneViewer
 |
 |   Author(s)	: Alain Dumesny
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

/*
 * Ported to Qt by Morgan Leborgne, 2010
 */

#include <QApplication>
#include <QPushButton>
#include <QAction>
#include <QToolBar>
#include <QKeyEvent>
#include <QMouseEvent>

#include <math.h>

#include <Inventor/SbPList.h>
#include <Inventor/nodes/SoOrthographicCamera.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/Qt/SoQtCursors.h>
#include <Inventor/Qt/viewers/SoQtPlaneViewer.h>
#include <Inventor/Qt/SoQtIcons.h>

//#include "SoQtBitmapButton.h"
#include <Inventor/misc/SoGL.h>

/*
 *  Defines
 */

enum ViewerModes {
    PICK_MODE, 
    VIEW_MODE,
    DOLLY_MODE_ACTIVE, 
    PAN_MODE, 
    PAN_MODE_ACTIVE, 
    ROLL_MODE_ACTIVE, 
    SEEK_MODE
};

// Resources for labels.
typedef struct {
    const char *planeViewer;
    const char *transX;
    const char *transY;
    const char *planeViewerPreferenceSheet;
    const char *dolly;
    const char *zoom;
} RES_LABELS;
static RES_LABELS rl;
static const char *defaultLabel[]={
	"Plane Viewer",  
	"transX", 
	"transY",
	"Plane Viewer Preference Sheet",
	"Dolly",
	"Zoom"
};


////////////////////////////////////////////////////////////////////////
//
// Public constructor - build the widget right now
//
SoQtPlaneViewer::SoQtPlaneViewer(
    QWidget *parent,
    const char *name, 
    SbBool buildInsideParent, 
    SoQtFullViewer::BuildFlag b, 
    SoQtViewer::Type t)
	: SoQtFullViewer(
	    parent,
	    name, 
	    buildInsideParent, 
	    b, 
	    t, 
	    FALSE) // tell base class not to build just yet  
//
////////////////////////////////////////////////////////////////////////
{
    // In this case, render area is what the app wants, so buildNow = TRUE
    constructorCommon(TRUE);
}

////////////////////////////////////////////////////////////////////////
//
// SoEXTENDER constructor - the subclass tells us whether to build or not
//
SoQtPlaneViewer::SoQtPlaneViewer(
    QWidget *parent,
    const char *name, 
    SbBool buildInsideParent, 
    SoQtFullViewer::BuildFlag b, 
    SoQtViewer::Type t, 
    SbBool buildNow)
	: SoQtFullViewer(
	    parent,
	    name, 
	    buildInsideParent, 
	    b, 
	    t, 
	    FALSE) // tell base class not to build just yet  
//
////////////////////////////////////////////////////////////////////////
{
    // In this case, render area may be what the app wants, 
    // or it may want a subclass of render area. Pass along buildNow
    // as it was passed to us.
    constructorCommon(buildNow);
}

////////////////////////////////////////////////////////////////////////
//
// Called by the constructors
//
// private
//
void
SoQtPlaneViewer::constructorCommon(SbBool buildNow)
//
////////////////////////////////////////////////////////////////////////
{
    // init local vars
    mode = isViewing() ? VIEW_MODE : PICK_MODE;
    createdCursors = FALSE;
    //ML transCursor = dollyCursor = seekCursor = 0;
    setSize( SbVec2s(520, 470) );  // default size
    setClassName("SoQtPlaneViewer");

    // Build the widget tree, and let SoQtComponent know about our base widget.
    if (buildNow) {
        QWidget* w = buildWidget(getParentWidget());
	setBaseWidget(w);
    }
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Destructor.
//
// Use: public

SoQtPlaneViewer::~SoQtPlaneViewer()
//
////////////////////////////////////////////////////////////////////////
{

}

////////////////////////////////////////////////////////////////////////
//
// Description:
//	Build the parent class widget, then add buttons.
//
// Use: protected
QWidget*
SoQtPlaneViewer::buildWidget(QWidget* parent)
//
////////////////////////////////////////////////////////////////////////
{
    rl.planeViewer = defaultLabel[0];
    setPopupMenuString( rl.planeViewer );

    // Create the root widget and register it with a class name
    QWidget* w = SoQtFullViewer::buildWidget(parent);

    // get resources...

    rl.transX = defaultLabel[1];
    rl.transY = defaultLabel[2];
    rl.planeViewerPreferenceSheet = defaultLabel[3];
    rl.dolly = defaultLabel[4];
    rl.zoom = defaultLabel[5];

    // assign decoration titles
    setBottomWheelString( rl.transX );
    setLeftWheelString( rl. transY );
    setPrefSheetString( rl.planeViewerPreferenceSheet );

    return w;
}    

////////////////////////////////////////////////////////////////////////
//
// Description:
//	add our own button to the existing list
//
// Use: virtual protected
void
SoQtPlaneViewer::createViewerButtons(QToolBar* parent)
//
////////////////////////////////////////////////////////////////////////
{
    // get the default buttons
    SoQtFullViewer::createViewerButtons(parent);

    xAction = new QAction(SoQtIcon::getIcon(SoQtIcon::X), tr("X"), this);
    connect (xAction, SIGNAL (triggered(bool)), SLOT (xButtonClicked()));

    yAction = new QAction(SoQtIcon::getIcon(SoQtIcon::Y), tr("Y"), this);
    connect (yAction, SIGNAL (triggered(bool)), SLOT (yButtonClicked()));

    zAction = new QAction(SoQtIcon::getIcon(SoQtIcon::Z), tr("Z"), this);
    connect (zAction, SIGNAL (triggered(bool)), SLOT (zButtonClicked()));

    cameraAction = new QAction(SoQtIcon::getIcon(SoQtIcon::PERSP), tr("Perspective"), this);
    cameraAction->setCheckable(true);
    connect (cameraAction, SIGNAL (triggered(bool)), SLOT (camButtonClicked()));

    parent->addAction (xAction);
    parent->addAction (yAction);
    parent->addAction (zAction);
    parent->addAction (cameraAction);
}


////////////////////////////////////////////////////////////////////////
//
// Description:
//	Call the base class and sets the correct cursor on the window
//
// Use: virtual public
void
SoQtPlaneViewer::setViewing(SbBool flag)
//
////////////////////////////////////////////////////////////////////////
{
    if (flag == viewingFlag)
	return;
    
    // call the base class
    SoQtFullViewer::setViewing(flag);
    
    switchMode(isViewing() ? VIEW_MODE : PICK_MODE);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//	Enables/Disable the viewer cursor on the window.
//
// Use: virtual public
void
SoQtPlaneViewer::setCursorEnabled(SbBool flag)
//
////////////////////////////////////////////////////////////////////////
{
    if (flag == cursorEnabledFlag)
	return;
    
    cursorEnabledFlag = flag;
    
    if (! isViewing())
	return;
    
    updateCursor();
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//	Call the parent class and change the right thumbwheel label.
//
// Use: virtual public
void
SoQtPlaneViewer::setCamera(SoCamera *newCamera)
//
////////////////////////////////////////////////////////////////////////
{
    if (camera == newCamera)
	return;

    // set the right thumbwheel label and toggle button image based on 
    // the camera type
    if (newCamera != NULL && (camera == NULL || 
	newCamera->getTypeId() != camera->getTypeId())) {
	if (newCamera->isOfType(SoOrthographicCamera::getClassTypeId())) {
            cameraAction->setIcon (SoQtIcon::getIcon (SoQtIcon::ORTHO));
	    setRightWheelString( rl.zoom );
	}
	else {
            cameraAction->setIcon (SoQtIcon::getIcon (SoQtIcon::PERSP));
	    setRightWheelString( rl.dolly );
	}
    }

    // call parent class
    SoQtFullViewer::setCamera(newCamera);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Process the given event to do viewing stuff
//
// Use: virtual protected
void
SoQtPlaneViewer::processEvent(QEvent *qe)
//
////////////////////////////////////////////////////////////////////////
{
    if ( processCommonEvents(qe) )
	return;
    
    if (!createdCursors)
	updateCursor();

    QMouseEvent    *be;
    QMouseEvent    *me;
    QKeyEvent	    *ke;
    
    Qt::KeyboardModifiers modifiers = QApplication::keyboardModifiers();
    Qt::MouseButtons buttons = QApplication::mouseButtons();

    SbVec2s raSize = getGlxSize();
    
    switch(qe->type()) {
    case QEvent::MouseButtonPress:
    case QEvent::MouseButtonRelease:
        be = (QMouseEvent *)qe;
        if (be->button() != Qt::LeftButton && be->button() != Qt::MidButton)
	    break;
	
        locator[0] = be->x();
        locator[1] = raSize[1] - be->y();
	if (mode == SEEK_MODE) {
            if (qe->type() == QEvent::MouseButtonPress)
		seekToPoint(locator);
	}
	else {
            if (qe->type() == QEvent::MouseButtonPress)
		interactiveCountInc();
	    else // ButtonRelease
		interactiveCountDec();
            updateViewerMode(be->modifiers(), be->buttons());
	}
	break;
	
    case QEvent::KeyPress:
    case QEvent::KeyRelease:
        ke = (QKeyEvent *)qe;
	
        locator[0] = QCursor::pos().x();
        locator[1] = raSize[1] - QCursor::pos().y();
        if (ke->modifiers() & Qt::ControlModifier)
            updateViewerMode(ke->modifiers(), buttons);
	break;
	
    case QEvent::MouseMove:
        me = (QMouseEvent *)qe;
	switch (mode) {
	    case DOLLY_MODE_ACTIVE:
                dollyCamera( SbVec2s(me->x(), raSize[1] - me->y()) );
		break;
	    case PAN_MODE_ACTIVE:
                translateCamera(SbVec2f(me->x()/float(raSize[0]), (raSize[1] - me->y())/float(raSize[1])));
		break;
	    case ROLL_MODE_ACTIVE:
                rollCamera( SbVec2s(me->x(), raSize[1] - me->y()) );
		break;
	}
	break;
	
    case QEvent::Leave:
    case QEvent::Enter:
	//
	// because the application might use Ctrl-key for motif menu
	// accelerators we might not receive a key-up event, so make sure
	// to reset any Ctrl mode if we loose focus, but don't do anything
	// if Ctrl-key is not down (nothing to do) or if a mouse button 
	// is down (we will get another leaveNotify).
	//
        if (! (modifiers & Qt::ControlModifier))
	    break;
        if (buttons & Qt::LeftButton || buttons & Qt::MidButton)
	    break;
        if (qe->type() == QEvent::Leave)
	    switchMode(VIEW_MODE);
	else
            updateViewerMode(modifiers, buttons);
	break;
    default: break;
    }
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    sets the viewer mode based on what keys and buttons are being pressed
//
// Use: private
void
SoQtPlaneViewer::updateViewerMode(int modifiers, int buttons)
//
////////////////////////////////////////////////////////////////////////
{
    // ??? WARNING - this routine ONLY works because of 
    // ??? SoQtViewer::updateEventState() which is called for us
    // ??? by SoQtViewer::processCommonEvents(). 
    // ??? (XEvent state for button and modifier keys is not updated
    // ??? until after the event is received. WEIRD)
    
    // LEFT+MIDDLE down
    if (buttons & Qt::LeftButton && buttons & Qt::MidButton) {
	switchMode(DOLLY_MODE_ACTIVE);
    }
    
    // LEFT down
    else if (buttons & Qt::LeftButton) {
        if (modifiers & Qt::ControlModifier)
	    switchMode(PAN_MODE_ACTIVE);
	else
	    switchMode(DOLLY_MODE_ACTIVE);
    }
    
    // MIDDLE DOWN
    else if (buttons & Qt::MidButton) {
        if (modifiers & Qt::ControlModifier)
	    switchMode(ROLL_MODE_ACTIVE);
	else
	    switchMode(PAN_MODE_ACTIVE);
    }
    
    // no buttons down...
    else {
        if (modifiers & Qt::ControlModifier)
	    switchMode(PAN_MODE);
	else
	    switchMode(VIEW_MODE);
    }
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    switches to the specified viewer mode
//
// Use: private
void
SoQtPlaneViewer::switchMode(int newMode)
//
////////////////////////////////////////////////////////////////////////
{
    // assing new mode
    SbBool redrawNeeded = FALSE;
    int prevMode = mode;
    mode = newMode;
    
    // update the cursor
    updateCursor();
    
    // check the old viewer mode for redraw need
    if (prevMode == ROLL_MODE_ACTIVE)
	redrawNeeded = TRUE;
    
    // switch to new viewer mode
    switch (newMode) {
	case PICK_MODE:
            if (QApplication::mouseButtons() & Qt::LeftButton && prevMode != SEEK_MODE)
                interactiveCountDec();
            if (QApplication::mouseButtons() & Qt::MidButton && prevMode != SEEK_MODE)
                interactiveCountDec();
	    break;
	    
	case PAN_MODE_ACTIVE:
	    {
	    // Figure out the focal plane
	    SbMatrix mx;
	    mx = camera->orientation.getValue();
	    SbVec3f forward(-mx[2][0], -mx[2][1], -mx[2][2]);
	    SbVec3f fp = camera->position.getValue() + 
		forward * camera->focalDistance.getValue();
	    focalplane = SbPlane(forward, fp);
	    
	    // map mouse starting position onto the panning plane
	    SbVec2s raSize = getGlxSize();
	    SbViewVolume    cameraVolume;
	    SbLine	    line;
	    cameraVolume = camera->getViewVolume(raSize[0]/float(raSize[1]));
	    cameraVolume.projectPointToLine(
		SbVec2f(locator[0]/float(raSize[0]), locator[1]/float(raSize[1])), line);
	    focalplane.intersect(line, locator3D);
	    }
	    break;
	    
	case ROLL_MODE_ACTIVE:
	    redrawNeeded = TRUE;
	    break;
    }
    
    if (redrawNeeded)
	scheduleRedraw();
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    switches to the specified viewer mode
//
// Use: private
void
SoQtPlaneViewer::updateCursor()
//
////////////////////////////////////////////////////////////////////////
{
    if (! createdCursors)
	defineCursors();
    
    // the viewer cursor are not enabled, then we don't set a new cursor.
    // Instead erase the old viewer cursor.
    if (! cursorEnabledFlag) {
        unsetCursor();
	return;
    }
    
    // ...else set the right cursor for the viewer mode....
    switch(mode) {
	case PICK_MODE:
	case ROLL_MODE_ACTIVE:
            unsetCursor();
	    break;
	    
	case VIEW_MODE:
	case DOLLY_MODE_ACTIVE:
            setCursor(dollyCursor);
	    break;
	    
	case PAN_MODE:
	case PAN_MODE_ACTIVE:
            setCursor(transCursor);
	    break;
	    
	case SEEK_MODE:
            setCursor(seekCursor);
	    break;
    }
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    draws viewer feedback during a render area redraw of the scene.
//
// Use: virtual protected
void
SoQtPlaneViewer::actualRedraw()
//
////////////////////////////////////////////////////////////////////////
{
    // have the base class draw the scene
    SoQtFullViewer::actualRedraw();
    
    // now draw the viewer feedback
    if (isViewing() && mode == ROLL_MODE_ACTIVE) {
	
	setFeedbackOrthoProjection(getGlxSize());
	
	drawViewerRollFeedback(getGlxSize()/2, locator);
	
	// now restore state
	restoreGLStateAfterFeedback();
    }
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//	Call the base class and sets the correct cursor on the window
//
// Use: virtual protected
void
SoQtPlaneViewer::setSeekMode(SbBool flag)
//
////////////////////////////////////////////////////////////////////////
{
    if ( !isViewing() )
	return;
    
    // call the base class
    SoQtFullViewer::setSeekMode(flag);
    
    mode = isSeekMode() ? SEEK_MODE : VIEW_MODE;
    
    updateCursor();
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//	Redefine this routine to add some viewer specific stuff.
//
// Use: virtual protected
void
SoQtPlaneViewer::createPrefSheet()
//
////////////////////////////////////////////////////////////////////////
{
#if 0
    // create the preference sheet shell and form widget
    Widget shell, form;
    createPrefSheetShellAndForm(shell, form);
    
    // create all of the parts
    Widget widgetList[20];
    int num = 0;
    createDefaultPrefSheetParts(widgetList, num, form);
    
    layoutPartsAndMapPrefSheet(widgetList, num, form, shell);
#endif
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//	Redefine this to keep the same camera rotation when seeking
//  
// Use: virtual protected

void
SoQtPlaneViewer::computeSeekFinalOrientation()
//
////////////////////////////////////////////////////////////////////////
{
    newCamOrientation = camera->orientation.getValue();
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//	Brings the viewer help card (called by "?" push button)
//
// Use: virtual protected
void
SoQtPlaneViewer::openViewerHelpCard()
//
////////////////////////////////////////////////////////////////////////
{
    // tell the base class to open the file for us
    openHelpCard("SoQtPlaneViewer.help");
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Translate the camera right/left (called by thumb wheel).
//
// Use: virtual protected

void
SoQtPlaneViewer::bottomWheelMotion(float newVal)
//
////////////////////////////////////////////////////////////////////////
{
    if (camera == NULL)
	return;
    
    // get camera right vector and translate camera by given amount
    SbMatrix mx;
    mx = camera->orientation.getValue();
    SbVec3f rightVector(mx[0][0], mx[0][1], mx[0][2]);
    float dist = transXspeed * (bottomWheelVal - newVal);
    camera->position = camera->position.getValue() + dist * rightVector;
    
    bottomWheelVal = newVal;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Translate the camera up/down (called by thumb wheel).
//
// Use: virtual protected

void
SoQtPlaneViewer::leftWheelMotion(float newVal)
//
////////////////////////////////////////////////////////////////////////
{
    if (camera == NULL)
	return;
    
    // get camera up vector and translate camera by given amount
    SbMatrix mx;
    mx = camera->orientation.getValue();
    SbVec3f upVector(mx[1][0], mx[1][1], mx[1][2]);
    float dist = transYspeed * (leftWheelVal - newVal);
    camera->position = camera->position.getValue() + dist * upVector;
    
    leftWheelVal = newVal;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//   	Moves the camera closer/further away from the plane of interest
//  (perspective camera case), else change the camera height (orthographic
//  camera case).
//
// Use: virtual protected

void
SoQtPlaneViewer::rightWheelMotion(float newVal)
//
////////////////////////////////////////////////////////////////////////
{
    if (camera == NULL)
	return;
    
    if (camera->isOfType(SoOrthographicCamera::getClassTypeId())) {
	// change the ortho camera height
	SoOrthographicCamera *cam = (SoOrthographicCamera *) camera;
	cam->height = cam->height.getValue() * powf(2.0, newVal - rightWheelVal);
    }
    else {
	// shorter/grow the focal distance given the wheel rotation
	float focalDistance = camera->focalDistance.getValue();;
	float newFocalDist = focalDistance;
	newFocalDist *= powf(2.0, newVal - rightWheelVal);
	
	// finally reposition the camera
	SbMatrix mx;
	mx = camera->orientation.getValue();
	SbVec3f forward(-mx[2][0], -mx[2][1], -mx[2][2]);
	camera->position = camera->position.getValue() + 
			   (focalDistance - newFocalDist) * forward;
	camera->focalDistance = newFocalDist;
    }
    
    rightWheelVal = newVal;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    This routine is used to define cursors (can only be called after
//  window has been realized).
//
// Use: private

void
SoQtPlaneViewer::defineCursors()
//
////////////////////////////////////////////////////////////////////////
{
    // view plane translate cursor
    transCursor = SoQtCursor::getCursor(SoQtCursor::FLAT_HAND);
    
    // dolly cursor
    dollyCursor = SoQtCursor::getCursor(SoQtCursor::POINTING_HAND);
    
    // seek cursor
    seekCursor = SoQtCursor::getCursor(SoQtCursor::TARGET);

    createdCursors = TRUE;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Rolls the camera around it's forward direction given the new mouse
//  location.
//
// Use: private

void
SoQtPlaneViewer::rollCamera(const SbVec2s &newLocator)
//
////////////////////////////////////////////////////////////////////////
{
    if (camera == NULL)
	return;
    
    SbVec2s center = getGlxSize()/2;
    SbVec2s p1, p2;
    float angle;
    
    // get angle of rotation
    p1 = locator - center;
    p2 = newLocator - center;
    // checking needed so that NaN won't occur
    angle  = (p2[0]==0 && p2[1]==0) ? 0 : atan2(float(p2[1]), float(p2[0]));
    angle -= (p1[0]==0 && p1[1]==0) ? 0 : atan2(float(p1[1]), float(p1[0]));
    
    // now find the rotation and rotate camera
    SbVec3f axis(0, 0, -1);
    SbRotation rot;
    rot.setValue(axis, angle);
    camera->orientation = rot * camera->orientation.getValue();
    
    locator = newLocator;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Moves the camera into the plane defined by the camera forward vector
//  and the focal point to follow the new mouse location.
//
// Use: private

void
SoQtPlaneViewer::translateCamera(const SbVec2f &newLocator)
//
////////////////////////////////////////////////////////////////////////
{
    if (camera == NULL)
	return;
    
    // map new mouse location into the camera focal plane
    SbViewVolume    cameraVolume;
    SbLine	    line;
    SbVec3f	    newLocator3D;
    SbVec2s	    raSize = getGlxSize();
    cameraVolume = camera->getViewVolume(raSize[0]/float(raSize[1]));
    cameraVolume.projectPointToLine(newLocator, line);
    focalplane.intersect(line, newLocator3D);
    
    // move the camera by the delta 3D position amount
    camera->position = camera->position.getValue() + 
	(locator3D - newLocator3D);
    
    // You would think we would have to set locator3D to
    // newLocator3D here.  But we don't, because moving the camera
    // essentially makes locator3D equal to newLocator3D in the
    // transformed space, and we will project the next newLocator3D in
    // this transformed space.
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Moves the camera forward/backward based on the new mouse potion.
//  (perspective camera), else change the camera height (orthographic
//  camera case).
//
// Use: private

void
SoQtPlaneViewer::dollyCamera(const SbVec2s &newLocator)
//
////////////////////////////////////////////////////////////////////////
{
    if (camera == NULL)
	return;
    
    // moving the mouse up/down will move the camera futher/closer.
    // moving the camera sideway will not move the camera at all
    float d = (newLocator[1] - locator[1]) / 40.0;
    
    if (camera->isOfType(SoOrthographicCamera::getClassTypeId())) {
	// change the ortho camera height
	SoOrthographicCamera *cam = (SoOrthographicCamera *) camera;
	cam->height = cam->height.getValue() * powf(2.0, d);
    }
    else {
	// shorter/grow the focal distance given the mouse move
	float focalDistance = camera->focalDistance.getValue();;
	float newFocalDist = focalDistance * powf(2.0, d);
	
	// finally reposition the camera
	SbMatrix mx;
	mx = camera->orientation.getValue();
	SbVec3f forward(-mx[2][0], -mx[2][1], -mx[2][2]);
	camera->position = camera->position.getValue() + 
			   (focalDistance - newFocalDist) * forward;
	camera->focalDistance = newFocalDist;
    }
    
    locator = newLocator;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Moves the camera to be aligned with the given plane
//
// Use: private

void
SoQtPlaneViewer::setPlane(const SbVec3f &newNormal, const SbVec3f &newRight)
//
////////////////////////////////////////////////////////////////////////
{
    if (camera == NULL)
	return;
    
    // get center of rotation
    SbRotation camRot = camera->orientation.getValue();
    float radius = camera->focalDistance.getValue();
    SbMatrix mx;
    mx = camRot;
    SbVec3f forward( -mx[2][0], -mx[2][1], -mx[2][2]);
    SbVec3f center = camera->position.getValue()
	+ radius * forward;
    
    // rotate the camera to be aligned with the new plane normal
    SbRotation rot( -forward, newNormal);
    camRot = camRot * rot;
    
    // rotate the camera to be aligned with new right direction
    mx = camRot;
    SbVec3f right(mx[0][0], mx[0][1], mx[0][2]);
    rot.setValue(right, newRight);
    camRot = camRot * rot;
    camera->orientation = camRot;
    
    // reposition camera to look at pt of interest
    mx = camRot;
    forward.setValue( -mx[2][0], -mx[2][1], -mx[2][2]);
    camera->position = center - radius * forward;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Called by the bottom and left thumbwheels to compute the 
//  translation factors (how fast should we translate given a wheel
//  rotation).
//
// Use: private

void
SoQtPlaneViewer::computeTranslateValues()
//
////////////////////////////////////////////////////////////////////////
{
    if (camera == NULL)
	return;
    
    float height;
    
    if (camera->isOfType(SoPerspectiveCamera::getClassTypeId())) {
	float angle = ((SoPerspectiveCamera *)camera)->heightAngle.getValue();
	float dist = camera->focalDistance.getValue();
	height = dist * tanf(angle);
    }
    else if (camera->isOfType(SoOrthographicCamera::getClassTypeId()))
	height = ((SoOrthographicCamera *)camera)->height.getValue();
    
    transYspeed = height / 2;
    transXspeed = transYspeed * camera->aspectRatio.getValue();
}

//
// redefine those generic virtual functions
//
const char *
SoQtPlaneViewer::getDefaultWidgetName() const
{ return "SoXtPlaneViewer"; }

const char *
SoQtPlaneViewer::getDefaultTitle() const
{ return rl.planeViewer; }

const char *
SoQtPlaneViewer::getDefaultIconTitle() const
{ return rl.planeViewer; }


void
SoQtPlaneViewer::bottomWheelStart()
{
    interactiveCountInc();
    computeTranslateValues();
}

void
SoQtPlaneViewer::leftWheelStart()
{
    interactiveCountInc();
    computeTranslateValues();
}


//
////////////////////////////////////////////////////////////////////////
// static callbacks stubs
////////////////////////////////////////////////////////////////////////
//


//
// viewer push button callbacks
//
void
SoQtPlaneViewer::xButtonClicked()
{
    setPlane( SbVec3f(1, 0, 0), SbVec3f(0, 0, -1) );
}

void
SoQtPlaneViewer::yButtonClicked()
{
    setPlane( SbVec3f(0, 1, 0), SbVec3f(1, 0, 0) );
}

void
SoQtPlaneViewer::zButtonClicked()
{
    setPlane( SbVec3f(0, 0, 1), SbVec3f(1, 0, 0) );
}

void
SoQtPlaneViewer::camButtonClicked()
{
    toggleCameraType();
}
