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
 |	SbVec4f
 |
 |   Author(s)		: Paul S. Strauss, Nick Thompson, 
 |			  David Mott, Alain Dumesny
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

#ifndef _SB_VEC4F_
#define _SB_VEC4F_

#include <Inventor/SbBasic.h>

//////////////////////////////////////////////////////////////////////////////
//
//  Class: SbVec4f
//
//  4D vector used to represet rational points or directions. Each component of
//  the vector is a float.
//
//////////////////////////////////////////////////////////////////////////////
class SbVec3f;

class INVENTOR_API SbVec4f {
  public:

    // Default constructor
    SbVec4f()						{ }

    // Constructor given an array of 4 components
    SbVec4f(const float v[4])				{ setValue(v); }

    // Constructor given 4 individual components
    SbVec4f(float x, float y, float z, float w)	      { setValue(x, y, z, w); }

    // Returns dot (inner) product of vector and another vector
    float	dot(const SbVec4f &v) const;
// C-api.h: #define SbV4fDot(_v0, _v1)
// C-api.h:     ((_v0).vec[0] * (_v1).vec[0] + (_v0).vec[1] * (_v1).vec[1] +
// C-api.h:      (_v0).vec[2] * (_v1).vec[2] + (_v0).vec[3] * (_v1).vec[3])

// C-api: begin
    // Returns the real portion of the vector by dividing by the fourth value
    void	getReal(SbVec3f &v) const;
// C-api: end

    // Returns pointer to array of 4 components
    const float	*getValue() const			{ return vec; }
// C-api.h: #define SbV4fGetXYZW(_dest, _src)
// C-api.h:     (((_dest)[0] = (_src).vec[0]), ((_dest)[1] = (_src).vec[1]),
// C-api.h:      ((_dest)[2] = (_src).vec[2]), ((_dest)[3] = (_src).vec[3]))

    // Returns 4 individual components
    void	getValue(float &x, float &y, float &z, float &w) const;
// C-api.h: #define SbV4fGetX_Y_Z_W(_x, _y, _z, _w, _src)
// C-api.h:     (((_x) = (_src).vec[0]), ((_y) = (_src).vec[1]),
// C-api.h:      ((_z) = (_src).vec[2]), ((_w) = (_src).vec[3]))

    // Returns geometric length of vector
    float	length() const;
// C-api.h: #define SbV4fLen(_v)
// C-api.h:     (sqrtf(SbV4fDot((_v), (_v))))

    // Negates each component of vector in place
    void	negate();
// C-api.h: #define SbV4fNegate(_v)
// C-api.h: 	SbV4fMultBy(_v, -1.0)

// C-api: begin
    // Changes vector to be unit length
    // C-api: name=norm
    float	normalize();
// C-api: end

    // Sets value of vector from array of 4 components
    SbVec4f &	setValue(const float v[4]);
// C-api.h: #define SbV4fSetXYZW(_dest, _src)
// C-api.h:     (((_dest).vec[0] = (_src)[0]), ((_dest).vec[1] = (_src)[1]),
// C-api.h:      ((_dest).vec[2] = (_src)[2]), ((_dest).vec[3] = (_src)[3]))

    // Sets value of vector from 4 individual components
    SbVec4f &	setValue(float x, float y, float z, float w);
// C-api.h: #define SbV4fSetX_Y_Z_W(_dest, _x, _y, _z, _w)
// C-api.h:     (((_dest).vec[0] = (_x)), ((_dest).vec[1] = (_y)),
// C-api.h:      ((_dest).vec[2] = (_z)), ((_dest).vec[3] = (_w)))

    // Accesses indexed component of vector
    float &	  operator [](int i) 		{ return (vec[i]); }
    const float & operator [](int i) const	{ return (vec[i]); }

    // Component-wise scalar multiplication and division operators
    SbVec4f &	operator *=(float d);
// C-api.h: #define SbV4fMultBy(_v, _s)
// C-api.h:     (((_v).vec[0] *= (_s)), ((_v).vec[1] *= (_s)),
// C-api.h:      ((_v).vec[2] *= (_s)), ((_v).vec[3] *= (_s)))

    SbVec4f &	operator /=(float d)
	{ return *this *= (1.0f / d); }

    // Component-wise vector addition and subtraction operators
    SbVec4f &	operator +=(const SbVec4f &u);
    SbVec4f &	operator -=(const SbVec4f &u);

    // Nondestructive unary negation - returns a new vector
    SbVec4f	operator -() const;

    // Component-wise binary scalar multiplication and division operators
    friend INVENTOR_API SbVec4f	operator *(const SbVec4f &v, float d);
    friend INVENTOR_API SbVec4f	operator *(float d, const SbVec4f &v)
	{ return v * d; }
    friend INVENTOR_API SbVec4f	operator /(const SbVec4f &v, float d)
	{ return v * (1.0f / d); }

    // Component-wise binary vector addition and subtraction operators
    friend INVENTOR_API SbVec4f	operator +(const SbVec4f &v1, const SbVec4f &v2);
// C-api.h: #define SbV4fAdd(_dest, _src1, _src2)
// C-api.h:     (((_dest).vec[0] = (_src1).vec[0] + (_src2).vec[0]),
// C-api.h:      ((_dest).vec[1] = (_src1).vec[1] + (_src2).vec[1]),
// C-api.h:      ((_dest).vec[2] = (_src1).vec[2] + (_src2).vec[2]))
// C-api.h:      ((_dest).vec[3] = (_src1).vec[3] + (_src2).vec[3]))

    friend INVENTOR_API SbVec4f	operator -(const SbVec4f &v1, const SbVec4f &v2);
// C-api.h: #define SbV4fSub(_dest, _src1, _src2)
// C-api.h:     (((_dest).vec[0] = (_src1).vec[0] - (_src2).vec[0]),
// C-api.h:      ((_dest).vec[1] = (_src1).vec[1] - (_src2).vec[1]),
// C-api.h:      ((_dest).vec[2] = (_src1).vec[2] - (_src2).vec[2]))
// C-api.h:      ((_dest).vec[3] = (_src1).vec[3] - (_src2).vec[3]))

// C-api: begin
    // Equality comparison operator
    friend INVENTOR_API int		operator ==(const SbVec4f &v1, const SbVec4f &v2);
    friend INVENTOR_API int		operator !=(const SbVec4f &v1, const SbVec4f &v2)
	{ return !(v1 == v2); }

    // Equality comparison within given tolerance - the square of the
    // length of the maximum distance between the two vectors
    SbBool		equals(const SbVec4f v, float tolerance) const;

  protected:
    float	vec[4];		// Storage for vector components
};

#endif /* _SB_VEC4F_ */
