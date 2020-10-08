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
 |   Classes:
 |	SoSFEngine
 |
 |   Author(s)		: Paul S. Strauss
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

#pragma once

class SoEngine;

#include <Inventor/fields/SoSubField.h>

/// Field containing a pointer to a engine.
/// \ingroup Fields
/// This field maintains a pointer to an <tt>SoEngine</tt> instance,
/// correctly maintaining its reference count.
///
/// <tt>SoSFEngines</tt> are written to file as the engine they are pointing to.
/// For example:
///
/// mySoSFEngineField ElapsedTime {}
///
/// is an SoSFEngine field named 'mySoSFEngineField', pointing to an
/// SoElapsedTime engine. If the engine is used elsewhere, the regular DEF/USE
/// instancing mechanism applies:
///
/// anotherSoSFEngineField USE calculator
///
/// is an SoSFEngine field that points to a engine named 'calculator' that was
/// DEF'ed earlier in the scene. \sa SoField, SoSField, SoMFEngine, SoEngine
class SoSFEngine : public SoSField {

    // Use standard field stuff
    SO_SFIELD_HEADER(SoSFEngine, SoEngine *, SoEngine *);

    SoINTERNAL
  public:
    static void initClass();

    // Update a copied field to use the copy of the engine if there is one
    virtual void fixCopy(SbBool copyConnections);

    // Override this to also check the stored engine
    virtual SbBool referencesCopy() const;

  private:
    // Override this to maintain write-references in engine
    virtual void countWriteRefs(SoOutput *out) const;

    // Changes value in field without doing other notification stuff.
    // Keeps track of references and auditors.
    void setVal(SoEngine *newValue);
};

