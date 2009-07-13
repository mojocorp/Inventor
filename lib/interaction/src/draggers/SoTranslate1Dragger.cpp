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
 |   $Revision: 1.1 $
 |
 |   Classes:
 |	SoTranslate1Dragger
 |
 |   Author(s): Paul Isaacs
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */


#include <stdio.h>
#include <Inventor/SbLinear.h>
#include <Inventor/SoDB.h>
#include <Inventor/sensors/SoFieldSensor.h>
#include <Inventor/SoInput.h>
#include <Inventor/SoPath.h>
#include <Inventor/projectors/SbLineProjector.h>
#include <Inventor/draggers/SoTranslate1Dragger.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoSwitch.h>

#include "geom/SoTranslate1DraggerGeom.h"


SO_KIT_SOURCE(SoTranslate1Dragger);

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Constructor
//
SoTranslate1Dragger::SoTranslate1Dragger()
//
////////////////////////////////////////////////////////////////////////
{
    SO_KIT_CONSTRUCTOR(SoTranslate1Dragger);

    isBuiltIn = TRUE;

    // Put this stuff under the geomSeparator so it will draw more
    // efficiently.
    SO_KIT_ADD_CATALOG_ENTRY(translatorSwitch, SoSwitch, TRUE,
				geomSeparator,\x0,FALSE);
    SO_KIT_ADD_CATALOG_ENTRY(translator, SoSeparator, TRUE,
				translatorSwitch,\x0,TRUE);
    SO_KIT_ADD_CATALOG_ENTRY(translatorActive, SoSeparator, TRUE,
				translatorSwitch,\x0,TRUE);
    SO_KIT_ADD_CATALOG_ENTRY(feedbackSwitch, SoSwitch, TRUE,
				geomSeparator,\x0,FALSE);
    SO_KIT_ADD_CATALOG_ENTRY(feedback, SoSeparator, TRUE,
				feedbackSwitch,\x0,TRUE);
    SO_KIT_ADD_CATALOG_ENTRY(feedbackActive, SoSeparator, TRUE,
				feedbackSwitch,\x0,TRUE);

    // read geometry for shared parts
    if (SO_KIT_IS_FIRST_INSTANCE())
	readDefaultParts("translate1Dragger.iv", geomBuffer, sizeof(geomBuffer) );

    SO_KIT_ADD_FIELD(translation, (0.0, 0.0, 0.0));

    SO_KIT_INIT_INSTANCE();

    // create the parts...
   setPartAsDefault("translator",      "translate1Translator");
   setPartAsDefault("translatorActive","translate1TranslatorActive");
   setPartAsDefault("feedback",        "translate1Feedback");
   setPartAsDefault("feedbackActive",  "translate1FeedbackActive");

    // Set the switches to 0...
    setSwitchValue( translatorSwitch.getValue(), 0 );
    setSwitchValue( feedbackSwitch.getValue(), 0 );

    // Create the line projector
    lineProj = new SbLineProjector();

    // add the callbacks to perform the dragging
    addStartCallback(  &SoTranslate1Dragger::startCB );
    addMotionCallback( &SoTranslate1Dragger::motionCB );
    addFinishCallback(   &SoTranslate1Dragger::finishCB );

    // Updates the translation field when the motionMatrix is set.
    addValueChangedCallback( &SoTranslate1Dragger::valueChangedCB );

    // Updates the motionMatrix when the translation field is set.
    fieldSensor = new SoFieldSensor( &SoTranslate1Dragger::fieldSensorCB, this);
    fieldSensor->setPriority( 0 );

    setUpConnections( TRUE, TRUE );
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Destructor
//
SoTranslate1Dragger::~SoTranslate1Dragger()
//
////////////////////////////////////////////////////////////////////////
{
    delete lineProj;
    if (fieldSensor)
        delete fieldSensor;
}

//    detach/attach any sensors, callbacks, and/or field connections.
//    Called by:            start/end of SoBaseKit::readInstance
//    and on new copy by:   start/end of SoBaseKit::copy.
//    Classes that redefine must call setUpConnections(TRUE,TRUE) 
//    at end of constructor.
//    Returns the state of the node when this was called.
SbBool
SoTranslate1Dragger::setUpConnections( SbBool onOff, SbBool doItAlways )
{
    if ( !doItAlways && connectionsSetUp == onOff)
	return onOff;

    if ( onOff ) {

	// We connect AFTER base class.
	SoDragger::setUpConnections( onOff, FALSE );

	// Call the sensor CBs to make things are up-to-date.
	fieldSensorCB( this, NULL );

	// Connect the field sensors
	if (fieldSensor->getAttachedField() != &translation)
	    fieldSensor->attach( &translation );
    }
    else {

	// We disconnect BEFORE base class.

	// Disconnect the field sensors.
	if (fieldSensor->getAttachedField())
	    fieldSensor->detach();

	SoDragger::setUpConnections( onOff, FALSE );
    }

    return !(connectionsSetUp = onOff);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Set up the highlighting, projector, and the initial hit on
//    the dragger
//
// Use: private
//
void
SoTranslate1Dragger::dragStart()
//
////////////////////////////////////////////////////////////////////////
{
    // Set the switches to 1...
    setSwitchValue( translatorSwitch.getValue(), 1 );
    setSwitchValue( feedbackSwitch.getValue(), 1 );

    // Establish the projector line in working space.
    // Working space is space at end of motion matrix.
    // translate direction is defined as (1,0,0) in local space.
	SbVec3f startLocalHitPt = getLocalStartingPoint();
	lineProj->setLine(SbLine(startLocalHitPt,
				 startLocalHitPt + SbVec3f(1,0,0)));
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Stretch the dragger according to the motion along the line
//    projector
//
// Use: private
//
void
SoTranslate1Dragger::drag()
//
////////////////////////////////////////////////////////////////////////
{
    // Set up the projector space and view.
    // Working space is space at end of motion matrix.
	lineProj->setViewVolume( getViewVolume() );    
	lineProj->setWorkingSpace( getLocalToWorldMatrix() );

    // Get newHitPt and startHitPt in workspace.
	SbVec3f newHitPt = lineProj->project( getNormalizedLocaterPosition()); 
	SbVec3f startHitPt = getLocalStartingPoint();

    // Figure out the translation relative to start position.
        SbVec3f motion = newHitPt - startHitPt;

    // Append this to the startMotionMatrix, which we saved at the beginning
    // of the drag, to find the current motion matrix.
	setMotionMatrix( appendTranslation( getStartMotionMatrix(), motion ) );
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//
// Use: private
//
void
SoTranslate1Dragger::dragFinish()
//
////////////////////////////////////////////////////////////////////////
{
    // Set the switches to 0...
    setSwitchValue( translatorSwitch.getValue(), 0 );
    setSwitchValue( feedbackSwitch.getValue(), 0 );
}    

////////////////////////////////////////////////////////////////////
//  Stubs for callbacks
////////////////////////////////////////////////////////////////////
void
SoTranslate1Dragger::startCB( void *, SoDragger *inDragger )
{
    SoTranslate1Dragger *m = (SoTranslate1Dragger *) inDragger;
    m->dragStart();
}

void
SoTranslate1Dragger::motionCB( void *, SoDragger *inDragger )
{
    SoTranslate1Dragger *m = (SoTranslate1Dragger *) inDragger;
    m->drag();
}

void
SoTranslate1Dragger::finishCB( void *, SoDragger *inDragger )
{
    SoTranslate1Dragger *m = (SoTranslate1Dragger *) inDragger;
    m->dragFinish();
}

void
SoTranslate1Dragger::valueChangedCB( void *, SoDragger *inDragger )
{
    SoTranslate1Dragger *m = (SoTranslate1Dragger *) inDragger;
    SbMatrix motMat = m->getMotionMatrix();

    SbVec3f    trans, scale;
    SbRotation rot, scaleOrient;
    getTransformFast( motMat, trans, rot, scale, scaleOrient );

    // Disconnect the field sensor
    m->fieldSensor->detach();

    if ( m->translation.getValue() != trans )
	m->translation = trans;

    // Reconnect the field sensor
    m->fieldSensor->attach( &(m->translation) );
}

void
SoTranslate1Dragger::fieldSensorCB( void *inDragger, SoSensor *)
{
    SoTranslate1Dragger *dragger = (SoTranslate1Dragger *) inDragger;

    // Incorporate the new field value into the matrix...
    SbMatrix motMat = dragger->getMotionMatrix();
    dragger->workFieldsIntoTransform( motMat );

    dragger->setMotionMatrix( motMat );
}
