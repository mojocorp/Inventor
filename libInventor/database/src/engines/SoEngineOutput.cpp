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
 |	SoEngine, SoEngineOutput
 |
 |   Author(s)		: Gavin Bell
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

#include <Inventor/engines/SoEngineOutput.h>
#include <Inventor/engines/SoEngine.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/engines/SoOutputData.h>
#include <Inventor/fields/SoField.h>
#include <Inventor/engines/SoFieldConverter.h>

//////////////////////////////////////////////////////////////////////
//
//	SoEngineOutput class
//
//////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Constructor.  EngineOutputs may only be constructed inside
//    SoEngine instances.
//
// Use: internal

SoEngineOutput::SoEngineOutput()
//
////////////////////////////////////////////////////////////////////////
{
    enabled = TRUE;
    container = NULL;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Destructor.  Called automatically when SoEngine destructor is
//    called.
//
// Use: internal

SoEngineOutput::~SoEngineOutput()
//
////////////////////////////////////////////////////////////////////////
{
    // A engine should only be deleted once it has nothing connected
    // to its outputs.  So, in the DEBUG case, we'll make sure there
    // are no more connections, and complain if there are.
#ifdef DEBUG
    if (getNumConnections() != 0)
    SoDebugError::post("SoEngineOutput::~SoEngineOutput",
               "Engine with output connections deleted.\n"
               "Did you unref() an engine that you didn't ref()?");
#endif /* DEBUG */
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Return the type of SoField this output can write to.
//
// Use: public

SoType
SoEngineOutput::getConnectionType() const
//
////////////////////////////////////////////////////////////////////////
{
    // The connection type is stored in our container's
    // static EngineOutputData member (because it is the same for
    // all engine instances).
#ifdef DEBUG
    if (container == NULL) {
    SoDebugError::post("SoEngineOutput::getConnectionType",
               "container is NULL!");
    return SoType::badType();
    }
#endif /* DEBUG */

    const SoEngineOutputData *od = container->getOutputData();

#ifdef DEBUG
    if (od == NULL) {
    SoDebugError::post("SoEngineOutput::getConnectionType",
               "container has no output data!");
    return SoType::badType();
    }
#endif /* DEBUG */

    return od->getType(od->getIndex(container, this));
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Returns the number of fields this output is writing to, and
//    adds pointers to those fields to the given list:
//
// Use: public

int
SoEngineOutput::getForwardConnections(SoFieldList &list) const
//
////////////////////////////////////////////////////////////////////////
{
    int numConnections = 0;

    for (int i = 0; i < connections.getLength(); i++) {
    SoField	*field = connections[i];

    // Skip over converter, if any
    SoFieldContainer *container = field->getContainer();
    if (container->isOfType(SoFieldConverter::getClassTypeId()))
        numConnections += ((SoFieldConverter *) container)->
        getForwardConnections(list);

    else {
        list.append(field);
        numConnections++;
    }
    }

    return numConnections;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Enables or disables all connections from this
//    output. The enabled flag is checked at SO_ENGINE_OUTPUT time and
//    at notify() to prevent notification/evaluation through disabled
//    outputs.
//
// Use: public

void
SoEngineOutput::enable(SbBool flag)
//
////////////////////////////////////////////////////////////////////////
{
    if (enabled != flag) {
    enabled = flag;

    // Notify if re-enabling connections
    if (flag) {

        // A very annoying double notification occurs with engines
        // that enable their outputs during inputChanged that we
        // prevent by not bothering to start notification if we're
        // already in the middle of notification:
        const SoEngine *e = getContainer();
        if (e && e->isNotifying()) return;

        for (int j = 0; j < getNumConnections(); j++) {
        SoField *f = (*this)[j];

        if (!f->flags.isEngineModifying) {
            f->startNotify();
        }
        }
    }
    }
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Called by SoField connection code to add a connection.  Can pass
//    NULL to just mark engine as needing evaluation.
//
// Use: internal

void
SoEngineOutput::addConnection(SoField *field)
//
////////////////////////////////////////////////////////////////////////
{
    if (field) {
    // Add to lists of connections
    connections.append(field);

    // Increment containing engine's reference count
    container->ref();
    }

    // This forces the engine to write to the new connection.
    container->needsEvaluation = TRUE;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Called by SoField::disconnect() to remove a connection.
//
// Use: internal

void
SoEngineOutput::removeConnection(SoField *field)
//
////////////////////////////////////////////////////////////////////////
{
    int index = connections.find(field);
#ifdef DEBUG
    if (index == -1)
    SoDebugError::post("SoEngineOutput::removeConnection",
               "Field is not connected!");
#endif /* DEBUG */

    connections.remove(index);

    // Decrement reference count of containing engine
    container->unref();
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Prepare for writing into fields.
//
// Use: internal

void
SoEngineOutput::prepareToWrite() const
//
////////////////////////////////////////////////////////////////////////
{
    for (int i = connections.getLength()-1; i >= 0; i--) {
    SoField *f = connections.get(i);
#ifdef DEBUG
    if (f->flags.isEngineModifying) {
        SoDebugError::post("SoEngineOutput::prepareToWrite",
                   "Internal field flags are wrong; "
                   "did you call engine->evaluate() "
                   "instead of engine->evaluateWrapper?");
    }
#endif
    f->flags.isEngineModifying = TRUE;
    }
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Done writing, reset notification flags.
//
// Use: internal

void
SoEngineOutput::doneWriting() const
//
////////////////////////////////////////////////////////////////////////
{
    for (int i = connections.getLength()-1; i >= 0; i--) {
    SoField *f = connections.get(i);
    f->flags.isEngineModifying = FALSE;
    }
}
