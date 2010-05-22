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
 |   $Revision: 1.2 $
 |
 |   Description:
 |	This is an abstract base class which defines a c++ protocol
 |	for motif components. 
 |
 |   Author(s): David Mott, Alain Dumesny
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

/*
 * Ported to Qt by Morgan Leborgne, 2010
 */

#ifndef _SO_QT_COMPONENT_H_
#define _SO_QT_COMPONENT_H_

#include <QObject>

#include <Inventor/Qt/SoQt.h>
#include <Inventor/SbLinear.h>
#include <Inventor/SbString.h>

class QWidget;
class SoQtComponent;
class SoCallbackList;

typedef void SoQtComponentCB(void *userData, SoQtComponent *comp);
typedef void SoQtComponentVisibilityCB(void *userData, SbBool visibleFlag);

class SOQT_EXPORT SoQtComponent : public QObject {
  public:
    //
    // This shows and hides the component. If this is a topLevelShell 
    // component, then show() will Realize and Map the window, else it
    // will simply Manage the widget.
    // Hide calls the appropriate XUnmapWindow() or XtUnmanageChild() 
    // routines (check SoQt::show and hide man pages for detail).
    //
    // In addition, show() will also pop the component window to the top
    // and de-iconify if necessary, to make sure the component is visible
    // by the user.
    //
    virtual void 	show();
    virtual void 	hide();
    
    //
    // returns TRUE if this component is mapped onto the screen. For
    // a component to be visible, it's widget and the shell containing
    // this widget must be mapped (which is FALSE when the component
    // is iconified).
    //
    // Subclasses should call this routine before redrawing anything
    // and in any sensor trigger methods. Calling this will check the 
    // current visibility (which is really cheap) and invoke the
    // visibility changed callbacks if the state changes (see
    // addVisibilityChangeCallback()).
    //
    SbBool   		isVisible();
    
    //
    // This returns the base widget for this component.
    // If the component created its own shell, this returns the 
    // topmost widget beneath the shell.
    // Call getShellWidget() to obtain the shell.
    //
    QWidget*	getWidget() const { return _baseWidget; }
    QWidget*	baseWidget() const { return getWidget(); }
    
    //
    // Returns TRUE if this component is a top level shell component (has its
    // own window). Subclasses may use this to decide if they are allowed to
    // resize themselfves.
    // Also method to return the shell widget (NULL if the shell hasn't been
    // created by this component).
    //
    SbBool		isTopLevelShell() const	    { return topLevelShell; }
    QWidget*	getShellWidget() const
			    { return topLevelShell ? parentWidget : NULL; }

    // Return the parent widget, be it a shell or not
    QWidget*	getParentWidget() const	    { return parentWidget; }
    
    //
    // Convenience routines on the widget - setSize calls XtSetValue.
    //
    void		setSize(const SbVec2s &size);
    SbVec2s		getSize();
    
    virtual SbBool setFullScreen( const SbBool enable );
    SbBool isFullScreen() const;
    void setFullScreenEnable( const SbBool enable ) { fullScreenEnabled = enable; }
    SbBool isFullScreenEnable() const { return fullScreenEnabled; }

 	//
    // The title can be set for topLevel shell components or components
    // which are directly under a shell widget (i.e. components which 
    // have their own window).
    //
    void    	    	setTitle(const char *newTitle);
    const char *    	getTitle() const		{ return title.getString(); }
    void    	    	setIconTitle(const char *newIconTitle);
    const char *    	getIconTitle() const		{ return iconTitle.getString(); }
 	//
    // Widget name and class - these are used when looking up X resource
    // values for the widget (each subclass provides a className).
    //
    const SbString	getWidgetName() const	{ return _name; }    
    const SbString	getClassName() const { return _classname; }
    
    virtual ~SoQtComponent();
    
  protected:
    //
    // If `parent` widget is suplied AND `buildInsideParent` is TRUE, this
    // component will build inside the given parent widget, else
    // it will create its own topLevelShell widget (component resides in 
    // its own window).
    // The topLevelShell can either be created under the given
    // parent shell (`parent` != NULL) or under the main window.
    //
    // The name is used for looking up X resource values. If NULL,
    // then this component inherits resource values defined for its class.
    //
    // Calling setBaseWidget is needed for looking up Xt like 
    // resources in the widget tree. It will use the class name of
    // the Inventor component (e.g. SoQtRenderArea) instead of
    // the class name of the Motif widget this component employs
    // (e.g. XmForm).
    // 
    // Thus apps are able to do this in their app-defaults file:
    //
    // *SoQtRenderArea*BackgroundColor: salmon
    //
    SoQtComponent(
	    QWidget *parent = NULL,
	    const char *name = NULL, 
	    SbBool buildInsideParent = TRUE);

    // Subclasses need to call this method passing the top most
    // widget after it has been created.
    void		setBaseWidget(QWidget *w);

    // Subclasses need to set the class name in the constructor
    // before the widget is built.
    void		setClassName(const char *n) { _classname = n; }
    //
    // This method can be used by subclasses to open a component help
    // card. The name of the file should be supplied withought a path
    // name. By default the file will be searched using:
    //	    1) current working directory
    //	    2) SO_HELP_DIR environment variable
    //	    3) $(IVPREFIX)/share/help/Inventor
    //	    4) else bring a no help card found dialog
    //
    void    openHelpCard(const char *cardName);
    
    // For fullscreen mode.
    SbBool  wasNotTopLevel;
  private:
    
    SbBool	topLevelShell;	// TRUE if in its own window
    SbBool	createdShell;	// TRUE if we created that toplevel shell
    QWidget	   *parentWidget;	// topLevel shell if in its own window
    QWidget	   *_baseWidget;	// returned by getWidget()
    SbString	_name;		// name of this widget
    SbString	title;		// title for window if in its own window
    SbString	iconTitle;	// title for icon if in its own window
    SbVec2s	size;		// size of the '_baseWidget' and 'shell' (if toplevel)
    SbString	_classname;
    
    // visibiltity stuff
    SbBool	    visibleState;
    SbBool      fullScreenEnabled;
};

#endif // _SO_QT_COMPONENT_H_
