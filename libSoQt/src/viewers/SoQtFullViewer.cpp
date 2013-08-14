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
#include <Inventor/Qt/viewers/SoQtPreferenceDialog.h>
#include <Inventor/Qt/SoQtIcons.h>
#include <Inventor/nodes/SoOrthographicCamera.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/errors/SoDebugError.h>

#include <QAbstractButton>
#include <QAction>
#include <QActionGroup>
#include <QToolBar>
#include <QLayout>
#include <QMenu>
#include <QLabel>
#include <QContextMenuEvent>

#include <Inventor/Qt/SoQtThumbWheel.h>

static const char *thisClassName = "SoQtFullViewer";

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

    // pre-create actions
    pickAction = new QAction (SoQtIcon::getIcon (SoQtIcon::PICK), tr("Pick mode"), this);
    connect (pickAction, SIGNAL (triggered(bool)), this, SLOT (setPickModeCB()));
    pickAction->setCheckable (true);

    viewAction = new QAction (SoQtIcon::getIcon (SoQtIcon::VIEW), tr("View mode"), this);
    connect (viewAction, SIGNAL (triggered(bool)), this, SLOT (setViewModeCB()));
    viewAction->setCheckable (true);

    QActionGroup* interactionGroup = new QActionGroup(this);
    interactionGroup->addAction (pickAction);
    interactionGroup->addAction (viewAction);

    pickAction->setChecked (!isViewing());
    viewAction->setChecked (isViewing());

    seekAction = new QAction (SoQtIcon::getIcon (SoQtIcon::SEEK), tr("Seek mode"), this);
    connect (seekAction, SIGNAL (triggered(bool)), this, SLOT (setSeekModeCB(bool)));

    gotoHomeAction = new QAction (SoQtIcon::getIcon (SoQtIcon::HOME), tr("Goto home position"), this);
    connect (gotoHomeAction, SIGNAL (triggered(bool)), this, SLOT (resetToHomePosition()));

    setHomeAction = new QAction (SoQtIcon::getIcon (SoQtIcon::SET_HOME), tr("Set home position"), this);
    connect (setHomeAction, SIGNAL (triggered(bool)), this, SLOT (saveHomePosition()));

    viewAllAction = new QAction (SoQtIcon::getIcon (SoQtIcon::VIEW_ALL), tr("View all"), this);
    connect (viewAllAction, SIGNAL (triggered(bool)), this, SLOT (viewAll()));

    viewingAction = new QAction (tr("Viewing"), this);
    connect (viewingAction, SIGNAL (triggered(bool)), this, SLOT (setViewingCB(bool)));
    viewingAction->setCheckable (true);
    viewingAction->setChecked (isViewing());

    IDM_MPOPUP_DECORAT = new QAction (tr("Decoration"), this);
    connect (IDM_MPOPUP_DECORAT, SIGNAL (triggered(bool)), this, SLOT (setDecorationCB(bool)));
    IDM_MPOPUP_DECORAT->setCheckable (true);
    IDM_MPOPUP_DECORAT->setChecked (decorationFlag);

    IDM_MPOPUP_HLIGHT = new QAction (tr("Headlight"),  this);
    connect (IDM_MPOPUP_HLIGHT, SIGNAL (triggered(bool)), this, SLOT (setHeadlightCB(bool)));
    IDM_MPOPUP_HLIGHT->setCheckable (true);
    IDM_MPOPUP_HLIGHT->setChecked (isHeadlight());

    IDM_MPOPUP_FULLSCREEN = new QAction (tr("FullScreen"),  this);
    IDM_MPOPUP_FULLSCREEN->setCheckable (true);
    IDM_MPOPUP_FULLSCREEN->setChecked (isFullScreen());
    connect (IDM_MPOPUP_FULLSCREEN, SIGNAL (triggered(bool)), this, SLOT (setFullScreen(bool)));

    rightWheelVal = 0.0;
    bottomWheelVal = 0.0;
    leftWheelVal = 0.0;

    rightWheelStr  = tr("Motion Z");
    bottomWheelStr = tr("Motion X");
    leftWheelStr   = tr("Motion Y");

    popupTitle  = tr("Viewer Menu");
    // init pref sheet vars
    prefSheetShellWidget = NULL;
    prefSheetStr  = tr("Viewer Preference Sheet");

    // init popup menu vars
    mainPopup = NULL;
    clientPopup = NULL;
    popupEnabled = (buildFlag & BUILD_POPUP) != 0;

    // init buttons stuff
    appButtonList = new SbPList;

    // Build the widget tree, and let SoXtComponent know about our base widget.
    if (buildNow) {
        setBaseWidget(buildWidget(getParentWidget()));
    }
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
    delete prefSheetShellWidget;
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

void
SoQtFullViewer::setFullScreen(bool enable)
{
    SoQtViewer::setFullScreen(enable);

    IDM_MPOPUP_FULLSCREEN->setChecked(isFullScreen());
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

    // check if the prefs needs to be updated
    if (prefSheetShellWidget)
        prefSheetShellWidget->setCamera (newCamera);
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
}

void
SoQtFullViewer::showPreferences()
{
    if (!prefSheetShellWidget) {
        prefSheetShellWidget = new SoQtPreferenceDialog(this, getWidget());

        createPrefSheet(prefSheetShellWidget->tabWidget());
    }
    prefSheetShellWidget->show();
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
    if (mainPopup)
        return;

    mainPopup = new QMenu (popupTitle, baseWidget());
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

    // build the popup menu
    buildPopupMenu();

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

void
SoQtFullViewer::setSeekMode (SbBool onOrOff)
{
    SoQtViewer::setSeekMode (onOrOff);
    seekAction->setChecked (isSeekMode());
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
