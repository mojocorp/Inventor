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
 |   $Revision: 1.3 $
 |
 |   Description:
 |	This file contains definitions of various linear algebra classes,
 |	such as vectors, coordinates, etc..
 |
 |   Classes:
 |	SbVec3s
 |
 |   Author(s)		: Paul S. Strauss, Nick Thompson, 
 |			  David Mott, Alain Dumesny
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

#ifndef _SB_VEC3S_
#define _SB_VEC3S_

#include <Inventor/SbBasic.h>

//////////////////////////////////////////////////////////////////////////////
//
//  WARNING!!!!!  Transcription of arrays of this class assume that the
//                only data stored in this class are three consecutive values.
//                Do not add any extra data members!!!
//
//////////////////////////////////////////////////////////////////////////////
class SbPlane;

/// 3D vector class.
/// \ingroup Basics
/// 3D vector class used to store 3D vectors and points. This class is used
/// throughout Inventor for arguments and return values.
/// \sa SbVec2f, SbVec3f, SbVec4f, SbVec2s, SbRotation
class INVENTOR_API SbVec3s {
public:
    /// Default constructor
    SbVec3s() { }

    /// Constructor given an array of 3 components
    SbVec3s(const short v[3]) {
        vec[0] = v[0];
        vec[1] = v[1];
        vec[2] = v[2];
    }

    /// Constructor given 3 individual components
    SbVec3s(short x, short y, short z) {
        vec[0] = x;
        vec[1] = y;
        vec[2] = z;
    }

    /// Returns right-handed cross product of vector and another vector
    SbVec3s cross(const SbVec3s &v) const;

    /// Returns dot (inner) product of vector and another vector
    short dot(const SbVec3s &v) const;

    /// Returns pointer to array of 3 components
    const short *getValue() const {
        return vec;
    }

    /// Returns 3 individual components
    void getValue(short &x, short &y, short &z) const;

    /// Negates each component of vector in place
    void negate();

    /// Sets value of vector from array of 3 components
    SbVec3s & setValue(const short v[3]) {
        vec[0] = v[0];
        vec[1] = v[1];
        vec[2] = v[2];
        return *this;
    }

    /// Sets value of vector from 3 individual components
    SbVec3s & setValue(short x, short y, short z) {
        vec[0] = x;
        vec[1] = y;
        vec[2] = z;
        return *this;
    }

    /// Accesses indexed component of vector
    short & operator [](int i) {
        return (vec[i]);
    }

    /// Accesses indexed component of vector
    const short & operator [](int i) const {
        return (vec[i]);
    }

    /// Component-wise scalar multiplication operator
    SbVec3s & operator *=(short d);

    /// Component-wise scalar division operator
    SbVec3s & operator /=(short d) {
        vec[0] /= d;
        vec[1] /= d;
        vec[2] /= d;
        return *this;
    }

    /// Component-wise vector addition operator
    SbVec3s & operator +=(SbVec3s v);

    /// Component-wise vector subtraction operator
    SbVec3s & operator -=(SbVec3s v);

    /// Nondestructive unary negation - returns a new vector
    SbVec3s operator -() const;

    /// Component-wise binary scalar multiplication operator
    friend INVENTOR_API SbVec3s operator *(const SbVec3s &v, short d);

    /// Component-wise binary scalar multiplication operator
    friend INVENTOR_API SbVec3s operator *(short d, const SbVec3s &v) {
        return v * d;
    }

    /// Component-wise binary scalar division operator
    friend INVENTOR_API SbVec3s operator /(const SbVec3s &v, short d) {
        return SbVec3s(v[0]/d, v[1]/d, v[2]/d);
    }

    /// Component-wise binary vector addition operator
    friend INVENTOR_API SbVec3s operator +(const SbVec3s &v1, const SbVec3s &v2);

    /// Component-wise binary vector subtraction operator
    friend INVENTOR_API SbVec3s operator -(const SbVec3s &v1, const SbVec3s &v2);

    /// Equality comparison operator
    friend INVENTOR_API int  operator ==(const SbVec3s &v1, const SbVec3s &v2);

    /// Inequality comparison operator
    friend INVENTOR_API int  operator !=(const SbVec3s &v1, const SbVec3s &v2) {
        return !(v1 == v2);
    }

protected:
    short vec[3];  // Storage for vector components
};

#endif /* _SB_VEC3S_ */
