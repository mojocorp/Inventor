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
 * Copyright (C) 1990,91,92   Silicon Graphics, Inc.
 *
 _______________________________________________________________________
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 |
 |   $Revision: 1.1 $
 |
 |   Classes:
 |	SoQtMouse
 |
 |   Author(s): David Mott
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

/*
 * Ported to Qt by Morgan Leborgne, 2010
 */

#include <Inventor/SbTime.h>
#include <Inventor/Qt/devices/SoQtMouse.h>

////////////////////////////////////////////////////////////////////////
//
// Description:
//   Constructor.
//
// public
//
SoQtMouse::SoQtMouse()
//
////////////////////////////////////////////////////////////////////////
{
    // allocate the SoEvents we support here
    loc2Event = new SoLocation2Event;
    buttonEvent = new SoMouseButtonEvent;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//   Destructor.
//
// public
//
SoQtMouse::~SoQtMouse()
//
////////////////////////////////////////////////////////////////////////
{
    delete loc2Event;
    delete buttonEvent;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//   This selects input for all mouse device events which occur in w.
// The callback routine is proc, and the callback data is clientData.
//
// virtual public
//
#if 0
void
SoQtMouse::enable(
    Widget w,
    XtEventHandler proc, 
    XtPointer clientData,
    Window)
//
////////////////////////////////////////////////////////////////////////
{
    XtAddEventHandler(w, eventMask, FALSE, proc, clientData);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//   This unselects input for all mouse device events which occur in w,
// i.e. mouse events will no longer be recognized.
//
// virtual public
//
void
SoQtMouse::disable(
    Widget w,
    XtEventHandler proc, 
    XtPointer clientData)
//
////////////////////////////////////////////////////////////////////////
{
    XtRemoveEventHandler(w, eventMask, FALSE, proc, clientData);
}
#endif
////////////////////////////////////////////////////////////////////////
//
// Description:
//   This returns an SoEvent for the passed Qt event, if the event
// was generated by the mouse device.
//
// static public
//
const SoEvent *
SoQtMouse::translateEvent(QEvent *qe)
//
////////////////////////////////////////////////////////////////////////
{
    SoEvent *event = NULL;
    
    // switch on event type
    switch (qe->type()) {
	case QEvent::MouseButtonPress:
    case QEvent::MouseButtonDblClick:
	    event = translateButtonEvent((QMouseEvent *) qe, SoButtonEvent::DOWN);
	    break;
	    
	case QEvent::MouseButtonRelease:
	    event = translateButtonEvent((QMouseEvent *) qe, SoButtonEvent::UP);
	    break;
	    
	case QEvent::MouseMove:
	    event = translateMotionEvent((QMouseEvent *) qe);
	    break;
	    
	default:
	    break;
    }
    
    return event;
}

SoMouseButtonEvent *
SoQtMouse::translateButtonEvent(QMouseEvent *be,
				SoButtonEvent::State whichState)
{
    SoMouseButtonEvent::Button whichButton;
    switch (be->button()) {
    case Qt::LeftButton:
        whichButton = SoMouseButtonEvent::BUTTON1;
        break;
    case Qt::MidButton:
        whichButton = SoMouseButtonEvent::BUTTON2;
        break;
    case Qt::RightButton:
        whichButton = SoMouseButtonEvent::BUTTON3;
        break;
    default:
        whichButton = SoMouseButtonEvent::ANY;
    }

    fillInEventState (buttonEvent, be);

    buttonEvent->setState (whichState);
    buttonEvent->setButton (whichButton);

    return buttonEvent;
}

SoLocation2Event *
SoQtMouse::translateMotionEvent(QMouseEvent *me)
{
    fillInEventState (loc2Event, me);

    return loc2Event;
}



