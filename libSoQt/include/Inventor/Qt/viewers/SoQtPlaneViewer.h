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
 * Copyright (C) 1990-93   Silicon Graphics, Inc.
 *
 _______________________________________________________________________
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 |
 |   $Revision: 1.1 $
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

#ifndef  _SO_QT_PLANE_VIEWER_
#define  _SO_QT_PLANE_VIEWER_

#include <Inventor/Qt/viewers/SoQtFullViewer.h>
#include <Inventor/SbLinear.h>

class QPushButton;


//////////////////////////////////////////////////////////////////////////////
//
//  Class: SoQtPlaneViewer
//
//  Plane Viewer - allows the user to move the camera in a plane, zoom in
//	and out, as well as rotate the viewing plane.
//
//	Keys used by this viewer:
//	-------------------------
//
//     Left Mouse: Dolly in and out (gets closer to and further away from the
//     object).
//
//     Middle Mouse: 
//     Ctrl + Left Mouse: Translate up, down, left and right.
//
//     Ctrl + Middle Mouse: Used for roll action (rotates around the viewer
//     forward direction).
//
//     <s> + click: Alternative to the Seek button. Press (but do not hold down)
//     the <s> key, then click on a target object.
//
//     Right Mouse: Open the popup menu.
//  
//////////////////////////////////////////////////////////////////////////////

class SoQtPlaneViewer : public SoQtFullViewer {
    Q_OBJECT
public:
    SoQtPlaneViewer(
            QWidget *parent = NULL,
            const char *name = NULL,
            SbBool buildInsideParent = TRUE,
            SoQtFullViewer::BuildFlag flag = BUILD_ALL,
            SoQtViewer::Type type = BROWSER);
    
    ~SoQtPlaneViewer();
    
    //
    // redefine these to add Plane viewer functionality
    //
    virtual void        setViewing(SbBool onOrOff);
    virtual void        setCamera(SoCamera *cam);
    virtual void        setCursorEnabled(SbBool onOrOff);
    
protected:

    // This constructor takes a boolean whether to build the widget now.
    // Subclasses can pass FALSE, then call SoQtPlaneViewer::buildWidget()
    // when they are ready for it to be built.
    SoEXTENDER
    SoQtPlaneViewer(
            QWidget *parent,
            const char *name,
            SbBool buildInsideParent,
            SoQtFullViewer::BuildFlag flag,
            SoQtViewer::Type type,
            SbBool buildNow);

    QWidget*            buildWidget(QWidget* parent);
        
    // redefine these
    virtual const char *    getDefaultWidgetName() const;
    virtual const char *    getDefaultTitle() const;
    virtual const char *    getDefaultIconTitle() const;
    
    // redefine those routines to do viewer specific stuff
    virtual void        processEvent(QEvent *anyevent);
    virtual void        setSeekMode(SbBool onOrOff);
    virtual void        actualRedraw();
    
    // Define those thumb wheels to translate in the viewer plane
    virtual void        bottomWheelMotion(float newVal);
    virtual void        leftWheelMotion(float newVal);
    virtual void        rightWheelMotion(float newVal);
    
    // add viewer preference stuff
    virtual void        createPrefSheet();
    
    // add some viewer buttons
    virtual void        createViewerButtons(QToolBar* parent);
    
    // Define this to bring the viewer help card
    virtual void        openViewerHelpCard();
    
    // Redefine this to keep the same camera rotation when seeking
    virtual void        computeSeekFinalOrientation();

private:
    // viewer state variables
    int             mode;
    SbBool          createdCursors;
    QCursor         transCursor, dollyCursor, seekCursor;
    SbVec2s         locator; // mouse position
    
    // camera translation vars
    SbVec3f         locator3D;
    SbPlane         focalplane;
    float           transXspeed, transYspeed;
    
    // push button vars and callbacks
    QAction        *xAction;
    QAction        *yAction;
    QAction        *zAction;
    QAction        *cameraAction;

    void            updateViewerMode(int modifiers, int buttons);
    void            switchMode(int newMode);
    void            updateCursor();
    void            defineCursors();
    void            rollCamera(const SbVec2s &newLocator);
    void            translateCamera(const SbVec2f &newLocator);
    void            dollyCamera(const SbVec2s &newLocator);
    void            setPlane(const SbVec3f &newNormal, const SbVec3f &newRight);
    
    // redefine these to also call computeTranslateValues()
    virtual void    bottomWheelStart();
    virtual void    leftWheelStart();
    void            computeTranslateValues();
    
    // this is called by both constructors
    void            constructorCommon(SbBool buildNow);
private slots:
    void            xButtonClicked();
    void            yButtonClicked();
    void            zButtonClicked();
    void            camButtonClicked();
};

#endif  /* _SO_QT_PLANE_VIEWER_ */
