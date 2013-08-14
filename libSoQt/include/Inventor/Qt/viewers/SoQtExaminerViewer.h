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

#ifndef  _SO_QT_EXAMINER_VIEWER_
#define  _SO_QT_EXAMINER_VIEWER_

#include <Inventor/Qt/SoQt.h>
#include <Inventor/Qt/viewers/SoQtFullViewer.h>
#include <Inventor/SbLinear.h>

#include <QCursor>
#include <QDateTime>

// classes used
class SbSphereSheetProjector;
class SoFieldSensor;
class SoSeparator;
class SoSwitch;
class SoTranslation;
class SoScale;
class SoQtThumbWheel;
class QSpinBox;
//////////////////////////////////////////////////////////////////////////////
//
//  Class: SoQtExaminerViewer
//
//	Examiner viewer - allows the user to change the camera position
//  by spinning a sphere in front of the viewpoint.
//
//
//	Keys used by this viewer:
//	-------------------------
//
//	Left Mouse: Tumbles the virtual trackball.
//
//	Middle Mouse: 
//	Ctrl + Left Mouse: Translate up, down, left and right.
//
//	Ctrl + Middle Mouse: 
//	Left + Middle Mouse: Dolly in and out (gets closer to and further
//			    away from the object).
//
//	<s> + click: Alternative to the Seek button. Press (but do not hold 
//		    down) the <s> key, then click on a target object.
//
//	Right Mouse: Open the popup menu.
//
//////////////////////////////////////////////////////////////////////////////


class SOQT_EXPORT SoQtExaminerViewer : public SoQtFullViewer {
    Q_OBJECT
public:
    SoQtExaminerViewer(
            QWidget* parent = NULL,
            const char *name = NULL,
            SbBool buildInsideParent = TRUE,
            SoQtFullViewer::BuildFlag flag = BUILD_ALL,
            SoQtViewer::Type type = BROWSER);

    ~SoQtExaminerViewer();
    
    enum ViewingMode {
        VIEWING_MODE_SPIN,
        VIEWING_MODE_PAN,
        VIEWING_MODE_ZOOM
    };

    void setViewingMode( ViewingMode viewingMode );
    ViewingMode getViewingMode() const { return viewingMode; }

    //
    // Show/hide the point of rotation feedback, which only appears while
    // in Viewing mode. (default OFF)
    //
    void        setFeedbackVisibility(SbBool onOrOff);
    SbBool      isFeedbackVisible() const { return feedbackFlag; }
    
    //
    // Set/get the point of rotation feeedback size in pixels (default 20).
    //
    void        setFeedbackSize(int newSize);
    int         getFeedbackSize() const { return (int) feedbackSize; }
    
    //
    // Enable/disable the animation feature of the viewer.
    // (enabled by default)
    //
    void        setAnimationEnabled(SbBool onOrOff);
    SbBool      isAnimationEnabled() const { return animationEnabled; }
    
    //
    // Stop animation, if it is occurring, and queuery if the viewer is
    // currently animating.
    //
    void        stopAnimating();
    SbBool      isAnimating() { return animatingFlag; }
    
    //
    // redefine these to add Examiner viewer functionality
    //
    virtual void        viewAll();
    virtual void        resetToHomePosition();
    virtual void        setCamera(SoCamera *cam);
    virtual void        setViewing(SbBool onOrOff);
    virtual void        setCursorEnabled(SbBool onOrOff);
    virtual void        setSeekMode(SbBool onOrOff);
    
protected:

    // This constructor takes a boolean whether to build the widget now.
    // Subclasses can pass FALSE, then call SoQtExaminerViewer::buildWidget()
    // when they are ready for it to be built.
    SoEXTENDER
    SoQtExaminerViewer(
            QWidget *parent,
            const char *name,
            SbBool buildInsideParent,
            SoQtFullViewer::BuildFlag flag,
            SoQtViewer::Type type,
            SbBool buildNow);

    // redefine these
    virtual const char *    getDefaultWidgetName() const;
    virtual const char *    getDefaultTitle() const;
    virtual const char *    getDefaultIconTitle() const;
    
    // redefine those routines to do viewer specific stuff
    virtual void        processEvent (QEvent *anyevent);
    virtual void        actualRedraw();
    
    // Get Qt resources for the widget.
    QWidget*            buildWidget(QWidget* parent);
    
    // Define those thumb wheels to rotate the object
    virtual void        bottomWheelMotion(float newVal);
    virtual void        leftWheelMotion(float newVal);
    virtual void        rightWheelMotion(float newVal);
    
    // redefine those routines to also stop animation if any
    virtual void        bottomWheelStart();
    virtual void        leftWheelStart();
    
    // add viewer preference stuff
    virtual void createPrefSheet(QTabWidget* parent);
    
    // add some viewer buttons
    virtual void        createViewerButtons (QToolBar* parent);

    // Define this to bring the viewer help card
    virtual void        openViewerHelpCard();

    virtual void visibilityChanged (bool flag);

protected:
    // viewer state variables
    int             mode;
    SbBool          createdCursors;
    QCursor         spinCursor, panCursor, dollyCursor, seekCursor;
    SbSphereSheetProjector *sphereSheet;
    SbVec2s         locator; // mouse position
    SbBool          firstBuild; // set FALSE after buildWidget called once
    
    ViewingMode viewingMode;

    // point of rotation feedback vars
    SbBool          feedbackFlag;
    float           feedbackSize;
    SoSeparator    *feedbackRoot;
    SoSwitch       *feedbackSwitch;
    SoTranslation  *feedbackTransNode;
    SoScale        *feedbackScaleNode;
    static const char    *geometryBuffer;
    void            createFeedbackNodes();
    
    // variables used for doing spinning animation
    SbBool          animationEnabled, animatingFlag;
    SoFieldSensor  *animationSensor;
    SbRotation     *rotBuffer;
    int             firstIndex, lastIndex;
    QDateTime       lastMotionTime;
    SbRotation      averageRotation;
    SbBool          computeAverage;
    static void	    animationSensorCB(void *v, SoSensor *s);
    void            doSpinAnimation();
    
    // camera panning vars
    SbVec3f         locator3D;
    SbPlane         focalplane;
    
    void            updateViewerMode (int modifiers = -1, int buttons = -1);
    void            switchMode(int newMode);
    void            updateCursor();
    void            defineCursors();
    void            rotateCamera(const SbRotation &rot);
    void            panCamera(const SbVec2f &newLocator);
    void            spinCamera(const SbVec2f &newLocator);
    void            dollyCamera(const SbVec2s &newLocator);
    void            dollyCamera(float amount);

    // push button vars and callbacks
    QAction            *cameraAction;
    Q_SLOT void camPushCB();

    // this is called by both constructors
    void constructorCommon(SbBool buildNow);
};

#endif  /* _SO_QT_EXAMINER_VIEWER_ */
