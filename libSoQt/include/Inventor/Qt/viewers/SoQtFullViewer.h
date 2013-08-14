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
 |   Classes	: SoQtFullViewer
 |
 |   Author(s)	: Alain Dumesny
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

/*
 * Ported to Qt by Morgan Leborgne, 2010
 */

#ifndef  _SO_QT_FULL_VIEWER_
#define  _SO_QT_FULL_VIEWER_

#include <QPointer>

#include <Inventor/Qt/SoQt.h>
#include <Inventor/Qt/viewers/SoQtViewer.h>
#include <Inventor/SbPList.h>


//////////////////////////////////////////////////////////////////////////////
//
//  Class: SoQtFullViewer
//
//	The SoQtFullViewer component class is the abstract base class for all
//  viewers which include a decoration around the rendering area. The 
//  decoration is made of thumbwheels, sliders and push/toggle buttons. There
//  is also a popup menu which includes all of the viewing options and methods.
//
//////////////////////////////////////////////////////////////////////////////

class QAbstractButton;
class QAction;
class QActionGroup;
class QButtonGroup;
class QCheckBox;
class QDoubleSpinBox;
class QLabel;
class QMenu;
class QSlider;
class QTabWidget;
class QToolBar;
class SoQtThumbWheel;
class SoQtPreferenceDialog;

class SOQT_EXPORT SoQtFullViewer : public SoQtViewer {
    Q_OBJECT

  public:
    // This specifies what should be build by default in the constructor
    enum BuildFlag {
	BUILD_NONE	    = 0x00, 
	BUILD_DECORATION    = 0x01, 
	BUILD_POPUP	    = 0x02, 
	BUILD_ALL	    = 0xff
    };
    
    //
    // Show/hide the viewer component trims (default ON)
    //
    void    	    setDecoration(SbBool onOrOff);
    SbBool  	    isDecoration()	    { return decorationFlag; }
    
    //
    // Enable/disable the popup menu (default enabled)
    //
    void    	    setPopupMenuEnabled(SbBool trueOrFalse);
    SbBool  	    isPopupMenuEnabled()    { return popupEnabled; }
    
    //
    // Add/remove push buttons for the application, which will be placed 
    // in the left hand side decoration trim.
    // The add() method appends the button to the end of the list, while 
    // insert() places the button at the specified index (starting at 0).
    //
	QWidget* getAppPushButtonParent() const { return appButtonForm; }
    void        addAppPushButton(QAbstractButton* newButton);
    void        insertAppPushButton(QAbstractButton* newButton, int index);
    void	    removeAppPushButton(QAbstractButton* oldButton);
    int         findAppPushButton(QAbstractButton* oldButton)
				{ return appButtonList->find(oldButton); }
    int         lengthAppPushButton()
				{ return appButtonList->getLength(); }
    
    QWidget*    getRenderAreaWidget()   { return raWidget; }
    
    // redefine these from the base class

public slots:
    virtual void    setViewing(SbBool onOrOff);
    virtual void    setHeadlight(SbBool onOrOff);
    virtual void    setFullScreen(bool enable);
    virtual void    setDrawStyle(SoQtViewer::DrawType type,
                        SoQtViewer::DrawStyle style);
    virtual void    setCamera(SoCamera *cam);

    void showPreferences();

protected slots:
    void setSeekModeCB(bool state) { setSeekMode(true); }
    void setViewingCB(bool state) { setViewing(state); }
    void setDecorationCB(bool state) { setDecoration((SbBool)state); }
    void setHeadlightCB(bool state) { setHeadlight((SbBool)state); }
    void setPickModeCB() { setViewing    (false); }
    void setViewModeCB() { setViewing    (true); }

protected:
    // Constructor/Destructor
    SoQtFullViewer(
	QWidget* parent,
	const char *name, 
	SbBool buildInsideParent, 
	SoQtFullViewer::BuildFlag flag, 
	SoQtViewer::Type type, 
	SbBool buildNow);
    ~SoQtFullViewer();

    virtual void    setSeekMode(SbBool onOrOff);

    // general decoration vars
    SbBool       decorationFlag;
    QGridLayout *mgrLayout;
    QWidget     *mgrWidget; // form which manages all other widgets
    QWidget     *raWidget;  // render area widget
    QToolBar    *leftTrimForm, *bottomTrimForm, *rightTrimForm;

    // thumb wheel vars
    SoQtThumbWheel     *rightWheel, *bottomWheel, *leftWheel;
    QString         rightWheelStr, bottomWheelStr, leftWheelStr;
    float   	    rightWheelVal, bottomWheelVal, leftWheelVal;
    QLabel          *rightWheelLabel, *bottomWheelLabel, *leftWheelLabel;

	// widget list of viewer buttons

    // The button widget should be used as the parent widget
    // when creating new buttons
    QWidget*        getButtonWidget() const { return appButtonForm; }

    // popup menu vars
	SbBool  	    popupEnabled;
    QPointer<QMenu> mainPopup, funcPopup, drawPopup;
    QMenu          *clientPopup;
    QString         popupTitle;
	
    QAction		   *viewingAction;
    QAction        *IDM_MPOPUP_DECORAT;
    QAction        *IDM_MPOPUP_HLIGHT;
    QAction        *IDM_MPOPUP_FULLSCREEN;

    QAction        *IDM_DPOPUP_ASIS;
    QAction        *IDM_DPOPUP_HLINE;
    QAction        *IDM_DPOPUP_NOTEX;
    QAction        *IDM_DPOPUP_LOWRES;
    QAction        *IDM_DPOPUP_WIRE;
    QAction        *IDM_DPOPUP_POINTS;
    QAction        *IDM_DPOPUP_BBOX;

    QAction        *IDM_DPOPUP_MSAMEAS;
    QAction        *IDM_DPOPUP_MNOTEX;
    QAction        *IDM_DPOPUP_MLOWRES;
    QAction        *IDM_DPOPUP_MWIRE;
    QAction        *IDM_DPOPUP_MLRWIRE;
    QAction        *IDM_DPOPUP_MPOINTS;
    QAction        *IDM_DPOPUP_MLRPOINTS;
    QAction        *IDM_DPOPUP_MBBOX;

    QAction        *IDM_DPOPUP_SBUFFER;
    QAction        *IDM_DPOPUP_DBUFFER;
    QAction        *IDM_DPOPUP_IBUFFER;
    
    //
    // Build/destroy decoration routines
    //
    QWidget*          buildWidget(QWidget *parent);
    virtual void      buildDecoration(QWidget* parent);
    virtual QToolBar* buildLeftTrim  (QWidget* parent);
    virtual QToolBar* buildBottomTrim (QWidget* parent);
    virtual QToolBar* buildRightTrim (QWidget* parent);


    virtual void    buildAppButtons(QWidget* parent);
    virtual void    createViewerButtons (QToolBar* parent);

    // viewer toolbar actions
    QAction        *pickAction;
    QAction        *viewAction;
    QAction        *seekAction;
    QAction        *gotoHomeAction;
    QAction        *setHomeAction;
    QAction        *viewAllAction;
	
    //
    // popup menu build routines
    //
    virtual void  	buildPopupMenu();

    void            setPopupMenuString (const char *name);
    QMenu*          buildFunctionsSubmenu(QMenu* popup);
    QMenu*          buildDrawStyleSubmenu(QMenu* popup);
    void            contextMenuEvent(QContextMenuEvent *event);
	
    //
    // Preference sheet build routines
    //
    void            setPrefSheetString(const char *name);
    virtual void    createPrefSheet(QTabWidget*){}

    // Subclasses SHOULD redefine these to do viewer specific tasks
    // since these do nothing in the base class. those routines are 
    // called by the thumb wheels whenever they rotate.
    virtual void    	rightWheelMotion(float);
    virtual void    	bottomWheelMotion(float);
    virtual void    	leftWheelMotion(float);
protected slots:    
    // Subclasses can redefine these to add viewer functionality. They
    // by default call start and finish interactive viewing methods.
    // (defined in the base class).
    virtual void    	rightWheelStart();
    virtual void    	bottomWheelStart();
    virtual void    	leftWheelStart();
    virtual void    	rightWheelFinish();
    virtual void    	bottomWheelFinish();
    virtual void    	leftWheelFinish();
    
    // Subclasses SHOULD set those wheel string to whatever functionality
    // name they are redefining the thumb wheels to do. Default names are
    // "Motion X, "Motion Y" and "Motion Z" for bottom, left and right wheels.
    void		setBottomWheelString(const char *name);
    void		setLeftWheelString(const char *name);
    void		setRightWheelString(const char *name);
    
    // Subclasses SHOULD redefine the openViewerHelpCard() routine to bring 
    // their help card (called by push button and popup menu entry).
    // They can simply call SoQtComponent::openHelpCard() to open their
    // specific help card.
    virtual void	openViewerHelpCard();

protected:
    
    // app button vars
    QWidget        *appButtonForm;
    SbPList        *appButtonList;


    // viewer button container
    QToolBar       *viewerButtonBar;

    // pref sheet variables
    SoQtPreferenceDialog *prefSheetShellWidget;
    QString         prefSheetStr;

    virtual void setCameraZoom (float zoom);
	float	    getCameraZoom();
	
    // callback functions for thumbwheels
    Q_SLOT void    rightWheelCB(float);
    Q_SLOT void    bottomWheelCB(float);
    Q_SLOT void    leftWheelCB(float);

    friend class SoQtPreferenceDialog;
};


#endif  /* _SO_QT_FULL_VIEWER_ */
