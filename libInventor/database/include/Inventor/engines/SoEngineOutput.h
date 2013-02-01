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
 * Copyright (C) 1990,91   Silicon Graphics, Inc.
 *
 _______________________________________________________________________
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 |
 |   $Revision: 1.1 $
 |
 |   Description:
 | This file defines the base SoEngineOutput class.
 |
 |   Author(s)  : Gavin Bell
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

#ifndef  _SO_ENGINE_OUTPUT_
#define  _SO_ENGINE_OUTPUT_

#include <Inventor/lists/SoFieldList.h>
#include <Inventor/SoType.h>

class SoEngine;

/// Class for all engine outputs.
/// \ingroup Engines
/// <tt>SoEngineOuput</tt> is the class for all engine output fields.
/// There is no public constructor routine for this class.
/// Only the engine classes create instances of <tt>SoEngineOutput</tt>
///
///
/// Each engine creates one or more engine outputs.
/// The type of the output is documented in the engine reference pages.
/// There is also an <tt>SoEngineOutput</tt>  method for querying
/// the connection type.
///
///
/// The application can at any time enable or disable the engine outputs.
/// By default the engine outputs are enabled.
/// \sa SoEngine
class INVENTOR_API SoEngineOutput {

public:
    /// Returns the type of field this output can connect to.
    SoType getConnectionType() const;

    /// Returns the number of fields this output is writing to,
    /// and adds pointers to those fields to the given list.
    int getForwardConnections(SoFieldList &list) const;

    /// Enables or disables all connections from this ouptut. If the
    /// connections are disabled, values will not be output along them.
    /// By default, outputs are enabled.
    void enable(SbBool flag);

    /// Returns TRUE if this output is currently enabled.
    SbBool isEnabled() const {
        return enabled;
    }

    /// Returns containing engine.
    SoEngine * getContainer() const  {
        return container;
    }

SoINTERNAL public:
    SoEngineOutput();

    virtual ~SoEngineOutput();

    void setContainer(SoEngine *eng) {
        container = eng;
    }

    // Adds/removes connection to field
    void addConnection(SoField *);
    void removeConnection(SoField *);

    // Number of connections this output currently has
    int getNumConnections() const {
        return connections.getLength();
    }

    // Returns the fields this output is writing into
    SoField * operator[](int i) const {
        return connections.get(i);
    }

    // Before evaluating (which is done with the regular field API),
    // we must disable notification on all the fields we're about to
    // write into.  After evaluating, the bits are restored:
    void prepareToWrite() const;
    void doneWriting() const;

private:
    SbBool  enabled;
    SoFieldList  connections;
    SoEngine  *container;
};

#endif /* _SO_ENGINE_OUTPUT_ */

