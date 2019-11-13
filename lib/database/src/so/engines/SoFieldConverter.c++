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
 |      SoFieldConverter
 |	    *** NOTE:  SoFieldConverter::initClasses()        ***
 |	    ***        is defined in SoFieldConverters.c++    ***
 |	    ***        rather than in this file.              ***
 |
 |   Author(s)          : Paul S. Strauss
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

#include <Inventor/fields/SoField.h>
#include <Inventor/engines/SoFieldConverter.h>

SO_ENGINE_ABSTRACT_SOURCE(SoFieldConverter);

////////////////////////////////////////////////////////////////////////
//
// Description:
//    This initializes the SoFieldConverter class.
//
// Use: internal

void
SoFieldConverter::initClass()
//
////////////////////////////////////////////////////////////////////////
{
    SO__ENGINE_INIT_ABSTRACT_CLASS(SoFieldConverter, "FieldConverter",
                                   SoEngine);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Constructor
//
// Use: public

SoFieldConverter::SoFieldConverter()
//
////////////////////////////////////////////////////////////////////////
{
    SO_ENGINE_CONSTRUCTOR(SoFieldConverter);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Destructor
//
// Use: private

SoFieldConverter::~SoFieldConverter()
//
////////////////////////////////////////////////////////////////////////
{}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Searches through the field data to find a field that has a
//    connection.
//
// Use: public

SoField *
SoFieldConverter::getConnectedInput()
//
////////////////////////////////////////////////////////////////////////
{
    const SoFieldData *fd = getFieldData();
    for (int i = 0; i < fd->getNumFields(); i++) {
        SoField *f = fd->getField(this, i);
        if (f->isConnected())
            return f;
    }
    return 0;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Adds all fields connected from the output of the converter to
//    the given list.
//
// Use: public

int
SoFieldConverter::getForwardConnections(SoFieldList &list) const
//
////////////////////////////////////////////////////////////////////////
{
    SoEngineOutputList outputs;
    int                numConnections = 0;

    // Get all of the outputs into a list
    getOutputs(outputs);

    // For each output, add all connections to the given list
    for (int i = 0; i < outputs.getLength(); i++)
        numConnections += outputs[i]->getForwardConnections(list);

    return numConnections;
}
