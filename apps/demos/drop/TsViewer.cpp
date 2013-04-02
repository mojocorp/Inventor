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
 _______________________________________________________________________
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 |
 |   $Revision: 1.1 $
 |
 |   Classes	: TsViewer
 |
 |   Author(s)	: Dave Immel
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

#include <QMouseEvent>

#include <math.h>

#include <Inventor/nodes/SoOrthographicCamera.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/Qt/SoQtCursors.h>
#include "TsViewer.h"


enum ViewerModes {
    IDLE_MODE, 
    TRANS_MODE, 
    SEEK_MODE, 
    ROT_MODE,
};

#define TS_XROT_FACTOR 3.14
#define TS_YROT_FACTOR 3.14
#define TS_YTRANS_FACTOR 16.0

////////////////////////////////////////////////////////////////////////
//
// Public constructor - build the widget right now
//
TsViewer::TsViewer(
    QWidget *parent,
    const char *name)
        : SoQtViewer(
            parent,
            name, 
            TRUE, 
            SoQtViewer::BROWSER,
            TRUE)  
//
////////////////////////////////////////////////////////////////////////
{
    // init local vars
    mode = IDLE_MODE;
    createdCursors = FALSE;
}


////////////////////////////////////////////////////////////////////////
//
// Destructor
//
TsViewer::~TsViewer()
//
////////////////////////////////////////////////////////////////////////
{}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Process the given event to change the camera
//
// Use: protected

void
TsViewer::processEvent(QEvent *xe)

//
////////////////////////////////////////////////////////////////////////
{
    // check if cursors need to be defined (they can only
    // be defined after the window has been mapped. 
    // Recieving events guarantees that the window has 
    // been mapped.
    if (!createdCursors) {
        defineCursors();
        setCursor(vwrCursor);
    }
    
    QMouseEvent    *be;
    QMouseEvent    *me;
    QKeyEvent      *ke;

    switch(xe->type()) {
        case QEvent::KeyPress:
            ke = (QKeyEvent *)xe;
            switch (ke->key()) {
                case Qt::Key_Home:
                    resetToHomePosition();
                    return;
            }
            break;
        case QEvent::MouseButtonPress:
        be = (QMouseEvent *)xe;
        locator[0] = windowSize[0] - be->x();
        locator[1] = windowSize[1] - be->y();
        if (be->button() == Qt::LeftButton) {
		switch (mode) {
		    case IDLE_MODE: 
			interactiveCountInc();
			break;
		}
                switchMode(ROT_MODE);
	    }
        else if (be->button() == Qt::RightButton) {
		switch (mode) {
		    case IDLE_MODE: 
			interactiveCountInc();
			break;
		}
                switchMode(TRANS_MODE);
            }
            return;
	    
    case QEvent::MouseButtonRelease:
        be = (QMouseEvent *)xe;
        if (be->button() == Qt::LeftButton) {
                if (mode == ROT_MODE)
                {
  		    switchMode(IDLE_MODE);
		    interactiveCountDec();
                }
                else
  		    switchMode(TRANS_MODE);
	    }
        else if (be->button() == Qt::RightButton) {
                if (mode == TRANS_MODE)
                {
  		    switchMode(IDLE_MODE);
		    interactiveCountDec();
                }
                else
  		    switchMode(ROT_MODE);
            }
            return;
	    
    case QEvent::MouseMove:
            me = (QMouseEvent *)xe;
            movement[0] = locator[0];
            movement[1] = locator[1];
            locator[0] = windowSize[0] - me->x();
            locator[1] = windowSize[1] - me->y();
	    if (mode == TRANS_MODE)
		translateCamera();
	    else if (mode == ROT_MODE)
            {
		rotateCamera();
            }
            return;
    }
    // Pass all other events to the render area so they will be given
    // to the scene graph.
    SoQtRenderArea::processEvent(xe);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Switches to the specified viewer mode. The correct
//    cursor is also set on the window.
//
// Use: private

void
TsViewer::switchMode(int newMode)

//
////////////////////////////////////////////////////////////////////////
{
    // needed to define new cursors
    if (!createdCursors)
	defineCursors();

    // switch to new viewer mode
    mode = newMode;
    switch (mode) {
	case IDLE_MODE:
        setCursor(vwrCursor);
	    break;
	case TRANS_MODE:
	    {
		// Figure out the focal plane
		SbMatrix mx;
		mx = camera->orientation.getValue();
		SbVec3f forward(-mx[2][0], -mx[2][1], -mx[2][2]);
		SbVec3f fp = camera->position.getValue() + 
		    forward * camera->focalDistance.getValue();
		focalplane = SbPlane(forward, fp);
		
		// map mouse position onto the viewing plane
		SbViewVolume cameraVolume;
		SbLine line;
		cameraVolume = camera->getViewVolume();
		cameraVolume.projectPointToLine(
		    SbVec2f( locator[0] / float(windowSize[0]), 
			locator[1] / float(windowSize[1])), line);
		focalplane.intersect(line, locator3D);
	    }
        setCursor(vwrCursor);
	    break;
    }
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Called by the base class (SoXtGLWidget) when the 
//    render Area widget changes size. Redefine it here to 
//    cache the RA window size.
//
// Use: protected

void
TsViewer::sizeChanged(const SbVec2s &newSize)

//
////////////////////////////////////////////////////////////////////////
{
    // cache the renderArea size
    windowSize = newSize;
    
    // call parent class
    SoQtViewer::sizeChanged(newSize);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    This routine is used to define cursors, which can 
//    only be called after the window has been realized.
//
// Use: private

void
TsViewer::defineCursors()

//
////////////////////////////////////////////////////////////////////////
{
    vwrCursor = SoQtCursor::getCursor(SoQtCursor::CURVED_HAND);
    seekCursor = SoQtCursor::getCursor(SoQtCursor::TARGET);

    createdCursors = TRUE;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Moves the camera along the vertical Y axis as the user moves
//    the mouse up and down.
//
// Use: private

void
TsViewer::translateCamera()

//
////////////////////////////////////////////////////////////////////////
{
    if (camera == NULL)
	return;
    
    SbVec2f delta2D( (movement[0]-locator[0]) / float(windowSize[0]), 
                     (movement[1]-locator[1]) / float(windowSize[1]) );

    // map vertical mouse motion into a Y translation in world space.
    SbVec3f  delta3D;
    delta3D[0] = 0.0;
    delta3D[1] = (delta2D[0] + delta2D[1]) * TS_YTRANS_FACTOR;
    delta3D[2] = 0.0;

    camera->position = camera->position.getValue() + delta3D;
}


////////////////////////////////////////////////////////////////////////
//
// Description:
//    Revolves the camera around its focal point
//    to follow the new mouse location.  Rotations are about
//    the X and Y axes only.
//
// Use: private

void
TsViewer::rotateCamera()
//
////////////////////////////////////////////////////////////////////////
{
    if (camera == NULL)
        return;

    SbVec2f newLocator( (movement[0]-locator[0]) / float(windowSize[0]), 
	(movement[1]-locator[1]) / float(windowSize[1]) );
    
    // figure out the new rotation.
    SbRotation xRot( SbVec3f(-1.0, 0.0, 0.0), newLocator[1] * TS_XROT_FACTOR );
    SbRotation yRot( SbVec3f(0.0, -1.0, 0.0), newLocator[0] * TS_YROT_FACTOR );
    SbRotation rot;

    // get center of rotation
    SbRotation camRot = camera->orientation.getValue();
    float radius = camera->focalDistance.getValue();
    SbMatrix mx;
    mx = camRot;
    SbVec3f forward( -mx[2][0], -mx[2][1], -mx[2][2]);
    SbVec3f center = camera->position.getValue()
        + radius * forward;

    // apply new rotation to the camera
    camRot = xRot * camRot * yRot;
    camera->orientation = camRot;

    // reposition camera to look at pt of interest
    mx = camRot;
    forward.setValue( -mx[2][0], -mx[2][1], -mx[2][2]);
    camera->position = center - radius * forward;

    movement[0] = locator[0];
    movement[1] = locator[1];
}
