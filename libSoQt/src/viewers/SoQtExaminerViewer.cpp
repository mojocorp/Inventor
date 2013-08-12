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
 * Copyright (C) 1990-95   Silicon Graphics, Inc.
 *
 _______________________________________________________________________
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 |
 |   $Revision: 1.2 $
 |
 |   Classes	: SoQtExaminerViewer
 |
 |   Author(s)	: Alain Dumesny
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

/*
 * Ported to Qt by Morgan Leborgne, 2010
 */

#include <stdlib.h>
#include <math.h>

#include <Inventor/SoDB.h>
#include <Inventor/SoFullPath.h>
#include <Inventor/SoInput.h>
#include <Inventor/sensors/SoFieldSensor.h>
#include <Inventor/fields/SoSFTime.h>
#include <Inventor/nodes/SoOrthographicCamera.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/nodes/SoScale.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/nodes/SoTranslation.h>
#include <Inventor/projectors/SbSphereSheetProjector.h>
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/Qt/SoQt.h>
#include <Inventor/Qt/SoQtIcons.h>
#include <Inventor/Qt/viewers/SoQtExaminerViewer.h>
#include <Inventor/Qt/SoQtCursors.h>
#include <Inventor/Qt/viewers/SoQtPreferenceDialog.h>

#ifdef DEBUG
#include <Inventor/errors/SoDebugError.h>
#include <SoDebug.h>
#endif

#include <QAction>
#include <QApplication>
#include <QBoxLayout>
#include <QCheckBox>
#include <QDoubleSpinBox>
#include <QEvent>
#include <QKeyEvent>
#include <QLabel>
#include <QMessageBox>
#include <QMouseEvent>
#include <QTabWidget>
#include <QToolBar>
#include <Inventor/Qt/SoQtThumbWheel.h>
/*
 *  Defines
 */

enum ViewerModes {
    PICK_MODE, 
    VIEW_MODE, 
    SPIN_MODE_ACTIVE, 
    PAN_MODE, 
    PAN_MODE_ACTIVE, 
    DOLLY_MODE_ACTIVE, 
    SEEK_MODE
};
// size of the rotation buffer, which is used to animate the spinning ball.
#define ROT_BUFF_SIZE 3


// Resources for labels.
typedef struct {
    const char *examinViewer;
    const char *roty;
    const char *rotx;
    const char *preferenceSheet;
    const char *zoom;
    const char *dolly;
    const char *axesSizeLabel;
} RES_LABELS;
static RES_LABELS rl;

static const char *defaultLabel[]={
	"Examiner Viewer",  
	"Roty", 
	"Rotx",
	"Examiner Viewer Preference Sheet",
	"Zoom",  
	"Dolly",   
	"axes size:"
};


//
// The point of interest geometry description
//
const char *SoQtExaminerViewer::geometryBuffer = "\
#Inventor V2.0 ascii\n\
\
Separator { \
    PickStyle { style UNPICKABLE } \
    LightModel { model BASE_COLOR } \
    MaterialBinding { value PER_PART } \
    DrawStyle { lineWidth 2 } \
    Coordinate3 { point [0 0 0, 1 0 0, 0 1 0, 0 0 1] } \
    BaseColor { rgb [1 0 0, 0 1 0, 0 0 1] } \
    IndexedLineSet { coordIndex [1, 0, 2, -1, 0, 3] } \
     \
    LightModel { model PHONG } \
    MaterialBinding { value OVERALL } \
    Complexity { value .1 } \
    Separator { \
        Material { \
            diffuseColor    [ 0.5 0 0 ] \
            emissiveColor   [ 0.5 0 0 ] \
        } \
        Translation { translation 1 0 0 } \
        RotationXYZ { axis Z angle -1.570796327 } \
        Cone { bottomRadius .2 height .3 } \
    } \
    Separator { \
        Material { \
            diffuseColor    [ 0 0.5 0 ] \
            emissiveColor   [ 0 0.5 0 ] \
        } \
        Translation { translation 0 1 0 } \
        Cone { bottomRadius .2 height .3 } \
    } \
    Material { \
        diffuseColor    [ 0 0 0.5 ] \
        emissiveColor   [ 0 0 0.5 ] \
    } \
    Translation { translation 0 0 1 } \
    RotationXYZ { axis X angle 1.570796327 } \
    Cone { bottomRadius .2 height .3 } \
} ";


static const char *thisClassName = "SoQtExaminerViewer";

////////////////////////////////////////////////////////////////////////
//
// Public constructor - build the widget right now
//
SoQtExaminerViewer::SoQtExaminerViewer(
    QWidget* parent,
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
	    FALSE) // tell GLWidget not to build just yet  
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
SoQtExaminerViewer::SoQtExaminerViewer(
    QWidget* parent,
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
	   FALSE) // tell GLWidget not to build just yet  
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
SoQtExaminerViewer::constructorCommon(SbBool buildNow)
//
////////////////////////////////////////////////////////////////////////
{
    // init local vars
    mode = isViewing() ? VIEW_MODE : PICK_MODE;
	createdCursors = FALSE;

	
    setSize( SbVec2s(500, 390) );  // default size
    setClassName(thisClassName);
    
    // axis of rotation feedback vars
    feedbackFlag = FALSE;
    feedbackRoot = NULL;
    feedbackSwitch = NULL;
    feedbackSize = 20.0;

    // init animation variables
    animationEnabled = TRUE;
    animatingFlag = FALSE;
    rotBuffer = new SbRotation[ROT_BUFF_SIZE];
    lastMotionTime = QDateTime::currentDateTime().addSecs (-1);
    animationSensor = new
        SoFieldSensor(SoQtExaminerViewer::animationSensorCB, this);
#ifdef DEBUG
    if (SoDebug::GetEnv("IV_DEBUG_SENSORS")) {
        SoDebug::NamePtr("examinerSpinSensor", animationSensor);
    }
#endif
    
    // init the projector class
    SbViewVolume vv;
    vv.ortho(-1, 1, -1, 1, -10, 10);
    sphereSheet = new SbSphereSheetProjector;
    sphereSheet->setViewVolume( vv );
    sphereSheet->setSphere( SbSphere( SbVec3f(0, 0, 0), .7f) );
  


    
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

SoQtExaminerViewer::~SoQtExaminerViewer()
//
////////////////////////////////////////////////////////////////////////
{
    delete animationSensor;

    delete sphereSheet;
    if (feedbackRoot)
	feedbackRoot->unref();

    delete [] rotBuffer;
}

void
SoQtExaminerViewer::setViewingMode( ViewingMode viewingMode )
{
    switch(viewingMode) {
        case VIEWING_MODE_SPIN: switchMode(SPIN_MODE_ACTIVE); break;
        case VIEWING_MODE_PAN:  switchMode(PAN_MODE_ACTIVE); break;
        case VIEWING_MODE_ZOOM: switchMode(DOLLY_MODE_ACTIVE); break;
    }
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//	Shows/hide the point of rotation feedback geometry
//
// Use: public
void
SoQtExaminerViewer::setFeedbackVisibility(SbBool insertFlag)
//
////////////////////////////////////////////////////////////////////////
{
    // check for trivial return
    if (camera == NULL || feedbackFlag == insertFlag) {
	feedbackFlag = insertFlag;
	return;
    }

    //
    // find the camera parent to insert/remove the feedback root
    //
    SoSearchAction sa;
    if (insertFlag)
	sa.setNode(camera);
    else {
	sa.setNode(feedbackRoot);
	sa.setSearchingAll(TRUE); // find under OFF switches for removal
    }
    sa.apply(sceneRoot);
    SoFullPath *fullPath = (SoFullPath *) sa.getPath();
    if (!fullPath) {
#if DEBUG
	SoDebugError::post("SoQtExaminerViewer::setFeedbackVisibility",
			    insertFlag ? "ERROR: cannot find camera in graph" :
			    "ERROR: cannot find axis feedback in graph");
#endif
	return;
    }
    SoGroup *parent = (SoGroup *) fullPath->getNodeFromTail(1);
    
    feedbackFlag = insertFlag;
    
    // make sure the feedback has been built
    if (!feedbackRoot)
	createFeedbackNodes();
    
    //
    // inserts/remove the feedback axis group
    //
    
    if (feedbackFlag) {
	int camIndex;
	
	// check to make sure that the camera parent is not a switch node
	// (VRML camera viewpoints are kept under a switch node). Otherwise
	// we will insert the feedback after the switch node.
	if (parent->isOfType(SoSwitch::getClassTypeId())) {
	    SoNode *switchNode = parent;
	    parent = (SoGroup *) fullPath->getNodeFromTail(2);
	    camIndex = parent->findChild(switchNode);
	}
	else
	    camIndex = parent->findChild(camera);
	
	// return if feedback is already there (this should be an error !)
	if (parent->findChild(feedbackRoot) >= 0)
	    return;
	
	// insert the feedback right after the camera+headlight (index+2)
	if (camIndex >= 0) {
	    if (isHeadlight())
		parent->insertChild(feedbackRoot, camIndex+2);
	    else
		parent->insertChild(feedbackRoot, camIndex+1);
	}
	
	// make sure the feedback switch is turned to the correct state now
	// that the feedback root has been inserted in the scene
	feedbackSwitch->whichChild.setValue(viewingFlag ? SO_SWITCH_ALL : SO_SWITCH_NONE);
    }
    else {
	if (parent->findChild(feedbackRoot) >= 0)
	    parent->removeChild(feedbackRoot);
    }


    if (prefSheetShellWidget) {
        prefSheetShellWidget->setFeedbackVisibility(feedbackFlag);
    }
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//	Sets the feedback size.
//
// Use: public
void
SoQtExaminerViewer::setFeedbackSize (int newSize)
//
////////////////////////////////////////////////////////////////////////
{
    if (feedbackSize == newSize)
	return;
    
    // assign new value and redraw (since it is not a field in the scene)
    feedbackSize = newSize;
    if (isFeedbackVisible() && isViewing())
	scheduleRedraw();
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//	Remove the extra geometry when doing a viewAll.
//
// Use: virtual public
void
SoQtExaminerViewer::viewAll()
//
////////////////////////////////////////////////////////////////////////
{
    // stop spinning
    if ( isAnimating() )
    	stopAnimating();
    
    // temporarily remove the feedback geometry
    if (feedbackFlag && isViewing() && feedbackSwitch)
	feedbackSwitch->whichChild.setValue( SO_SWITCH_NONE );
    
    // call the base class
    SoQtFullViewer::viewAll();
    
    // now add the geometry back in
    if (feedbackFlag && isViewing() && feedbackSwitch)
	feedbackSwitch->whichChild.setValue( SO_SWITCH_ALL );
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//	Call the parent class and stop animation if any
//
// Use: virtual public
void
SoQtExaminerViewer::resetToHomePosition()
//
////////////////////////////////////////////////////////////////////////
{
    // stop spinning
    if ( isAnimating() )
    	stopAnimating();
    
    // call the base class
    SoQtFullViewer::resetToHomePosition();
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//	Call the parent class and insert/remove the feedback root
//
// Use: virtual public
void
SoQtExaminerViewer::setCamera(SoCamera *newCamera)
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
	    	setRightWheelString(rl.zoom);
        } 
		else {
            cameraAction->setIcon (SoQtIcon::getIcon (SoQtIcon::PERSP));
	    	setRightWheelString(rl.dolly);
        }
    }

    // detach feedback which depends on camera
    if ( feedbackFlag ) {
	setFeedbackVisibility(FALSE);
	feedbackFlag = TRUE;  // can later be turned on
    }
    
    // call parent class
    SoQtFullViewer::setCamera(newCamera);
    
    // attach feedback back on
    if ( feedbackFlag ) {
	feedbackFlag = FALSE; // enables routine to be called
	setFeedbackVisibility(TRUE);
    }
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//	Call the base class and sets the correct cursors on the window,
//  plus insert/remove the feedback geometry.
//
// Use: virtual public
void
SoQtExaminerViewer::setViewing(SbBool flag)
//
////////////////////////////////////////////////////////////////////////
{
    if (flag == viewingFlag)
	return;
    
    // call the parent class
    SoQtFullViewer::setViewing(flag);
    
    switchMode(isViewing() ? VIEW_MODE : PICK_MODE);
    
    // show/hide the feedback geometry based on the viewing state
    if (feedbackFlag && feedbackSwitch)
	feedbackSwitch->whichChild.setValue(viewingFlag ? SO_SWITCH_ALL : SO_SWITCH_NONE);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//	Enables/Disable the viewer cursor on the window.
//
// Use: virtual public
void
SoQtExaminerViewer::setCursorEnabled(SbBool flag)
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
//    Process the given event to do viewing stuff
//
// Use: virtual protected
void
SoQtExaminerViewer::processEvent (QEvent *qe)
//
////////////////////////////////////////////////////////////////////////
{
    if ( processCommonEvents(qe) )
        return;
    
    if (!createdCursors)
        updateCursor();
    
    QMouseEvent    *be;
    QMouseEvent    *me;
    QWheelEvent    *we;
    QKeyEvent      *ke;
    
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
            else { //... ButtonRelease

                // check if we need to start spinning
                if (mode == SPIN_MODE_ACTIVE && animationEnabled
                        && lastMotionTime == QDateTime::currentDateTime()) {
                    animatingFlag = TRUE;
                    computeAverage = TRUE;
                    animationSensor->attach(viewerRealTime);
                    interactiveCountInc();
                }

                interactiveCountDec();
            }
            updateViewerMode(be->modifiers(), be->buttons());
        }
        if (qe->type() == QEvent::MouseButtonPress)
            stopAnimating();
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
        case SPIN_MODE_ACTIVE:
            lastMotionTime = QDateTime::currentDateTime();
            spinCamera(SbVec2f(me->x()/float(raSize[0]), (raSize[1] - me->y())/float(raSize[1])));
            break;
        case PAN_MODE_ACTIVE:
            panCamera(SbVec2f(me->x()/float(raSize[0]), (raSize[1] - me->y())/float(raSize[1])));
            break;
        case DOLLY_MODE_ACTIVE:
            dollyCamera( SbVec2s(me->x(), raSize[1] - me->y()) );
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
    case QEvent::Wheel:
        we = (QWheelEvent *)qe;
        dollyCamera(we->delta()/1000.0f);
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
SoQtExaminerViewer::updateViewerMode (int modifiers, int buttons)
//
////////////////////////////////////////////////////////////////////////
{
    if (modifiers == -1) {
        modifiers = QApplication::keyboardModifiers();
    }
    if (buttons == -1) {
        buttons = QApplication::mouseButtons();
    }
    // LEFT+MIDDLE down
    if (buttons & Qt::LeftButton && buttons & Qt::MidButton) {
	switchMode(DOLLY_MODE_ACTIVE);
    }
    
    // LEFT down
    else if (buttons & Qt::LeftButton) {
	if (modifiers & Qt::ControlModifier)
	    switchMode(PAN_MODE_ACTIVE);
	else
	    switchMode(SPIN_MODE_ACTIVE);
    }
    
    // MIDDLE DOWN
    else if (buttons & Qt::MidButton) {
	if (modifiers & Qt::ControlModifier)
	    switchMode(DOLLY_MODE_ACTIVE);
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
SoQtExaminerViewer::switchMode(int newMode)
//
////////////////////////////////////////////////////////////////////////
{

    SbVec2s raSize = getGlxSize();
    
    // assing new mode
    int prevMode = mode;
    mode = newMode;
    
    // update the cursor
    updateCursor();
    
    // switch to new viewer mode
    switch (newMode) {
        case PICK_MODE:
		// ???? is if are going into PICK mode and some of our
		// mouse buttons are still down, make sure to decrement
		// interactive count correctly (correct draw style). One
		// for the LEFT and one for MIDDLE mouse.
                    if (QApplication::mouseButtons() & Qt::LeftButton && prevMode != SEEK_MODE)
					 interactiveCountDec(); 
                    if (QApplication::mouseButtons() & Qt::MidButton && prevMode != SEEK_MODE)
					 interactiveCountDec();
	    stopAnimating();
	    break;
	    
	case SPIN_MODE_ACTIVE:
	    // set the sphere sheet starting point
	    sphereSheet->project(
		SbVec2f(locator[0]/float(raSize[0]), locator[1]/float(raSize[1])) );
	    
	    // reset the animation queue
	    firstIndex = 0;
	    lastIndex = -1;
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
	    SbViewVolume    cameraVolume;
	    SbLine	    line;
	    cameraVolume = camera->getViewVolume(raSize[0]/float(raSize[1]));
	    cameraVolume.projectPointToLine(
		SbVec2f(locator[0]/float(raSize[0]), locator[1]/float(raSize[1])), line);
	    focalplane.intersect(line, locator3D);
	    }
	    break;
    }
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    updates the viewer cursor
//
// Use: private
void
SoQtExaminerViewer::updateCursor()
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
	    setCursor(Qt::ArrowCursor);
	    break;
	    
	case VIEW_MODE:
	case SPIN_MODE_ACTIVE:
	    setCursor(spinCursor);
	    break;
	    
	case DOLLY_MODE_ACTIVE:
	    setCursor(dollyCursor);
	    break;
	    
	case PAN_MODE:
	case PAN_MODE_ACTIVE:
	    setCursor(panCursor);
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
SoQtExaminerViewer::actualRedraw()
//
////////////////////////////////////////////////////////////////////////
{
    // place the feedback at the focal point
    // ??? we really only need to do this when the camera changes
    if (isViewing() && feedbackFlag && camera != NULL && feedbackRoot) {
	
	// adjust the position to be at the focal point
	SbMatrix mx;
	mx = camera->orientation.getValue();
	SbVec3f forward(-mx[2][0], -mx[2][1], -mx[2][2]);
	feedbackTransNode->translation = camera->position.getValue() + 
	    camera->focalDistance.getValue() * forward;
	
	// adjust the size to be constant on the screen
	float height = 1.0f;
	if (camera->isOfType(SoPerspectiveCamera::getClassTypeId())) {
	    float angle = ((SoPerspectiveCamera *)camera)->heightAngle.getValue();
	    height = camera->focalDistance.getValue() * tanf(angle/2);
	}
	else if (camera->isOfType(SoOrthographicCamera::getClassTypeId()))
	    height = ((SoOrthographicCamera *)camera)->height.getValue() / 2;
	
	// ??? getGlxSize[1] == 0 the very first time, so return in that case
	// ??? else the redraws are 3 times slower from now on !! (alain)
	if (getGlxSize()[1] != 0) {
	    float size = 2.0 * height * feedbackSize / float (getGlxSize()[1]);
	    feedbackScaleNode->scaleFactor.setValue(size, size, size);
	}
    }
    
    // have the base class draw the scene
    SoQtFullViewer::actualRedraw();
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Enable/disable the animation feature of the Examiner
//
// Use: public
void
SoQtExaminerViewer::setAnimationEnabled(SbBool flag)
//
////////////////////////////////////////////////////////////////////////
{
    if (animationEnabled == flag)
	return;
    
    animationEnabled = flag;
    if ( !animationEnabled && isAnimating())
        stopAnimating();
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Stops ongoing animation (if any)
//
// Use: public
void
SoQtExaminerViewer::stopAnimating()
//
////////////////////////////////////////////////////////////////////////
{
    if (animatingFlag) {
	animatingFlag = FALSE;
	animationSensor->detach();
	animationSensor->unschedule();
	interactiveCountDec();
    }
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//	Call the base class and sets the correct cursors on the window
//
// Use: virtual protected
void
SoQtExaminerViewer::setSeekMode(SbBool flag)
//
////////////////////////////////////////////////////////////////////////
{
    if ( !isViewing() )
	return;
    
    // stop spinning
    if (isAnimating())
    	stopAnimating();
    
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
SoQtExaminerViewer::createPrefSheet(QTabWidget* parent)
//
////////////////////////////////////////////////////////////////////////
{
    // create the preference sheet shell and form widget
    SoQtFullViewer::createPrefSheet(parent);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//	Brings the viewer help card (called by "?" push button)
//
// Use: virtual protected
void
SoQtExaminerViewer::openViewerHelpCard()
//
////////////////////////////////////////////////////////////////////////
{
    // tell the base class to open the file for us
    openHelpCard("SoQtExaminerViewer.help");
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Call the base class and stop animating
//
// Use: virtual protected

void
SoQtExaminerViewer::bottomWheelStart()
//
////////////////////////////////////////////////////////////////////////
{
    SoQtFullViewer::bottomWheelStart();
    stopAnimating();
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Call the base class and stop animating
//
// Use: virtual protected

void
SoQtExaminerViewer::leftWheelStart()
//
////////////////////////////////////////////////////////////////////////
{
    SoQtFullViewer::bottomWheelStart();
    stopAnimating();
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Rotates the object around the screen x axis (called by thumb wheel).
//
// Use: virtual protected

void
SoQtExaminerViewer::bottomWheelMotion(float newVal)
//
////////////////////////////////////////////////////////////////////////
{
    // get rotation and apply to camera
    SbVec3f axis(0, 1, 0);
    SbRotation rot(axis, bottomWheelVal - newVal);
    rotateCamera(rot);
    
    bottomWheelVal = newVal;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Rotates the object around the screen y axis (called by thumb wheel).
//
// Use: virtual protected

void
SoQtExaminerViewer::leftWheelMotion(float newVal)
//
////////////////////////////////////////////////////////////////////////
{
    // get rotation and apply to camera
    SbVec3f axis(1, 0, 0);
    SbRotation rot(axis, newVal - leftWheelVal);
    rotateCamera(rot);
    
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
SoQtExaminerViewer::rightWheelMotion(float newVal)
//
////////////////////////////////////////////////////////////////////////
{
    if (camera == NULL)
        return;
    
    dollyCamera(newVal - rightWheelVal);

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
SoQtExaminerViewer::defineCursors()
//
////////////////////////////////////////////////////////////////////////
{
	// spin cursor
    spinCursor = SoQtCursor::getCursor(SoQtCursor::CURVED_HAND);
	
	// panning cursor
    panCursor = SoQtCursor::getCursor(SoQtCursor::FLAT_HAND);
    
    // dolly cursor
    dollyCursor = SoQtCursor::getCursor(SoQtCursor::POINTING_HAND);
    
    // seek cursor
    seekCursor = SoQtCursor::getCursor(SoQtCursor::TARGET);
    
    createdCursors = TRUE;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Rotates the camera around pt of interest by given rotation
//
// Use: private

void
SoQtExaminerViewer::rotateCamera(const SbRotation &rot)
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
    
    // apply new rotation to the camera
    camRot = rot * camRot;
    camera->orientation = camRot;
    
    // reposition camera to look at pt of interest
    mx = camRot;
    forward.setValue( -mx[2][0], -mx[2][1], -mx[2][2]);
    camera->position = center - radius * forward;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Moves the camera into the plane defined by the camera forward vector
//  and the focal point to follow the new mouse location.
//
// Use: private

void
SoQtExaminerViewer::panCamera(const SbVec2f &newLocator)
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
//    rotates the camera using the sheet sphere projector, given the new
//  mouse location.
//
// Use: private

void
SoQtExaminerViewer::spinCamera(const SbVec2f &newLocator)
//
////////////////////////////////////////////////////////////////////////
{
    // find rotation and rotate camera
    SbRotation rot;
    sphereSheet->projectAndGetRotation(newLocator, rot);
    rot.invert();

    rotateCamera(rot);
    
    // save rotation for animation
    lastIndex = ((lastIndex+1) % ROT_BUFF_SIZE);
    rotBuffer[lastIndex] = rot;
    
    // check if queue is full
    if (((lastIndex+1) % ROT_BUFF_SIZE) == firstIndex)
	firstIndex = ((firstIndex+1) % ROT_BUFF_SIZE);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Moves the camera forward/backward based on the new mouse position.
//  (perspective camera), else change the camera height (orthographic
//  camera case).
//
// Use: private

void
SoQtExaminerViewer::dollyCamera(const SbVec2s &newLocator)
//
////////////////////////////////////////////////////////////////////////
{
    if (camera == NULL)
	return;
    
    // moving the mouse up/down will move the camera futher/closer.
    // moving the camera sideway will not move the camera at all
    dollyCamera((newLocator[1] - locator[1]) / 40.0);
    
    locator = newLocator;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Moves the camera forward/backward (perspective camera),
//  else change the camera height (orthographic camera case).
//
// Use: private

void
SoQtExaminerViewer::dollyCamera(float amount)
//
////////////////////////////////////////////////////////////////////////
{
    if (camera == NULL)
        return;

    if (camera->isOfType(SoOrthographicCamera::getClassTypeId())) {
        // change the ortho camera height
        SoOrthographicCamera *cam = (SoOrthographicCamera *) camera;
        cam->height = cam->height.getValue() * powf(2.0, amount);
    }
    else {
        // shorter/grow the focal distance given the mouse move
        float focalDistance = camera->focalDistance.getValue();;
        float newFocalDist = focalDistance * powf(2.0, amount);

        // finally reposition the camera
        SbMatrix mx;
        mx = camera->orientation.getValue();
        SbVec3f forward(-mx[2][0], -mx[2][1], -mx[2][2]);
        camera->position = camera->position.getValue() +
                (focalDistance - newFocalDist) * forward;
        camera->focalDistance = newFocalDist;
    }
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Routine which animates the ball spinning (called by sensor).
//
// Use: private

void
SoQtExaminerViewer::doSpinAnimation()
//
////////////////////////////////////////////////////////////////////////
{
    //
    // check if average rotation needs to be computed
    //
    
    if (computeAverage) {
	float averageAngle, angle;
	SbVec3f averageAxis, axis;
	
	// get number of samples
	int num = (((lastIndex - firstIndex) + 1 + 
	    ROT_BUFF_SIZE) % ROT_BUFF_SIZE);
	
	// check for not enough samples
	if (num < 2) {
	    stopAnimating();
	    return;
	}
	
	// get average axis of rotation
	// ??? right now only take one sample
	rotBuffer[firstIndex].getValue(averageAxis, angle);
	
	// get average angle of rotation
	averageAngle = 0;
	for (int i=0; i<num; i++) {
	    int n = (firstIndex + i) % ROT_BUFF_SIZE;
	    rotBuffer[n].getValue(axis, angle);
	    averageAngle += angle;
	}
	averageAngle /= float(num);
	
	averageRotation.setValue(averageAxis, averageAngle);
	computeAverage = FALSE;
    }
    
    //
    // rotate camera by average rotation
    //
    rotateCamera(averageRotation);
}


////////////////////////////////////////////////////////////////////////
//
// Description:
//	Build the parent class widget, then register this widget.
//
// Use: protected
QWidget*
SoQtExaminerViewer::buildWidget(QWidget* parent)
//
////////////////////////////////////////////////////////////////////////
{
    // Get a resource value for menu title.
	setPopupMenuString(rl.examinViewer);

    // Create the root widget and register it with a class name
    QWidget* w = SoQtFullViewer::buildWidget(parent);
    
	// Full viewer registered the widget for us
    //setAnimationEnabled(flag);
    //setFeedbackVisibility(flag);
    //feedbackSize = val;
        //
    rl.roty = defaultLabel[1];
    rl.rotx = defaultLabel[2];
    rl.preferenceSheet = defaultLabel[3];
    rl.zoom = defaultLabel[4];
    rl.dolly = defaultLabel[5];
    rl.axesSizeLabel = defaultLabel[6];
	
	// assign decoration names
	setBottomWheelString(rl.roty);
	setLeftWheelString(rl.rotx);
	setPrefSheetString(rl.preferenceSheet);
    
    return w;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//	add our own button to the existing list
//
// Use: virtual protected
void
SoQtExaminerViewer::createViewerButtons(QToolBar* parent)
//
////////////////////////////////////////////////////////////////////////
{
    // create the default buttons
    SoQtFullViewer::createViewerButtons(parent);
        cameraAction = new QAction(SoQtIcon::getIcon(SoQtIcon::PERSP), tr("Perspective"), this);
	cameraAction->setCheckable(true);
	connect (cameraAction, SIGNAL (triggered(bool)), SLOT (camPushCB()));

    // add this button to the list...
    parent->addAction (cameraAction);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//	read the point of interest geometry, which will be placed
//  right after the camera node (or right after the headlight
//  if the headlight is turned on).
//
// Use: private
void
SoQtExaminerViewer::createFeedbackNodes()
//
////////////////////////////////////////////////////////////////////////
{
    // make sure we havn't built this yet...
    if (feedbackRoot)
	return;
    
    feedbackRoot	= new SoSeparator(1);
    feedbackSwitch	= new SoSwitch(3);
    feedbackTransNode	= new SoTranslation;
    feedbackScaleNode	= new SoScale;
    feedbackRoot->ref();
    feedbackRoot->addChild( feedbackSwitch );
    feedbackSwitch->addChild( feedbackTransNode );
    feedbackSwitch->addChild( feedbackScaleNode );
    SoInput in;
    in.setBuffer((void *)geometryBuffer, (size_t) strlen(geometryBuffer));
    SoNode *node;
    SbBool ok = SoDB::read(&in, node);
    if (ok && node != NULL)
	feedbackSwitch->addChild(node);
#ifdef DEBUG
    else
	SoDebugError::post("SoQtExaminerViewer::createFeedbackNodes",
			    "couldn't read feedback axis geometry");
#endif
}

//
// redefine those generic virtual functions
//
const char *
SoQtExaminerViewer::getDefaultWidgetName() const
{ return thisClassName; }

const char *
SoQtExaminerViewer::getDefaultTitle() const
{ return rl.examinViewer; }

const char *
SoQtExaminerViewer::getDefaultIconTitle() const
{ return rl.examinViewer; }


//
////////////////////////////////////////////////////////////////////////
// static callbacks stubs
////////////////////////////////////////////////////////////////////////
//

void
SoQtExaminerViewer::camPushCB()
{ toggleCameraType(); }


void
SoQtExaminerViewer::animationSensorCB (void *v, SoSensor *)
{
    ((SoQtExaminerViewer *) v)->doSpinAnimation();
}

// called when the viewer becomes visible/hidden - when hidden, make
// sure to temporary stop any ongoing animation (and restart it as soon
// as we become visible).
//
void
SoQtExaminerViewer::visibilityChanged (bool visible)
{
	SoQtFullViewer::visibilityChanged(visible);

    // only do this if we are/were spinning....
    if (! animatingFlag)
	return;
    
    if (visible) {
	// we now are visible again so attach the field sensor
	animationSensor->attach(viewerRealTime);
    }
    else {
	// if hidden, detach the field sensor, but don't change the
	// animatingFlag var to let us know we need to turn it back on
	// when we become visible....
	animationSensor->detach();
	animationSensor->unschedule();
    }
}

