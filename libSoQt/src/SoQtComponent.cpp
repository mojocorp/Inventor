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
 |   $Revision: 1.6 $
 |
 |   Classes:
 |	SoQtComponent
 |
 |   Author(s): David Mott, Alain Dumesny
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

/*
 * Ported to Qt by Morgan Leborgne, 2010
 */

#include <QWidget>

#include <Inventor/SbDict.h>
#include <Inventor/misc/SoCallbackList.h>
#include <Inventor/Qt/SoQtComponent.h>
#include <Inventor/Qt/SoQt.h>

#include <Inventor/errors/SoDebugError.h>


static const char *helpDialogTitle = "Help Card Error Dialog";
static const char *helpCardError = "Inventor Help Cards not installed.";
static const char *helpPrgError = "acroread not installed on this system.";
static const char *thisClassName = "SoQtComponent";
////////////////////////////////////////////////////////////////////////
//
// Constructor
//
SoQtComponent::SoQtComponent(
    QWidget *parent,
    const char *name, 
    SbBool buildInsideParent)
//
////////////////////////////////////////////////////////////////////////
{
    setClassName(thisClassName);

    _name = name;

    size.setValue(0, 0);
    // This is set by subclasses through setBaseWidget()
    _baseWidget = NULL;
    // create a shell window if necessary
    createdShell = (parent == NULL || !buildInsideParent);
    topLevelShell = (createdShell || (parent != NULL && parent->isWindow()));
 	if (createdShell) {
		QWidget* parentShell = (parent != NULL) ? parent : NULL;
		parentWidget = new QWidget(parent);
		parentWidget->setObjectName(getWidgetName().getString());
    }
    else parentWidget = parent;
}

////////////////////////////////////////////////////////////////////////
//
// Destructor
//
SoQtComponent::~SoQtComponent()
//
////////////////////////////////////////////////////////////////////////
{

}

////////////////////////////////////////////////////////////////////////
//
// Set the base widget and install the destroy handler.
//
SbBool
SoQtComponent::isVisible()
//
////////////////////////////////////////////////////////////////////////
{
    if (_baseWidget != NULL)
		visibleState = _baseWidget->isVisible();
    
    return visibleState;
}

////////////////////////////////////////////////////////////////////////
//
// Set the base widget and install the destroy handler.
//
void
SoQtComponent::setBaseWidget(QWidget *w)
//
////////////////////////////////////////////////////////////////////////
{ 
    _baseWidget = w;

}

////////////////////////////////////////////////////////////////////////
//
// Resizes the outer most widget.
//
// Use: public
void
SoQtComponent::setSize(const SbVec2s &newSize)
//
////////////////////////////////////////////////////////////////////////
{
	if (_baseWidget != NULL)
    	_baseWidget->resize(newSize[0], newSize[1]);
    // cache value for later builds
    size = newSize;
}

////////////////////////////////////////////////////////////////////////
//
// Returns the outermost widget size.
//
// Use: public
SbVec2s
SoQtComponent::getSize()
//
////////////////////////////////////////////////////////////////////////
{
	if (_baseWidget != NULL)
    	size.setValue(_baseWidget->width(), _baseWidget->height());

    return size;
}

void
SoQtComponent::setFullScreen( bool enable )
{
    enable = enable && fullScreenEnabled;

    if (_baseWidget) {
        if ((_baseWidget->windowState() & Qt::WindowFullScreen) == enable)
            return;

        if (enable) {
            wasNotTopLevel = !_baseWidget->isWindow();
            _baseWidget->setWindowFlags(_baseWidget->windowFlags () | Qt::Window);
            _baseWidget->showFullScreen();
        } else {
            if (wasNotTopLevel)
                _baseWidget->setWindowFlags(_baseWidget->windowFlags () & ~Qt::Window);

            _baseWidget->showNormal();
        }
    }
}

SbBool
SoQtComponent::isFullScreen() const
{
    if (_baseWidget)
        return _baseWidget->isFullScreen();
    return FALSE;
}
////////////////////////////////////////////////////////////////////////
//
// set the title if the widget is directly under a shell widget
//
void
SoQtComponent::setTitle(const char *newTitle)
//
////////////////////////////////////////////////////////////////////////
{
    title = newTitle;
    
    if (_baseWidget != NULL)
	_baseWidget->setWindowTitle(title.getString());
}

////////////////////////////////////////////////////////////////////////
//
// set the icon title if the widget is directly under a shell widget
//
void
SoQtComponent::setIconTitle(const char *newTitle)
//
////////////////////////////////////////////////////////////////////////
{
    iconTitle = newTitle;
}

////////////////////////////////////////////////////////////////////////
//
// show - display the component.
//
void
SoQtComponent::show()
//
////////////////////////////////////////////////////////////////////////
{
    // Display that widget!
    if (_baseWidget)
	 _baseWidget->show();
}

////////////////////////////////////////////////////////////////////////
//
// hide - hide the component.
//
void
SoQtComponent::hide()
//
////////////////////////////////////////////////////////////////////////
{
    if (_baseWidget)
	 _baseWidget->hide();

}

////////////////////////////////////////////////////////////////////////
//
// Description:
//	This routine is called by subclasses to open the specific help
//  card given the file name. By default the file will be searched using:
//	    1) current working directory
//	    2) SO_HELP_DIR environment variable
//	    3) $(IVPREFIX)/share/help/Inventor
//	    4) else bring a "No help card found message"
//
// Use: protected
//
void
SoQtComponent::openHelpCard(const char *cardName)
//
////////////////////////////////////////////////////////////////////////
{

}
