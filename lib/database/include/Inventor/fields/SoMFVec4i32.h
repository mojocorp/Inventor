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
 |	SoMFVec4i32
 |
 |   Author(s)		: Paul S. Strauss
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

#ifndef  _SO_MF_VEC4I32_
#define  _SO_MF_VEC4I32_

#include <Inventor/fields/SoSubField.h>
#include <Inventor/SbVec4i32.h>

/// Multiple-value field containing any number of four-dimensional vectors.
/// \ingroup Fields
/// A multiple-value field that contains any number of four-dimensional
/// vectors.
///
///
/// <tt>SoMFVec4fs</tt> are written to file as one or more triples of floating
/// point values separated by whitespace.
///
///
/// When more than one value is present, all of the
/// values are enclosed in square brackets and separated by commas; for
/// example:
///
/// [ 0 0 0, 1.2 3.4 5.6, 98.6 -4e1 212 ]
///
class INVENTOR_API SoMFVec4i32 : public SoMField {
    // Use standard field stuff
    SO_MFIELD_HEADER(SoMFVec4i32, SbVec4i32, const SbVec4i32 &);
    SO_MFIELD_SETVALUESPOINTER_HEADER(SbVec4i32);
    SO_MFIELD_SETVALUESPOINTER_HEADER(int32_t);

  public:
    //
    // Some additional convenience functions:
    //

    /// Sets \a num values starting at index \a start to the given floating point
    /// values.  There must be \a num *4 values in the passed array.
    void	setValues(int start, int num, const int32_t xyzw[][4]);

    /// Set the \a index 'th value to the given floating point values.
    void	set1Value(int index, int32_t x, int32_t y, int32_t z, int32_t w);

    /// Set the \a index 'th value to the given floating point values.
    void	set1Value(int index, const int32_t xyzw[4]);

    /// Sets the field to contain the given value and only the given value (if
    /// the array had multiple values before, they are deleted).
    void	setValue(int32_t x, int32_t y, int32_t z, int32_t w);

    /// Sets the field to contain the given value and only the given value (if
    /// the array had multiple values before, they are deleted).
    void	setValue(const int32_t xyzw[4]);

  SoINTERNAL public:
    static void	initClass();

  private:
    virtual void	writeBinaryValues(SoOutput *out) const;
    virtual SbBool	readBinaryValues(SoInput *in, int numToRead);
};

#endif /* _SO_MF_VEC4I32_ */
