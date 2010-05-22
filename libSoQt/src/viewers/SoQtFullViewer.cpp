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
 |   $Revision: 1.4 $
 |
 |   Classes    : SoQtFullViewer
 |
 |   Author()	: Alain Dumesny
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

/*
 * Ported to Qt by Morgan Leborgne, 2010
 */

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include <Inventor/SoPickedPoint.h>
#include <Inventor/Qt/viewers/SoQtFullViewer.h>
#include <Inventor/Qt/SoQtIcons.h>
#include <Inventor/nodes/SoOrthographicCamera.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/errors/SoDebugError.h>

#include <QAbstractButton>
#include <QAction>
#include <QActionGroup>
#include <QBoxLayout>
#include <QButtonGroup>
#include <QCheckBox>
#include <QDialog>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QMenu>
#include <QMessageBox>
#include <QPushButton>
#include <QRadioButton>
#include <QSlider>
#include <QTabWidget>
#include <QToolBar>
#include <QContextMenuEvent>
#include <Inventor/Qt/SoQtThumbWheel.h>

static char *thisClassName = "SoQtFullViewer";

////////////////////////////////////////////////////////////////////////
//
//  Constructor.
//
// Use: protected

SoQtFullViewer::SoQtFullViewer(
    QWidget* parent,
    const char *name, 
    SbBool buildInsideParent,
    SoQtFullViewer::BuildFlag buildFlag, 
    SoQtViewer::Type t, 
    SbBool buildNow) 
	: SoQtViewer(
	    parent,
	    name, 
	    buildInsideParent, 
	    t, 
	    FALSE)  // buildNow
//
////////////////////////////////////////////////////////////////////////
{
    setClassName(thisClassName);
    
    setSize(SbVec2s(500, 390));  // default size
    // init decoration vars
    decorationFlag = (buildFlag & BUILD_DECORATION) != 0;
	mgrWidget = NULL;

       appButtonForm = NULL;
    rightWheelLabel = NULL;
    bottomWheelLabel = NULL;
    leftWheelLabel = NULL;
    appButtonForm = NULL;
    seekTimeInput = NULL;
    seekModeGroup = NULL;
    seekDistInput = NULL;
    seekDistModeGroup = NULL;
    zoomPrefSheet = NULL;
    zoomSlider = NULL;
    zoomInput = NULL;
    zoomFromInput = NULL;
    zoomToInput = NULL;
    autoClippingToggle = NULL;
    clipPlanesWidgets = NULL;
    clipNearInput = NULL;
    clipFarInput = NULL;
    stereoViewingToggle = NULL;
    stereoDistWidgets = NULL;
    stereoDistInput = NULL;

    // pre-create actions
    pickAction = new QAction (SoQtIcon::getIcon (SoQtIcon::PICK), tr("Pick mode"), this);
    connect (pickAction, SIGNAL (triggered(bool)), this, SLOT (setPickMode()));
    pickAction->setCheckable (true);

    viewAction = new QAction (SoQtIcon::getIcon (SoQtIcon::VIEW), tr("View mode"), this);
    connect (viewAction, SIGNAL (triggered(bool)), this, SLOT (setViewMode()));
    viewAction->setCheckable (true);

    QActionGroup* interactionGroup = new QActionGroup(this);
    interactionGroup->addAction (pickAction);
    interactionGroup->addAction (viewAction);

    pickAction->setChecked (!isViewing());
    viewAction->setChecked (isViewing());

    seekAction = new QAction (SoQtIcon::getIcon (SoQtIcon::SEEK), tr("Seek mode"), this);
    connect (seekAction, SIGNAL (triggered(bool)), this, SLOT (toggleSeekMode()));
    seekAction->setCheckable (true);
    seekAction->setChecked (isSeekMode());

    gotoHomeAction = new QAction (SoQtIcon::getIcon (SoQtIcon::HOME), tr("Goto home position"), this);
    connect (gotoHomeAction, SIGNAL (triggered(bool)), this, SLOT (resetToHomePosition()));

    setHomeAction = new QAction (SoQtIcon::getIcon (SoQtIcon::SET_HOME), tr("Set home position"), this);
    connect (setHomeAction, SIGNAL (triggered(bool)), this, SLOT (saveHomePosition()));

    viewAllAction = new QAction (SoQtIcon::getIcon (SoQtIcon::VIEW_ALL), tr("View all"), this);
    connect (viewAllAction, SIGNAL (triggered(bool)), this, SLOT (viewAll()));

    viewingAction = new QAction (tr("Viewing"), this);
    connect (viewingAction, SIGNAL (triggered(bool)), this, SLOT (toggleViewing()));
    viewingAction->setCheckable (true);
    viewingAction->setChecked (isViewing());

    IDM_MPOPUP_DECORAT = new QAction (tr("Decoration"), this);
    connect (IDM_MPOPUP_DECORAT, SIGNAL (triggered(bool)), this, SLOT (toggleDecoration()));
    IDM_MPOPUP_DECORAT->setCheckable (true);
    IDM_MPOPUP_DECORAT->setChecked (decorationFlag);

    IDM_MPOPUP_HLIGHT = new QAction (tr("Headlight"),  this);
    connect (IDM_MPOPUP_HLIGHT, SIGNAL (triggered(bool)), this, SLOT (toggleHeadlight()));
    IDM_MPOPUP_HLIGHT->setCheckable (true);
    IDM_MPOPUP_HLIGHT->setChecked (isHeadlight());
	
    IDM_MPOPUP_FULLSCREEN = new QAction (tr("FullScreen"),  this);
    IDM_MPOPUP_FULLSCREEN->setCheckable (true);
    IDM_MPOPUP_FULLSCREEN->setChecked (isFullScreen());
    connect (IDM_MPOPUP_FULLSCREEN, SIGNAL (triggered(bool)), this, SLOT (toggleFullScreen()));

    clipNearWheelVal = 0.0;
    clipFarWheelVal = 0.0;
    seekDistWheelVal = 0.0;
    stereoWheelVal = 0.0;
    rightWheelVal = 0.0;
    bottomWheelVal = 0.0;
    leftWheelVal = 0.0;

    // (the popup menu is created when it is shown for the first time)
    setPopupMenuEnabled ((buildFlag & BUILD_POPUP) != 0);
    zoomSldRange.setValue(1, 140);
    rightWheelStr  = tr("Motion Z");
    bottomWheelStr = tr("Motion X");
    leftWheelStr   = tr("Motion Y");

    popupTitle  = tr("Viewer Menu");
    prefSheetStr  = tr("Viewer Preference Sheet");
	
	// init popup menu vars
    mainPopup = NULL;
    clientPopup = NULL;
    popupEnabled = (buildFlag & BUILD_POPUP) != 0;
	// init buttons stuff
   appButtonList = new SbPList;
   viewerButtonWidgets = appButtonList;

    // Build the widget tree, and let SoXtComponent know about our base widget.
    if (buildNow) {
	setBaseWidget(buildWidget(getParentWidget()));
    }
	
    // build the popup menu 
    if (popupEnabled)
    	buildPopupMenu();
}



////////////////////////////////////////////////////////////////////////
//
//    Destructor.
//
// Use: protected

SoQtFullViewer::~SoQtFullViewer()
//
////////////////////////////////////////////////////////////////////////
{
    delete appButtonList;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    redefine this to also update the UI
//
// Use: virtual public
void
SoQtFullViewer::setViewing (SbBool flag)
//
////////////////////////////////////////////////////////////////////////
{
    if (flag == viewingFlag)
	return;

    // call the base class
    SoQtViewer::setViewing(flag);

    // update the push buttons
    viewAction->setChecked (viewingFlag);
    pickAction->setChecked (!viewingFlag);

    // update the popup menu entry
    viewingAction->setChecked (viewingFlag);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    redefine this to also update the popup menu
//
// Use: virtual public
void
SoQtFullViewer::setHeadlight (SbBool flag)
//
////////////////////////////////////////////////////////////////////////
{
    if (flag == isHeadlight())
	return;
    
    // call base class routine
    SoQtViewer::setHeadlight (flag);

    // update the popup menu entry
    IDM_MPOPUP_HLIGHT->setChecked (isHeadlight());
}

SbBool
SoQtFullViewer::setFullScreen( const SbBool enable )
{
    SoQtViewer::setFullScreen(enable);

    IDM_MPOPUP_FULLSCREEN->setChecked(isFullScreen());

    return TRUE;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    redefine this to also update the popup menu
//
// Use: virtual public
void
SoQtFullViewer::setDrawStyle (SoQtViewer::DrawType type,
                              SoQtViewer::DrawStyle style)
//
////////////////////////////////////////////////////////////////////////
{
    // call base class routine
    SoQtViewer::setDrawStyle(type, style);

    // update the popup menu entries
    switch(type)
    {
    case STILL: break;
    case INTERACTIVE: break;
    }

    switch (style)
    {
    case VIEW_AS_IS: break;
    case VIEW_HIDDEN_LINE: break;
    case VIEW_NO_TEXTURE: break;
    case VIEW_LOW_COMPLEXITY: break;
    case VIEW_LINE: break;
    case VIEW_POINT: break;
    case VIEW_BBOX: break;
    case VIEW_LOW_RES_LINE: break;
    case VIEW_LOW_RES_POINT: break;
    case VIEW_SAME_AS_STILL: break;
    }
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Sets the camera to use (done in base class) and makes sure to
// detach and re-attach() the zoom slider sensor.
//
// Use: virtual public
void
SoQtFullViewer::setCamera (SoCamera *newCamera)
//
////////////////////////////////////////////////////////////////////////
{
    // call base class routine
    SoQtViewer::setCamera(newCamera);

    // check if the zoom slider needs to be enabled
    if (zoomPrefSheet != NULL) {

        SbBool enable = camera != NULL &&
                        camera->isOfType (SoPerspectiveCamera::getClassTypeId());
        zoomPrefSheet->setEnabled (enable);

        // update the UI if enabled
        if (enable) {
            double zoom = getCameraZoom();
            setZoomSliderPosition (zoom);
        }
    }
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    shows/hides the decoration.
//
// Use: public
void
SoQtFullViewer::setDecoration (SbBool flag)
//
////////////////////////////////////////////////////////////////////////
{
    if (mgrWidget == NULL || flag == decorationFlag) {
    	decorationFlag = flag;
	return;
    }
    decorationFlag = flag;

	// check if decoration needs to be built
	// ??? just need to check one the decoration form widget ?
	if (leftTrimForm == NULL)
	    buildDecoration(mgrWidget);
		
	// show the decoration
    if (leftTrimForm)
        leftTrimForm->setVisible (decorationFlag);
    if (rightTrimForm)
       rightTrimForm->setVisible (decorationFlag);

    if (bottomTrimForm)
        bottomTrimForm->setVisible (decorationFlag);

    // update the popup menu entry
    IDM_MPOPUP_DECORAT->setChecked (decorationFlag);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    enables/disables the popup menu.
//
// Use: virtual public
void
SoQtFullViewer::setPopupMenuEnabled (SbBool flag)
//
////////////////////////////////////////////////////////////////////////
{
    popupEnabled = flag;

	if (popupEnabled)
		buildPopupMenu();
}

void
SoQtFullViewer::showPreferences()
{
    QDialog prefSheetDialog(getWidget());
    prefSheetDialog.setWindowTitle (prefSheetStr);
    prefSheetDialog.setLayout (new QVBoxLayout());
    
    QTabWidget* prefSheetContainer = new QTabWidget();

    prefSheetDialog.layout()->addWidget (prefSheetContainer);

    createPrefSheet(prefSheetContainer);
    
    prefSheetDialog.exec();
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Add a button to the end of the application list
//
// Use: public
void
SoQtFullViewer::addAppPushButton (QAbstractButton* newButton)
//
////////////////////////////////////////////////////////////////////////
{
    // add the button to the end of the list
    appButtonList->append(newButton);
    if (appButtonForm != NULL) {
        appButtonForm->layout()->addWidget (newButton);
    }
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Insert a button in the application list at the given index
//
// Use: public
void
SoQtFullViewer::insertAppPushButton(QAbstractButton* newButton, int index)
//
////////////////////////////////////////////////////////////////////////
{
    // add the button at the specified index
    appButtonList->insert(newButton, index);
    if (appButtonForm != NULL) {
        appButtonForm->layout()->addWidget (newButton);
    }
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Remove a button from the application list
//
// Use: public
void
SoQtFullViewer::removeAppPushButton(QAbstractButton* oldButton)
//
////////////////////////////////////////////////////////////////////////
{
    // find the index where the button is
    int index = appButtonList->find(oldButton);
    if (index == -1)
	return;
    
    // remove from the list and redo the layout
    int lastIndex = appButtonList->getLength() - 1;
    appButtonList->remove(index);
    appButtonForm->layout()->removeWidget(oldButton);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//	Sets the popup menu string
//
// use: protected
void
SoQtFullViewer::setPopupMenuString (const char* str)
//
////////////////////////////////////////////////////////////////////////
{
    popupTitle = str;
    if (mainPopup != NULL) {
        mainPopup->setTitle (str);
    }
}


void
SoQtFullViewer::setPrefSheetString(const char *name)
{
    prefSheetStr = name;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//	Sets the decoration bottom wheel string
//
// use: protected
void
SoQtFullViewer::setBottomWheelString(const char *name)
//
////////////////////////////////////////////////////////////////////////
{
    bottomWheelStr = name;
    if (bottomWheelLabel) {
        bottomWheelLabel->setText (name);
    }
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//	Sets the decoration left wheel string
//
// use: protected
void
SoQtFullViewer::setLeftWheelString(const char *name)
//
////////////////////////////////////////////////////////////////////////
{
    leftWheelStr = name;
    if (leftWheelLabel) {
        leftWheelLabel->setText (name);
    }
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//	Sets the decoration right wheel string
//
// use: protected
void
SoQtFullViewer::setRightWheelString(const char *name)
//
////////////////////////////////////////////////////////////////////////
{
    rightWheelStr = name;
    if (rightWheelLabel) {
        rightWheelLabel->setText (name);
    }
}


////////////////////////////////////////////////////////////////////////
//
// Description:
//	Builds the viewer popup menu.
//
// Use: virtual protected
void
SoQtFullViewer::buildPopupMenu()
//
////////////////////////////////////////////////////////////////////////
{
    mainPopup = new QMenu (popupTitle, getGlxMgrWidget());
    mainPopup->addMenu (buildFunctionsSubmenu(mainPopup));
    mainPopup->addMenu (buildDrawStyleSubmenu(mainPopup));
    mainPopup->addAction (viewingAction);
    mainPopup->addAction (IDM_MPOPUP_DECORAT);
    mainPopup->addAction (IDM_MPOPUP_FULLSCREEN);
    mainPopup->addAction (IDM_MPOPUP_HLIGHT);
    mainPopup->addAction (tr("Preferences..."), this, SLOT (showPreferences()));
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//	Builds function submenu - this include all of the viewer push 
//  buttons plus any useful entries.
//
// Use: protected
QMenu*
SoQtFullViewer::buildFunctionsSubmenu(QMenu* popup)
//
////////////////////////////////////////////////////////////////////////
{
    funcPopup = new QMenu (tr("Functions"));

    funcPopup->addAction (gotoHomeAction);
    funcPopup->addAction (setHomeAction);
    funcPopup->addAction (viewAllAction);
    funcPopup->addAction (seekAction);
    /*
    funcPopup->addSeparator();
    funcPopup->addAction (tr("Copy View"), this, SLOT (copyView()));
    funcPopup->addAction (tr("Paste View"), this, SLOT (pasteView()));
    */
    return funcPopup;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//	Builds drawing style submenu
//
// Use: protected
QMenu*
SoQtFullViewer::buildDrawStyleSubmenu(QMenu* popup)
//
////////////////////////////////////////////////////////////////////////
{
    drawPopup = new QMenu (tr("Draw Style"));

    QActionGroup* drawStyleStillGroup = new QActionGroup (this);
    drawStyleStillGroup->setExclusive (true);
    IDM_DPOPUP_ASIS          = drawStyleStillGroup->addAction (tr("as is"));
    IDM_DPOPUP_ASIS->setCheckable(true);
    IDM_DPOPUP_ASIS->setChecked(getDrawStyle(SoQtViewer::STILL) == SoQtViewer::VIEW_AS_IS);

    IDM_DPOPUP_HLINE    = drawStyleStillGroup->addAction (tr("hidden line"));
    IDM_DPOPUP_HLINE->setCheckable(true);
    IDM_DPOPUP_HLINE->setChecked(getDrawStyle(SoQtViewer::STILL) == SoQtViewer::VIEW_HIDDEN_LINE);

    IDM_DPOPUP_NOTEX     = drawStyleStillGroup->addAction (tr("no texture"));
    IDM_DPOPUP_NOTEX->setCheckable(true);
    IDM_DPOPUP_NOTEX->setChecked(getDrawStyle(SoQtViewer::STILL) == SoQtViewer::VIEW_NO_TEXTURE);

    IDM_DPOPUP_LOWRES = drawStyleStillGroup->addAction (tr("low resolution"));
    IDM_DPOPUP_LOWRES->setCheckable(true);
    IDM_DPOPUP_LOWRES->setChecked(getDrawStyle(SoQtViewer::STILL) == SoQtViewer::VIEW_LOW_COMPLEXITY);

    IDM_DPOPUP_WIRE     = drawStyleStillGroup->addAction (tr("wireframe"));
    IDM_DPOPUP_WIRE->setCheckable(true);
    IDM_DPOPUP_WIRE->setChecked(getDrawStyle(SoQtViewer::STILL) == SoQtViewer::VIEW_LINE);

    IDM_DPOPUP_POINTS        = drawStyleStillGroup->addAction (tr("points"));
    IDM_DPOPUP_POINTS->setCheckable(true);
    IDM_DPOPUP_POINTS->setChecked(getDrawStyle(SoQtViewer::STILL) == SoQtViewer::VIEW_POINT);
    
    IDM_DPOPUP_BBOX   = drawStyleStillGroup->addAction (tr("bounding box (no depth)"));
    IDM_DPOPUP_BBOX->setCheckable(true);
    IDM_DPOPUP_BBOX->setChecked(getDrawStyle(SoQtViewer::STILL) == SoQtViewer::VIEW_BBOX);

    drawPopup->addActions(drawStyleStillGroup->actions());
    drawPopup->addSeparator();

    QActionGroup* drawStyleInteractiveGroup = new QActionGroup (this);
    drawStyleInteractiveGroup->setExclusive (true);

    IDM_DPOPUP_MSAMEAS       = drawStyleInteractiveGroup->addAction (tr("move same as still"));
    IDM_DPOPUP_MSAMEAS->setCheckable(true);
    IDM_DPOPUP_MSAMEAS->setChecked(getDrawStyle(SoQtViewer::INTERACTIVE) == SoQtViewer::VIEW_SAME_AS_STILL);

    IDM_DPOPUP_MNOTEX         = drawStyleInteractiveGroup->addAction (tr("move no texture"));
    IDM_DPOPUP_MNOTEX->setCheckable(true);
    IDM_DPOPUP_MNOTEX->setChecked(getDrawStyle(SoQtViewer::INTERACTIVE) == SoQtViewer::VIEW_NO_TEXTURE);

    IDM_DPOPUP_MLOWRES            = drawStyleInteractiveGroup->addAction (tr("move low res"));
    IDM_DPOPUP_MLOWRES->setCheckable(true);
    IDM_DPOPUP_MLOWRES->setChecked(getDrawStyle(SoQtViewer::INTERACTIVE) == SoQtViewer::VIEW_LOW_COMPLEXITY);

    IDM_DPOPUP_MWIRE         = drawStyleInteractiveGroup->addAction (tr("move wireframe"));
    IDM_DPOPUP_MWIRE->setCheckable(true);
    IDM_DPOPUP_MWIRE->setChecked(getDrawStyle(SoQtViewer::INTERACTIVE) == SoQtViewer::VIEW_LINE);

    IDM_DPOPUP_MLRWIRE   = drawStyleInteractiveGroup->addAction (tr("move low res wireframe (no depth)"));
    IDM_DPOPUP_MLRWIRE->setCheckable(true);
    IDM_DPOPUP_MLRWIRE->setChecked(getDrawStyle(SoQtViewer::INTERACTIVE) == SoQtViewer::VIEW_LOW_RES_LINE);

    IDM_DPOPUP_MPOINTS            = drawStyleInteractiveGroup->addAction (tr("move points"));
    IDM_DPOPUP_MPOINTS->setCheckable(true);
    IDM_DPOPUP_MPOINTS->setChecked(getDrawStyle(SoQtViewer::INTERACTIVE) == SoQtViewer::VIEW_POINT);

    IDM_DPOPUP_MLRPOINTS      = drawStyleInteractiveGroup->addAction (tr("move low res points (no depth)"));
    IDM_DPOPUP_MLRPOINTS->setCheckable(true);
    IDM_DPOPUP_MLRPOINTS->setChecked(getDrawStyle(SoQtViewer::INTERACTIVE) == SoQtViewer::VIEW_LOW_RES_POINT);

    IDM_DPOPUP_MBBOX       = drawStyleInteractiveGroup->addAction (tr("move bounding box (no depth)"));
    IDM_DPOPUP_MBBOX->setCheckable(true);
    IDM_DPOPUP_MBBOX->setChecked(getDrawStyle(SoQtViewer::INTERACTIVE) == SoQtViewer::VIEW_BBOX);

    drawPopup->addActions(drawStyleInteractiveGroup->actions());
    drawPopup->addSeparator();

    // create the second part of this sub menu
    QActionGroup * bufferStyleGroup = new QActionGroup(this);
    bufferStyleGroup->setExclusive (true);
    IDM_DPOPUP_SBUFFER      = bufferStyleGroup->addAction (tr("single buffer"));
    IDM_DPOPUP_SBUFFER->setCheckable(true);
    IDM_DPOPUP_SBUFFER->setChecked(getBufferingType() == SoQtViewer::BUFFER_SINGLE);

    IDM_DPOPUP_DBUFFER      = bufferStyleGroup->addAction (tr("double buffer"));
    IDM_DPOPUP_DBUFFER->setCheckable(true);
    IDM_DPOPUP_DBUFFER->setChecked(getBufferingType() == SoQtViewer::BUFFER_DOUBLE);

    IDM_DPOPUP_IBUFFER = bufferStyleGroup->addAction (tr("interactive buffer"));
    IDM_DPOPUP_IBUFFER->setCheckable(true);
    IDM_DPOPUP_IBUFFER->setChecked(getBufferingType() == SoQtViewer::BUFFER_INTERACTIVE);
    drawPopup->addActions(bufferStyleGroup->actions());

    return drawPopup;
}

void
SoQtFullViewer::contextMenuEvent(QContextMenuEvent *event)
{
    if (!isPopupMenuEnabled())
        return;

    if (clientPopup) {
        clientPopup->exec(event->globalPos());
        return;
    }

    if (mainPopup != NULL) {
        QAction* res = mainPopup->exec(event->globalPos());
        if (res == IDM_DPOPUP_ASIS)
            setDrawStyle(SoQtViewer::STILL, SoQtViewer::VIEW_AS_IS);
        else if(res == IDM_DPOPUP_HLINE)
            setDrawStyle(SoQtViewer::STILL, SoQtViewer::VIEW_HIDDEN_LINE);
        else if(res == IDM_DPOPUP_NOTEX)
            setDrawStyle(SoQtViewer::STILL, SoQtViewer::VIEW_NO_TEXTURE);
        else if(res == IDM_DPOPUP_LOWRES)
            setDrawStyle(SoQtViewer::STILL, SoQtViewer::VIEW_LOW_COMPLEXITY);
        else if(res == IDM_DPOPUP_WIRE)
            setDrawStyle(SoQtViewer::STILL, SoQtViewer::VIEW_LINE);
        else if(res == IDM_DPOPUP_POINTS)
            setDrawStyle(SoQtViewer::STILL, SoQtViewer::VIEW_POINT);
        else if(res == IDM_DPOPUP_BBOX)
            setDrawStyle(SoQtViewer::STILL, SoQtViewer::VIEW_BBOX);
        else if(res == IDM_DPOPUP_MSAMEAS)
            setDrawStyle(SoQtViewer::INTERACTIVE, SoQtViewer::VIEW_SAME_AS_STILL);
        else if(res == IDM_DPOPUP_MNOTEX)
            setDrawStyle(SoQtViewer::INTERACTIVE, SoQtViewer::VIEW_NO_TEXTURE);
        else if(res == IDM_DPOPUP_MLOWRES)
            setDrawStyle(SoQtViewer::INTERACTIVE, SoQtViewer::VIEW_LOW_COMPLEXITY);
        else if(res == IDM_DPOPUP_MWIRE)
            setDrawStyle(SoQtViewer::INTERACTIVE, SoQtViewer::VIEW_LINE);
        else if(res == IDM_DPOPUP_MLRWIRE)
            setDrawStyle(SoQtViewer::INTERACTIVE, SoQtViewer::VIEW_LOW_RES_LINE);
        else if(res == IDM_DPOPUP_MPOINTS)
            setDrawStyle(SoQtViewer::INTERACTIVE, SoQtViewer::VIEW_POINT);
        else if(res == IDM_DPOPUP_MLRPOINTS)
            setDrawStyle(SoQtViewer::INTERACTIVE, SoQtViewer::VIEW_LOW_RES_POINT);
        else if(res == IDM_DPOPUP_MBBOX)
            setDrawStyle(SoQtViewer::INTERACTIVE, SoQtViewer::VIEW_BBOX);
        else if(res == IDM_DPOPUP_SBUFFER)
            setBufferingType(SoQtViewer::BUFFER_SINGLE);
        else if(res == IDM_DPOPUP_DBUFFER)
            setBufferingType(SoQtViewer::BUFFER_DOUBLE);
        else if(res == IDM_DPOPUP_IBUFFER)
            setBufferingType(SoQtViewer::BUFFER_INTERACTIVE);
    }
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//	Builds the app buttons form and any putton the application supplied
//
// Use: protected
void
SoQtFullViewer::buildAppButtons (QWidget* /*parent*/)
//
////////////////////////////////////////////////////////////////////////
{
    for (int i=0;i<appButtonList->getLength();i++) {
        QWidget* appButton = (QWidget*)(*appButtonList)[i];
        appButtonForm->layout()->addWidget (appButton);
    }
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//	Builds the basic Viewer Component widget, complete with
// functionality of a SoQtFullViewerManip, pop-up menu, sliders, etc.
// Builds all subwidgets, and does layout using motif
//
// Use: protected
QWidget*
SoQtFullViewer::buildWidget(QWidget *parent)
//
////////////////////////////////////////////////////////////////////////
{
    //
    // create a top level form to hold everything together
    //
    
    mgrWidget = new QWidget(parent);

	mgrWidget->setObjectName(getWidgetName().getString());
	mgrLayout = new QGridLayout(mgrWidget);
    mgrLayout->setMargin(0);
    mgrLayout->setSpacing(0);

    SbVec2s size = getSize();
    if ((size[0] != 0) && (size[1] != 0)) {
		mgrWidget->resize(size[0], size[1]);
    }
	
	// build the components
    raWidget = SoQtRenderArea::buildWidget(mgrWidget);
    raWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    if (decorationFlag)
    	buildDecoration(mgrWidget);
    
    //
    // Layout
    //
	mgrLayout->addWidget(raWidget, 1, 1);
    
    // manage children
    decorationFlag = !decorationFlag;   // enable routine to be called
    setDecoration(! decorationFlag);
    
    // build the popup menu 
    if (popupEnabled)
    	buildPopupMenu();

    return mgrWidget;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//	Builds the Decoration trim (thumbwheel, text, slider, buttons, ..).
//
// Use: virtual protected
void
SoQtFullViewer::buildDecoration(QWidget *parent)
//
////////////////////////////////////////////////////////////////////////
{
    // build the trim sides
    leftTrimForm = buildLeftTrim(parent);
    bottomTrimForm = buildBottomTrim(parent);
    rightTrimForm = buildRightTrim(parent);
    
    //
    // layout
    //
    mgrLayout->addWidget(leftTrimForm, 1, 0);
    mgrLayout->addWidget(bottomTrimForm, 2, 0, 1, 3);
    mgrLayout->addWidget(rightTrimForm, 1, 2);
 }
 
////////////////////////////////////////////////////////////////////////
//
// Description:
//	Builds the left trim decoration
//
// Use: virtual protected
QToolBar*
SoQtFullViewer::buildLeftTrim (QWidget* parent)
//
////////////////////////////////////////////////////////////////////////
{
    leftTrimForm = new QToolBar (parent);
    leftTrimForm->setOrientation(Qt::Vertical);
    buildAppButtons(leftTrimForm);

    QWidget* spacer = new QWidget();
    spacer->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    spacer->setLayout(new QVBoxLayout);
    spacer->layout()->setMargin(0);
    spacer->layout()->setSpacing(0);
    leftTrimForm->addWidget(spacer);

    leftWheel = new SoQtThumbWheel (Qt::Vertical, leftTrimForm);
    leftWheel->setValue(leftWheelVal);
    connect (leftWheel, SIGNAL (valueChanged(float)), this, SLOT (leftWheelCB(float)));
    connect (leftWheel, SIGNAL (sliderPressed()),    this, SLOT (leftWheelStart()));
    connect (leftWheel, SIGNAL (sliderReleased()),     this, SLOT (leftWheelFinish()));
    spacer->layout()->addWidget (leftWheel);
    spacer->layout()->setAlignment(leftWheel, Qt::AlignBottom);

    return leftTrimForm;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//	Builds the bottom trim decoration
//
// Use: virtual protected
QToolBar*
SoQtFullViewer::buildBottomTrim (QWidget* parent)
//
////////////////////////////////////////////////////////////////////////
{
    bottomTrimForm = new QToolBar (parent);
    bottomTrimForm->setOrientation(Qt::Horizontal);

    QWidget* spacer = new QWidget();
    spacer->setLayout(new QHBoxLayout);
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    spacer->layout()->setMargin(0);

    bottomTrimForm->addWidget (spacer);

    leftWheelLabel = new QLabel (leftWheelStr, bottomTrimForm);
    spacer->layout()->addWidget (leftWheelLabel);

    bottomWheelLabel = new QLabel (bottomWheelStr, bottomTrimForm);
    spacer->layout()->addWidget (bottomWheelLabel);

    bottomWheel = new SoQtThumbWheel (Qt::Horizontal, bottomTrimForm);
    bottomWheel->setFixedSize(130, 20);
    bottomWheel->setValue (bottomWheelVal);
    connect (bottomWheel, SIGNAL (valueChanged(float)), this, SLOT (bottomWheelCB(float)));
    connect (bottomWheel, SIGNAL (sliderPressed()),    this, SLOT (bottomWheelStart()));
    connect (bottomWheel, SIGNAL (sliderReleased()),     this, SLOT (bottomWheelFinish()));
    spacer->layout()->addWidget (bottomWheel);

    ((QHBoxLayout*)spacer->layout())->addStretch();
    
    rightWheelLabel = new QLabel (rightWheelStr, bottomTrimForm);
    spacer->layout()->addWidget (rightWheelLabel);
    spacer->layout()->setAlignment(rightWheelLabel, Qt::AlignRight);

    return bottomTrimForm;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//	Builds the right trim decoration
//
// Use: virtual protected
QToolBar*
SoQtFullViewer::buildRightTrim (QWidget* parent)
//
////////////////////////////////////////////////////////////////////////
{
    rightTrimForm = new QToolBar (parent);
    rightTrimForm->setOrientation (Qt::Vertical);
    createViewerButtons (rightTrimForm);

    QWidget* spacer = new QWidget();
    spacer->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    spacer->setLayout(new QVBoxLayout);
    spacer->layout()->setMargin(0);
    spacer->layout()->setSpacing(0);
    rightTrimForm->addWidget (spacer);

    rightWheel = new SoQtThumbWheel (Qt::Vertical, rightTrimForm);
    rightWheel->setValue (rightWheelVal);
    connect (rightWheel, SIGNAL (valueChanged(float)), this, SLOT (rightWheelCB(float)));
    connect (rightWheel, SIGNAL (sliderPressed()),    this, SLOT (rightWheelStart()));
    connect (rightWheel, SIGNAL (sliderReleased()),     this, SLOT (rightWheelFinish()));
    spacer->layout()->addWidget (rightWheel);
    spacer->layout()->setAlignment(rightWheel, Qt::AlignBottom|Qt::AlignHCenter);

    return rightTrimForm;
}


////////////////////////////////////////////////////////////////////////
//
// Description:
//	creates the default viewer buttons
//
// Use: virtual protected
void
SoQtFullViewer::createViewerButtons (QToolBar* parent)
//
////////////////////////////////////////////////////////////////////////
{
    parent->addAction (pickAction);
    parent->addAction (viewAction);
    parent->addAction (gotoHomeAction);
    parent->addAction (setHomeAction);
    parent->addAction (viewAllAction);
    parent->addAction (seekAction);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//	This creates the preference sheet in a separate window. It
//  calls other routines to create the actual content of the sheet.
//
// Use: virtual protected
void
SoQtFullViewer::createPrefSheet(QTabWidget* parent)
//
////////////////////////////////////////////////////////////////////////
{
    createSeekPrefSheetGuts(parent);
    createSeekDistPrefSheetGuts(parent);
    createZoomPrefSheetGuts(parent);
    createClippingPrefSheetGuts(parent);
    createStereoPrefSheetGuts(parent);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//	This creates the seek preference sheet stuff.
//
// Use: protected
void
SoQtFullViewer::createSeekPrefSheetGuts (QTabWidget* parent)
//
////////////////////////////////////////////////////////////////////////
{
    QWidget* sheet = new QWidget();
    QGridLayout* grid = new QGridLayout();
    sheet->setLayout (grid);

    grid->addWidget (new QLabel (tr("Seek animation time:")), 0, 0);
    seekTimeInput = new QDoubleSpinBox();
    seekTimeInput->setRange (0.0, 60.0);
    seekTimeInput->setValue (getSeekTime());
    connect (seekTimeInput, SIGNAL (valueChanged(double)), this, SLOT (setSeekTime(double)));
    //-> SoQtFullViewer::seekPrefSheetFieldCB
    grid->addWidget (seekTimeInput, 0, 1);
    grid->addWidget (new QLabel (tr("seconds")), 0, 2);

    grid->addWidget (new QLabel (tr("Seek to:")), 1, 0);
    QAbstractButton* seekPointsButton = new QRadioButton (tr("point"));
    grid->addWidget (seekPointsButton, 1, 1);
    QAbstractButton* seekObjectsButton = new QRadioButton (tr("object"));
    grid->addWidget (seekObjectsButton, 1, 2);
    // make buttons mutually exclusive:
    QButtonGroup* group = new QButtonGroup (sheet);
    group->addButton (seekPointsButton, 1);
    group->addButton (seekObjectsButton, 0);
    group->button ((int) isDetailSeek())->setChecked (true);
    connect (group, SIGNAL (buttonClicked(int)), this, SLOT (setSeekDetailMode(int)));
    seekModeGroup = group;
    //-> SoQtFullViewer::seekPrefSheetToggle1CB
    //-> SoQtFullViewer::seekPrefSheetToggle2CB

    parent->addTab(sheet, tr("Seek"));
}


////////////////////////////////////////////////////////////////////////
//
// Description:
//	This creates the seek distance setting preference sheet stuff.
//
// Use: protected
void
SoQtFullViewer::createSeekDistPrefSheetGuts (QTabWidget* parent)
//
////////////////////////////////////////////////////////////////////////
{
    QWidget* sheet = new QWidget();
    QGridLayout* grid = new QGridLayout();
    sheet->setLayout (grid);

    grid->addWidget (new QLabel (tr("Seek distance:")), 0, 0);
    SoQtThumbWheel* seekDistWheel = new SoQtThumbWheel (Qt::Horizontal);
    seekDistWheel->setValue (seekDistance);
    connect (seekDistWheel, SIGNAL (valueChanged(float)), this, SLOT (setSeekDistWheelValue(float)));
    //-> SoQtFullViewer::seekDistWheelCB
    grid->addWidget (seekDistWheel, 0, 1);
    seekDistInput = new QDoubleSpinBox();
    seekDistInput->setRange (0.0, 1000.0);
    seekDistInput->setValue (seekDistance);
    connect (seekDistInput, SIGNAL (valueChanged(double)), this, SLOT (setSeekDistance(double)));
    //-> SoQtFullViewer::seekDistInputCB
    grid->addWidget (seekDistInput, 0, 2);

    // create the second line
    QAbstractButton* seekDistPercButton = new QRadioButton (tr("percentage"));
    grid->addWidget (seekDistPercButton, 1, 1);
    QAbstractButton* seekDistAbsButton = new QRadioButton (tr("absolute"));
    grid->addWidget (seekDistAbsButton, 1, 2);

    // make buttons mutually exclusive:
    QButtonGroup* group = new QButtonGroup (sheet);
    group->addButton (seekDistPercButton, 1);
    group->addButton (seekDistAbsButton, 0);
    group->button ((int) seekDistAsPercentage)->setChecked (true);
    connect (group, SIGNAL (buttonClicked(int)), this, SLOT (setSeekDistMode(int)));
    seekDistModeGroup = group;
    //-> SoQtFullViewer::seekDistPercPrefSheetToggleCB
    //-> SoQtFullViewer::seekDistAbsPrefSheetToggleCB

    parent->addTab(sheet, tr("Seek Distance"));
}


void
SoQtFullViewer::createZoomPrefSheetGuts (QTabWidget* parent)
{
    QWidget* zoomPrefSheet = new QWidget();
    QGridLayout* grid = new QGridLayout();
    zoomPrefSheet->setLayout (grid);

    grid->addWidget (new QLabel (tr("Camera Zoom:")), 0, 0);
    zoomSlider = new QSlider (Qt::Horizontal);
    zoomSlider->setRange (0, 1000);
    setZoomSliderPosition (getCameraZoom());
    connect (zoomSlider, SIGNAL (valueChanged (int)), this, SLOT (setZoomSliderValue(int)));
    //-> SoQtFullViewer::zoomSliderCB
    grid->addWidget (zoomSlider, 0, 1);
    zoomInput = new QDoubleSpinBox();
    zoomInput->setRange (0, 1000);
    zoomInput->setValue (getCameraZoom());
    connect (zoomInput, SIGNAL (valueChanged(double)), this, SLOT (setZoomValue(double)));
    //-> SoQtFullViewer::zoomFieldCB
    grid->addWidget (zoomInput, 0,2);

    grid->addWidget (new QLabel (tr("Zoom slider ranges from:")), 1, 0);

    QHBoxLayout* box = new QHBoxLayout();

    zoomFromInput = new QDoubleSpinBox();
    zoomFromInput->setDecimals (1);
    zoomFromInput->setRange (0.0, 180.0);
    zoomFromInput->setValue (zoomSldRange[0]);
    connect (zoomFromInput, SIGNAL (valueChanged(double)), this, SLOT (setZoomMinValue(double)));
    //-> SoQtFullViewer::zoomPrefSheetMinFieldCB
    box->addWidget (zoomFromInput, 1);

    box->addWidget (new QLabel (tr("to:")));

    zoomToInput = new QDoubleSpinBox();
    zoomToInput->setDecimals (1);
    zoomToInput->setRange (0.0, 180.0);
    zoomToInput->setValue (zoomSldRange[1]);
    connect (zoomToInput, SIGNAL (valueChanged(double)), this, SLOT (setZoomMaxValue(double)));
    //-> SoQtFullViewer::zoomPrefSheetMaxFieldCB
    box->addWidget (zoomToInput, 1);

    grid->addLayout (box, 1, 1, 1, 2);

    bool enabled = (camera != NULL && camera->isOfType(SoPerspectiveCamera::getClassTypeId()));
    // TODO: keep reference to sheet so we can en-/disable it as needed
    zoomPrefSheet->setEnabled (enabled);

    parent->addTab(zoomPrefSheet, tr("Zoom"));
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//	This creates the clipping plane preference sheet stuff.
//
// Use: protected
void
SoQtFullViewer::createClippingPrefSheetGuts(QTabWidget* parent)
//
////////////////////////////////////////////////////////////////////////
{
    QWidget* sheet = new QWidget();
    QBoxLayout* box = new QVBoxLayout();
    sheet->setLayout (box);

    autoClippingToggle = new QCheckBox(tr("Auto clipping planes"));
    autoClippingToggle->setEnabled (camera != NULL);
    autoClippingToggle->setCheckState (isAutoClipping() ? Qt::Checked : Qt::Unchecked);
    connect (autoClippingToggle, SIGNAL (stateChanged(int)), this, SLOT (setAutoClippingState(int)));
    //-> SoQtFullViewer::clipPrefSheetToggleCB
    box->addWidget (autoClippingToggle);

    clipPlanesWidgets = new QWidget();
    QGridLayout* grid = new QGridLayout();
    clipPlanesWidgets->setLayout (grid);

    grid->addWidget (new QLabel (tr("near plane:")), 0, 0);

    double value = (camera != NULL) ? camera->nearDistance.getValue() : 0.0;
    SoQtThumbWheel* clipNearWheel = new SoQtThumbWheel (Qt::Horizontal);
    connect (clipNearWheel, SIGNAL (valueChanged(float)), this, SLOT (setClipNearWheelValue(float)));
    //-> SoQtFullViewer::clipNearWheelCB
    grid->addWidget (clipNearWheel, 0, 1);

    clipNearInput = new QDoubleSpinBox();
    // clipNearInput>setDecimals (5);
    clipNearInput->setRange (0.0, 1000.0);
    clipNearInput->setValue (value);
    connect (clipNearInput, SIGNAL (valueChanged(double)), this, SLOT (setClipNearValue(double)));
    //-> SoQtFullViewer::clipFieldCB
    grid->addWidget (clipNearInput, 0, 2);

    grid->addWidget (new QLabel (tr("far plane:")), 1, 0);

    value = (camera != NULL) ? camera->farDistance.getValue() : 0.0;
    SoQtThumbWheel* clipFarWheel = new SoQtThumbWheel (Qt::Horizontal);
    connect (clipFarWheel, SIGNAL (valueChanged(float)), this, SLOT (setClipFarWheelValue(float)));
    //-> SoQtFullViewer::clipFarWheelCB
    grid->addWidget (clipFarWheel, 1, 1);

    clipFarInput = new QDoubleSpinBox();
    // clipFarInput>setDecimals (5);
    clipFarInput->setRange (0.0, 1000.0);
    clipFarInput->setValue (value);
    connect (clipFarInput, SIGNAL (valueChanged(double)), this, SLOT (setClipFarValue(double)));
    //-> SoQtFullViewer::clipFieldCB
    grid->addWidget (clipFarInput, 1, 2);

    box->addWidget (clipPlanesWidgets);
    clipPlanesWidgets->setEnabled (!isAutoClipping());

    parent->addTab(sheet, tr("Clipping"));
}



////////////////////////////////////////////////////////////////////////
//
// Description:
//	This creates the stereo viewing preference sheet stuff.
//
// Use: protected
void
SoQtFullViewer::createStereoPrefSheetGuts(QTabWidget* parent)
//
////////////////////////////////////////////////////////////////////////
{
    QWidget* sheet = new QWidget();
    QBoxLayout* box = new QVBoxLayout();
    sheet->setLayout (box);

    stereoViewingToggle = new QCheckBox(tr("Stereo Viewing"));
    stereoViewingToggle->setCheckState (isStereoViewing() ? Qt::Checked : Qt::Unchecked);
    connect (stereoViewingToggle, SIGNAL (stateChanged(int)), this, SLOT (setStereoViewingValue(int)));
    //-> SoQtFullViewer::stereoPrefSheetToggleCB
    box->addWidget (stereoViewingToggle);

    stereoDistWidgets = new QWidget (sheet);
    QBoxLayout* distBox = new QHBoxLayout();
    stereoDistWidgets->setLayout (distBox);
    distBox->addWidget (new QLabel (tr("camera rotation:")));
    SoQtThumbWheel* stereoDistWheel = new SoQtThumbWheel (Qt::Horizontal);
    connect (stereoDistWheel, SIGNAL (valueChanged(float)), this, SLOT (setStereoWheelValue(float)));
    //-> SoQtFullViewer::stereoWheelCB
    distBox->addWidget (stereoDistWheel);
    stereoDistInput = new QDoubleSpinBox();
    stereoDistInput->setDecimals (4);
    stereoDistInput->setRange (-100.0, 100.0);
    stereoDistInput->setValue (getStereoOffset());
    connect (stereoDistInput, SIGNAL (valueChanged(double)), this, SLOT (setStereoDistance(double)));
    //-> SoQtFullViewer::stereoFieldCB
    distBox->addWidget (stereoDistInput);
    stereoDistWidgets->setEnabled (isStereoViewing());
    box->addWidget (stereoDistWidgets, 1, Qt::AlignVCenter);

    parent->addTab(sheet, tr("Stereo"));
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//	Creates the viewer speed pref sheet stuff
//
// Use: protected
void
SoQtFullViewer::createSpeedPrefSheetGuts(QTabWidget* parent)
//
////////////////////////////////////////////////////////////////////////
{
    QWidget* sheet = new QWidget();
    QBoxLayout* box = new QVBoxLayout();
    sheet->setLayout (box);

    box->addWidget (new QLabel (tr("Viewer Speed:")));

    QPushButton* incSpeedButton = new QPushButton (tr("increase"));
    connect (incSpeedButton, SIGNAL (clicked(bool)), this, SLOT (increaseSpeed()));
    //-> SoQtFullViewer::speedIncPrefSheetButtonCB
    box->addWidget (incSpeedButton);

    QPushButton* decSpeedButton = new QPushButton (tr("decrease"));
    connect (decSpeedButton, SIGNAL (clicked(bool)), this, SLOT (decreaseSpeed()));
    //-> SoQtFullViewer::speedDecPrefSheetButtonCB
    box->addWidget (decSpeedButton);

    parent->addTab(sheet, tr("Speed"));
}

void
SoQtFullViewer::setSeekMode (SbBool onOrOff)
{
    SoQtViewer::setSeekMode (onOrOff);
    seekAction->setChecked (isSeekMode());
}

void
SoQtFullViewer::setDetailSeek(SbBool onOrOff)
{
    SoQtViewer::setDetailSeek (onOrOff);
    if (seekDistModeGroup) {
        seekDistModeGroup->button ((int) seekDistAsPercentage)->setChecked (true);
    }
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Sets the camera given zoom value (in degree for perspective cameras).
//
// Use: private

void
SoQtFullViewer::setCameraZoom (float zoom)
//
////////////////////////////////////////////////////////////////////////
{
    if (camera == NULL) {
        return;
    } else if ( camera->isOfType (SoPerspectiveCamera::getClassTypeId()) ) {
        ((SoPerspectiveCamera *)camera)->heightAngle = zoom * M_PI / 180.0;
    } else if ( camera->isOfType (SoOrthographicCamera::getClassTypeId()) ) {
        ((SoOrthographicCamera *)camera)->height = zoom;
#if DEBUG
    } else {
        SoDebugError::post ("SoQtFullViewer::setCameraZoom",
                "unknown camera type");
#endif
    }
}


float
SoQtFullViewer::getCameraZoom()
{
    if (camera == NULL) {
        return 0.0;
    } else if ( camera->isOfType (SoPerspectiveCamera::getClassTypeId()) ) {
        return ((SoPerspectiveCamera *)camera)->heightAngle.getValue() * 180.0 / M_PI;
    } else if ( camera->isOfType (SoOrthographicCamera::getClassTypeId()) ) {
        return ((SoOrthographicCamera *)camera)->height.getValue();
    } else {
#if DEBUG
        SoDebugError::post("SoQtFullViewer::getCameraZoom",
                            "unknown camera type");
#endif
        return 0.0;
    }
}


void
SoQtFullViewer::setZoomSliderPosition (float zoom)
{
    if (zoomSlider != NULL) {
        // find the slider position, using a square root distance to make the
        // slider smoother and less sensitive when close to zero.
        double f = (zoom - zoomSldRange[0]) / (zoomSldRange[1] - zoomSldRange[0]);
        f = (f < 0.0) ? 0.0 : ((f > 1.0) ? 1.0 : f);
        f = sqrt (f);
        // finally position the slider
        zoomSlider->setValue (int (f * 1000));
    }
    if (zoomInput != NULL) {
        zoomInput->setValue (zoom);
    }
}

void
SoQtFullViewer::setZoomValue (double zoom)
{
    if (nestingFlag) { return; }
    nestingFlag = true;
    if (zoom > 0.0) {
        // check for valid perspective camera range
        if ( camera != NULL &&
            camera->isOfType(SoPerspectiveCamera::getClassTypeId()) ) {
            zoom = (zoom < 0.01) ? 0.01 : ((zoom > 179.99) ? 179.99 : zoom);
        }

        // check if the new value changed the slider range
        if (zoom < zoomSldRange[0]) {
            zoomSldRange[0] = zoom;
        } else if (zoom > zoomSldRange[1]) {
            zoomSldRange[1] = zoom;
        }
        setCameraZoom (zoom);
    } else {
        zoom = getCameraZoom();
    }

    setZoomSliderPosition (zoom);
    nestingFlag = false;
}

void
SoQtFullViewer::setZoomSliderValue (int value)
{
    // get the slider zoom value, taking the square value since we
    // are using the square root to make the slider smoother to use.
    double f = int(value) / 1000.0;
    f *= f;
    setZoomValue (zoomSldRange[0] + f * (zoomSldRange[1] - zoomSldRange[0]));
}

void
SoQtFullViewer::setAutoClippingState (int state)
{
    SoQtViewer::setAutoClipping (state == Qt::Checked);
    clipPlanesWidgets->setEnabled (!isAutoClipping());
}



void
SoQtFullViewer::setClipNearWheelValue (float value)
{
    if (camera == NULL) { return; }

    // shorter/grow the near plane distance given the wheel rotation
    double dist = camera->nearDistance.getValue();
    dist *= pow (80.0, (value - clipNearWheelVal) / 360.0);
    clipNearWheelVal = value;

    setClipNearValue (dist);
}

void
SoQtFullViewer::setClipFarWheelValue (float value)
{
    if (camera == NULL) { return; }

    // shorter/grow the near plane distance given the wheel rotation
    double dist = camera->farDistance.getValue();
    dist *= pow (80.0, (value - clipFarWheelVal) / 360.0);
    clipFarWheelVal = value;

    setClipFarValue (dist);
}

void
SoQtFullViewer::setClipNearValue (double value)
{
    if (camera == NULL) { return; }
    if (nestingFlag) { return; }
    nestingFlag = true;

    if (value > 0.0 || camera->isOfType(SoOrthographicCamera::getClassTypeId())) {
        camera->nearDistance = value;
    }
    if (clipNearInput != NULL) {
        clipNearInput->setValue (value);
    }
    nestingFlag = false;
}

void
SoQtFullViewer::setClipFarValue (double value)
{
    if (camera == NULL) { return; }
    if (nestingFlag) { return; }
    nestingFlag = true;

    if (value > 0.0 || camera->isOfType(SoOrthographicCamera::getClassTypeId())) {
        camera->farDistance = value;
    }
    if (clipFarInput != NULL) {
        clipFarInput->setValue (value);
    }
    nestingFlag = false;
}

void
SoQtFullViewer::setStereoViewingValue (int state)
{
    // checks to make sure stereo viewing can be set, else
    // grey the UI and bring and error message.
    SbBool toggleState = (state == Qt::Checked);
    if (toggleState != isStereoViewing()) {
        SoQtViewer::setStereoViewing (toggleState);
    }
    if (toggleState && !isStereoViewing()) {
        // we couldn't activate stereo viewing:
        stereoViewingToggle->setCheckState (Qt::Unchecked);
        stereoViewingToggle->setEnabled (false);
        QMessageBox::critical (stereoViewingToggle, tr("Stereo Error"),
                               tr("Stereo Viewing can't be set on this machine."));
        return;
    }

    // show/hide the eye spacing thumbwheel
    stereoDistWidgets->setEnabled (isStereoViewing());
}

void
SoQtFullViewer::setStereoWheelValue (float value)
{
    // shorter/grow the stereo camera offset
    setStereoDistance (getStereoOffset() * pow (80.0, value - stereoWheelVal) / 360.0);
    stereoWheelVal = value;
}

void
SoQtFullViewer::setStereoDistance (double value)
{
    // get text value from the label and update camera
    setStereoOffset(value);
    scheduleRedraw();
    if (stereoDistInput != NULL) {
        stereoDistInput->setValue (value);
    }
}

void
SoQtFullViewer::setSeekDistWheelValue (float value)
{
    // shorter/grow the seek distance given the wheel rotation
    setSeekDistance (seekDistance *
                     pow (80.0, (value - seekDistWheelVal) / 360.0));
    seekDistWheelVal = value;
}

void
SoQtFullViewer::setSeekDistance (double value)
{
    seekDistance = value;
    if (seekDistInput != NULL && !nestingFlag) {
        nestingFlag = true;
        seekDistInput->setValue (value);
        nestingFlag = false;
    }
}

void
SoQtFullViewer::setSeekTime (double value)
{
    SoQtViewer::setSeekTime (value);
    if (seekTimeInput != NULL && !nestingFlag) {
        nestingFlag = true;
        seekTimeInput->setValue (value);
        nestingFlag = false;
    }
}

void
SoQtFullViewer::setZoomMinValue (double value)
{
    // check for valid perspective camera range
    if (camera != NULL &&
        camera->isOfType(SoPerspectiveCamera::getClassTypeId())) {
        value = (value < 0.01) ? 0.01 : ((value > 178.99) ? 178.99 : value);

        // finally update the slider to reflect the changes
        zoomSldRange[0] = value;
        setZoomSliderPosition (getCameraZoom());
    }
}

void
SoQtFullViewer::setZoomMaxValue (double value)
{
    // check for valid perspective camera range
    if (camera != NULL &&
        camera->isOfType(SoPerspectiveCamera::getClassTypeId())) {
        value = (value < 0.01) ? 0.01 : ((value > 178.99) ? 178.99 : value);

        // finally update the slider to reflect the changes
        zoomSldRange[1] = value;
        setZoomSliderPosition (getCameraZoom());
    }
}

void SoQtFullViewer::rightWheelCB(float value)
{
    rightWheelMotion(- value * M_PI / 180.0);
}

void SoQtFullViewer::bottomWheelCB(float value)
{
    bottomWheelMotion(value * M_PI / 180.0);
}

void SoQtFullViewer::leftWheelCB(float value)
{
    leftWheelMotion(- value * M_PI / 180.0);
}

//
// Virtual thumb wheels methods which subclasses can redefine
//
void SoQtFullViewer::rightWheelStart()      { interactiveCountInc(); }
void SoQtFullViewer::bottomWheelStart()     { interactiveCountInc(); }
void SoQtFullViewer::leftWheelStart()       { interactiveCountInc(); }
void SoQtFullViewer::rightWheelFinish()     { interactiveCountDec(); }
void SoQtFullViewer::bottomWheelFinish()    { interactiveCountDec(); }
void SoQtFullViewer::leftWheelFinish()      { interactiveCountDec(); }

void SoQtFullViewer::rightWheelMotion(float v)    { rightWheelVal  = v; }
void SoQtFullViewer::bottomWheelMotion(float v)   { bottomWheelVal = v; }
void SoQtFullViewer::leftWheelMotion(float v)     { leftWheelVal   = v; }
void SoQtFullViewer::openViewerHelpCard()	{}
