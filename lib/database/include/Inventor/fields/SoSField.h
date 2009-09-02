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
 |	Defines the SoSField class.
 |
 |   Author(s)		: Paul S. Strauss, Nick Thompson, Gavin Bell
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

#ifndef  _SO_SFIELD_
#define  _SO_SFIELD_

#include <Inventor/fields/SoField.h>

/// Abstract base class for all single-value fields.
/// \ingroup Fields
/// Each class derived from <tt>SoSField</tt> begins with an
/// <tt>SoSF</tt> prefix and contains one value of a particular type. Each has
/// #setValue() and #getValue() methods
/// that are used to change or
/// access this value. In addition, some field classes have extra
/// convenience routines that allow values to be set or retrieved in other
/// related formats (see below).
///
/// In addition to #setValue(), all single-value fields overload the "="
/// assignment operator to set the field value from the correct datatype
/// or from another field instance of the same type.
/// The value of a single-value field is written to file in a format
/// dependent on the field type; see the subclass man pages for details.
///
/// A field that is ignored has a tilde (~)
/// either in place of the value (if the actual value is the default)
/// or after it (otherwise).
/// \sa SoField, SoMField
class INVENTOR_API SoSField : public SoField {
  public:
    /// Destructor
    virtual ~SoSField();

    /// Return the type identifier for this field class.
    static SoType	getClassTypeId()	{ return classTypeId; }

  protected:
    // Constructor
    SoSField();

  SoINTERNAL public:
    static void		initClass();

    static void *	createInstance();

  private:
    static SoType	classTypeId;

    // Reads value of field
    virtual SbBool	readValue(SoInput *in) = 0;

    // Writes value of field
    virtual void	writeValue(SoOutput *out) const = 0;
};

#endif /* _SO_SFIELD_ */
