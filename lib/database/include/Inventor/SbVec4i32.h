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
 |	This file contains definitions of SbVec4i32
 |
 |   Classes:
 |	SbVec4i32
 |
 |   Author(s)		: Paul S. Strauss, Nick Thompson, 
 |			  David Mott, Alain Dumesny
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

#ifndef _SB_VEC4I32_
#define _SB_VEC4I32_

#include <Inventor/SbBasic.h>

/// 4D vector class.
/// \ingroup Basics
/// 4D vector class used to store homogeneous coordinates. This class is used
/// in Inventor for arguments and return values.
/// \sa SbVec2f, SbVec3f, SbVec2s, SbRotation
class INVENTOR_API SbVec4i32 {
  public:

    /// Default constructor
    SbVec4i32()						{ }

    /// Constructor given an array of 4 components
    SbVec4i32(const int32_t v[4])				{ setValue(v); }

    /// Constructor given 4 individual components
    SbVec4i32(int32_t x, int32_t y, int32_t z, int32_t w)	      { setValue(x, y, z, w); }

    /// Returns dot (inner) product of vector and another vector
    int32_t	dot(const SbVec4i32 &v) const;

    /// Returns pointer to array of 4 components
    const int32_t	*getValue() const			{ return vec; }

    /// Returns 4 individual components
    void	getValue(int32_t &x, int32_t &y, int32_t &z, int32_t &w) const;

    /// Returns geometric length of vector
    int32_t	length() const;

    /// Negates each component of vector in place
    void	negate();

    /// Changes vector to be unit length, returning the length before normalization.
    int32_t	normalize();

    /// Sets value of vector from array of 4 components
    SbVec4i32 &	setValue(const int32_t v[4]);

    /// Sets value of vector from 4 individual components
    SbVec4i32 &	setValue(int32_t x, int32_t y, int32_t z, int32_t w);

    /// Accesses indexed component of vector
    int32_t &	  operator [](int i) 		{ return (vec[i]); }

    /// Accesses indexed component of vector
    const int32_t & operator [](int i) const	{ return (vec[i]); }

    /// Component-wise scalar multiplication operator
    SbVec4i32 &	operator *=(int32_t d);

    /// Component-wise scalar division operator
    SbVec4i32 &	operator /=(int32_t d) { return *this *= (1.0f / d); }

    /// Component-wise vector addition operator
    SbVec4i32 &	operator +=(const SbVec4i32 &u);

    /// Component-wise vector subtraction operator
    SbVec4i32 &	operator -=(const SbVec4i32 &u);

    /// Nondestructive unary negation - returns a new vector
    SbVec4i32	operator -() const;

    /// Component-wise binary scalar multiplication operator
    friend INVENTOR_API SbVec4i32	operator *(const SbVec4i32 &v, int32_t d);

    /// Component-wise binary scalar multiplication operator
    friend INVENTOR_API SbVec4i32	operator *(int32_t d, const SbVec4i32 &v) { return v * d; }

    /// Component-wise binary scalar division operator
    friend INVENTOR_API SbVec4i32	operator /(const SbVec4i32 &v, int32_t d) { return v * (1.0f / d); }

    /// Component-wise binary vector addition operator
    friend INVENTOR_API SbVec4i32	operator +(const SbVec4i32 &v1, const SbVec4i32 &v2);

    /// Component-wise binary vector subtraction operator
    friend INVENTOR_API SbVec4i32	operator -(const SbVec4i32 &v1, const SbVec4i32 &v2);

    /// Equality comparison operator
    friend INVENTOR_API int		operator ==(const SbVec4i32 &v1, const SbVec4i32 &v2);

    /// Inequality comparison operator
    friend INVENTOR_API int		operator !=(const SbVec4i32 &v1, const SbVec4i32 &v2) { return !(v1 == v2); }

    /// Equality comparison within given tolerance - the square of the
    /// length of the maximum distance between the two vectors
    SbBool		equals(const SbVec4i32 & v, int32_t tolerance) const;

  protected:
    int32_t	vec[4];		// Storage for vector components
};

#endif /* _SB_VEC4I32_ */
