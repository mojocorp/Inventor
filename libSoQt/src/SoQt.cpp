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
 |   $Revision: 1.3 $
 |
 |   Classes:
 |	SoQt
 |	SoQtEventHandler
 |
 |   Author(s): David Mott, Nick Thompson, Paul Isaacs, Alain Dumesny
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

/*
 * Ported to Qt by Morgan Leborgne, 2010
 */


#include <QObject>
#include <QTimerEvent>
#include <QApplication>

#include <Inventor/errors/SoDebugError.h>
#include <Inventor/SoDB.h>
#include <Inventor/SoInteraction.h>
#include <Inventor/nodekits/SoNodeKit.h>
#include <Inventor/sensors/SoSensorManager.h>
#include <Inventor/Qt/SoQt.h>

// this class ties Inventor sensors into Xt events
class SoQtEventHandler : public QObject {
  public:
    SoQtEventHandler();

  protected:
    virtual void timerEvent( QTimerEvent * event );

  private:
    bool        timerOn;
    int         qtTimer;
    SbTime      currentDeadline;

    bool        workProcOn;
    int         qtWorkProc;

    // Callback used by the timer
    void        qtTimerCallback();

    // Callback used by the workproc
    void        qtWorkProcCallback();

    // Callback for changes to the sensor manager
    static void sensorManagerChangedCallback (void *data);

    void        setUpCallbacks();
};

// static members
bool                SoQt::initialized = false;
SoQtEventHandler *  SoQt::eventHandler = NULL;


void
SoQt::init (const char* /*appName*/, const char* /*className*/)
{
    // see if we are already initialized!
    if (initialized) {
        return;
    }
    initialized = true;

    // init Inventor
    SoDB::init();
    SoNodeKit::init();
    SoInteraction::init();

    // bind Inventor with Qt
    eventHandler = new SoQtEventHandler();
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//   This retrieves and dispatches events (it loops forever!)
//
// static public
//
int
SoQt::mainLoop()
//
////////////////////////////////////////////////////////////////////////
{
    return QApplication::exec();
}
//----------------------------------------------------------------------
//
//  SoQtEventHandler - this class ties in sensors to Qt events
//
//----------------------------------------------------------------------

SoQtEventHandler::SoQtEventHandler()
{
    qtTimer     = -1;
    qtWorkProc  = -1;

    SoDB::getSensorManager()->setChangedCallback
        (SoQtEventHandler::sensorManagerChangedCallback,
         this);
    // Call this once because things might already be in the timer queue.
    // If we don't call this and no new things happen, then the callbacks
    // will never be set up.
    setUpCallbacks();
}

void 
SoQtEventHandler::timerEvent( QTimerEvent * event )
{
    if (event->timerId()==qtTimer)
        qtTimerCallback();
    
    if (event->timerId()==qtWorkProc)
        qtWorkProcCallback();
}

void
SoQtEventHandler::setUpCallbacks()
{
    SbTime	nextEvent;

    // If we have a timer at some point, schedule that
    if (SoDB::getSensorManager()->isTimerSensorPending(nextEvent)) {
	// Only change the timer if its deadline is different from the
	// current one.
        if (qtTimer==-1 || nextEvent != currentDeadline) {
            currentDeadline = nextEvent;
            long msec = (nextEvent - SbTime::getTimeOfDay()).getMsecValue();
            if (msec < 0) { msec = 0; }
            qtTimer = startTimer(msec);
        }
    } else if (qtTimer!=-1) {           // get rid of existing timer
        killTimer(qtTimer);
        qtTimer = -1;
    }

    // If we have an idle task, schedule a workproc for it
    if (SoDB::getSensorManager()->isDelaySensorPending()) {
	// only schedule it if one is not already active
        if (qtWorkProc==-1) {
            qtWorkProc = startTimer (0);
        }
    }
    else if (qtWorkProc!=-1) {          // remove obsolete workproc
        killTimer(qtWorkProc);
        qtWorkProc = -1;
    }
}

void
SoQtEventHandler::qtTimerCallback()
{
    // process the timer queue
    SoDB::getSensorManager()->processTimerQueue();
    setUpCallbacks();
}

void
SoQtEventHandler::qtWorkProcCallback()
{
    // process the idle queue
    SoDB::getSensorManager()->processDelayQueue(TRUE);
    setUpCallbacks();
}

// Callback for changes to the sensor manager
void
SoQtEventHandler::sensorManagerChangedCallback (void *data)
{
    SoQtEventHandler *eh = (SoQtEventHandler *) data;

    // update the Qt timeout and workproc if necessary
    eh->setUpCallbacks();
}

