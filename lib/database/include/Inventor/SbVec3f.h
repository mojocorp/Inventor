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
 |	SbVec3f
 |
 |   Author(s)		: Paul S. Strauss, Nick Thompson, 
 |			  David Mott, Alain Dumesny
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

#ifndef _SB_VEC3F_
#define _SB_VEC3F_

#include <Inventor/SbBasic.h>

//////////////////////////////////////////////////////////////////////////////
//
//  Class: SbVec3f
//
//  3D vector used to represent points or directions. Each component of
//  the vector is a floating-point number.
//
//  WARNING!!!!!  Transcription of arrays of this class assume that the
//                only data stored in this class are three consecutive values.
//                Do not add any extra data members!!!
//
//////////////////////////////////////////////////////////////////////////////
class SbPlane;

class INVENTOR_API SbVec3f {
  public:
    // Default constructor
    SbVec3f()						{ }

    // Constructor given an array of 3 components
    SbVec3f(const float v[3])
	 { vec[0] = v[0]; vec[1] = v[1]; vec[2] = v[2]; }

    // Constructor given 3 individual components
    SbVec3f(float x, float y, float z)
	 { vec[0] = x; vec[1] = y; vec[2] = z; }

    // Constructor given 3 planes
    SbVec3f(SbPlane &p0, SbPlane &p1, SbPlane &p2);

// C-api: begin
    // Returns right-handed cross product of vector and another vector
    SbVec3f	cross(const SbVec3f &v) const;
// C-api: end

    // Returns dot (inner) product of vector and another vector
    float	dot(const SbVec3f &v) const;
// C-api.h: #define SbV3fDot(_v0, _v1)
// C-api.h:     ((_v0).vec[0] * (_v1).vec[0] + (_v0).vec[1] * (_v1).vec[1] +
// C-api.h:      (_v0).vec[2] * (_v1).vec[2])

    // Returns pointer to array of 3 components
    const float	*getValue() const			{ return vec; }
// C-api.h: #define SbV3fGetXYZ(_xyz, _src)
// C-api.h:     (((_xyz)[0] = (_src).vec[0]), ((_xyz)[1] = (_src).vec[1]),
// C-api.h:	 ((_xyz)[2] = (_src).vec[2]))

    // Returns 3 individual components
    void	getValue(float &x, float &y, float &z) const;
// C-api.h: #define SbV3fGetX_Y_Z(_x, _y, _z, _src)
// C-api.h:     (((_x) = (_src).vec[0]), ((_y) = (_src).vec[1]), ((_z) = (_src).vec[2]))

    // Returns geometric length of vector
    float	length() const;
// C-api.h: #define SbV3fLen(_v)
// C-api.h:     (sqrtf(SbV3fDot((_v), (_v))))

// C-api: begin
    // Changes vector to be unit length
    // C-api: name=norm
    float	normalize();
// C-api: end

    // Negates each component of vector in place
    void	negate();
// C-api.h: #define SbV3fNegate(_v)
// C-api.h: 	SbV3fMultBy(_v, -1.0)

    // Sets value of vector from array of 3 components
    SbVec3f &	setValue(const float v[3])
	 { vec[0] = v[0]; vec[1] = v[1]; vec[2] = v[2]; return *this; }
// C-api.h: #define SbV3fSetXYZ(_dest, _src)
// C-api.h:     (((_dest).vec[0] = (_src)[0]), ((_dest).vec[1] = (_src)[1]),
// C-api.h:      ((_dest).vec[2] = (_src)[2]))

    // Sets value of vector from 3 individual components
    SbVec3f &	setValue(float x, float y, float z)
	 { vec[0] = x; vec[1] = y; vec[2] = z; return *this; }
// C-api.h: #define SbV3fSetX_Y_Z(_dest, _x, _y, _z)
// C-api.h:     (((_dest).vec[0] = (_x)), ((_dest).vec[1] = (_y)),
// C-api.h:      ((_dest).vec[2] = (_z)))

    // Sets value of vector to be convex combination of 3 other
    // vectors, using barycentic coordinates
    SbVec3f &	setValue(const SbVec3f &barycentic,
		const SbVec3f &v0, const SbVec3f &v1, const SbVec3f &v2);

    // Accesses indexed component of vector
    float &	  operator [](int i) 		{ return (vec[i]); }
    const float & operator [](int i) const	{ return (vec[i]); }

    // Component-wise scalar multiplication and division operators
    SbVec3f &	operator *=(float d);
// C-api.h: #define SbV3fMultBy(_v, _s)
// C-api.h:     (((_v).vec[0] *= (_s)), ((_v).vec[1] *= (_s)), ((_v).vec[2] *= (_s)))

    SbVec3f &	operator /=(float d)
	{ return *this *= (1.0f / d); }

    // Component-wise vector addition and subtraction operators
    SbVec3f &	operator +=(SbVec3f v);
    SbVec3f &	operator -=(SbVec3f v);

    // Nondestructive unary negation - returns a new vector
    SbVec3f	operator -() const;

    // Component-wise binary scalar multiplication and division operators
    friend INVENTOR_API SbVec3f	operator *(const SbVec3f &v, float d);
    friend INVENTOR_API SbVec3f	operator *(float d, const SbVec3f &v)
	{ return v * d; }
    friend INVENTOR_API SbVec3f	operator /(const SbVec3f &v, float d)
	{ return v * (1.0f / d); }

    // Component-wise binary vector addition and subtraction operators
    friend INVENTOR_API SbVec3f	operator +(const SbVec3f &v1, const SbVec3f &v2);
// C-api.h: #define SbV3fAdd(_dest, _src1, _src2)
// C-api.h:     (((_dest).vec[0] = (_src1).vec[0] + (_src2).vec[0]),
// C-api.h:      ((_dest).vec[1] = (_src1).vec[1] + (_src2).vec[1]),
// C-api.h:      ((_dest).vec[2] = (_src1).vec[2] + (_src2).vec[2]))

    friend INVENTOR_API SbVec3f	operator -(const SbVec3f &v1, const SbVec3f &v2);
// C-api.h: #define SbV3fSub(_dest, _src1, _src2)
// C-api.h:     (((_dest).vec[0] = (_src1).vec[0] - (_src2).vec[0]),
// C-api.h:      ((_dest).vec[1] = (_src1).vec[1] - (_src2).vec[1]),
// C-api.h:      ((_dest).vec[2] = (_src1).vec[2] - (_src2).vec[2]))

// C-api: begin
    // Equality comparison operator
    friend INVENTOR_API int		operator ==(const SbVec3f &v1, const SbVec3f &v2);
    friend INVENTOR_API int		operator !=(const SbVec3f &v1, const SbVec3f &v2)
	{ return !(v1 == v2); }

    // Equality comparison within given tolerance - the square of the
    // length of the maximum distance between the two vectors
    SbBool		equals(const SbVec3f v, float tolerance) const;

    // Returns principal axis that is closest (based on maximum dot
    // product) to this vector
    SbVec3f		getClosestAxis() const;

  protected:
    float	vec[3];		// Storage for vector components
};

#endif /* _SB_VEC3F_ */
