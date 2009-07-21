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
 |	SbVec2f
 |
 |   Author(s)		: Paul S. Strauss, Nick Thompson, 
 |			  David Mott, Alain Dumesny
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

#ifndef _SB_VEC2F_
#define _SB_VEC2F_

#include <Inventor/SbBasic.h>

//////////////////////////////////////////////////////////////////////////////
//
//  Class: SbVec2f
//
//  2D vector used to represet points or directions. Each component of
//  the vector is a float.
//
//////////////////////////////////////////////////////////////////////////////

class INVENTOR_API SbVec2f {
  public:

    // Default constructor
    SbVec2f()						{ }

    // Constructor given an array of 2 components
    SbVec2f(const float v[2])				{ setValue(v); }

    // Constructor given 2 individual components
    SbVec2f(float x, float y)				{ setValue(x, y); }

    // Returns dot (inner) product of vector and another vector
    float	dot(const SbVec2f &v) const;
// C-api.h: #define SbV2fDot(_v0, _v1)
// C-api.h:     ((_v0).vec[0] * (_v1).vec[0] + (_v0).vec[1] * (_v1).vec[1])

    // Returns pointer to array of 2 components
    const float	*getValue() const			{ return vec; }
// C-api.h: #define SbV2fGetXY(_xy, _src)
// C-api.h:     (((_xy)[0] = (_src).vec[0]), ((_xy)[1] = (_src).vec[1]))

    // Returns 2 individual components
    void	getValue(float &x, float &y) const;
// C-api.h: #define SbV2fGetX_Y(_x, _y, _src)
// C-api.h:     (((_x) = (_src).vec[0]), ((_y) = (_src).vec[1]))

    // Returns geometric length of vector
    float	length() const;
// C-api.h: #define SbV2fLen(_v)
// C-api.h:     (sqrtf(SbV2fDot((_v), (_v))))

    // Negates each component of vector in place
    void	negate();
// C-api.h: #define SbV2fNegate(_v)
// C-api.h: 	SbV2fMultBy(_v, -1.0)

// C-api: begin
    // Changes vector to be unit length
    // C-api: name=norm
    float	normalize();
// C-api: end

    // Sets value of vector from array of 2 components
    SbVec2f &	setValue(const float v[2]);
// C-api.h: #define SbV2fSetXY(_dest, _src)
// C-api.h:     (((_dest).vec[0] = (_src)[0]), ((_dest).vec[1] = (_src)[1]))

    // Sets value of vector from 2 individual components
    SbVec2f &	setValue(float x, float y);
// C-api.h: #define SbV2fSetX_Y(_dest, _x, _y)
// C-api.h:     (((_dest).vec[0] = (_x)), ((_dest).vec[1] = (_y)))

    // Accesses indexed component of vector
    float &	  operator [](int i) 		{ return (vec[i]); }
    const float & operator [](int i) const 	{ return (vec[i]); }

    // Component-wise scalar multiplication and division operators
    SbVec2f &	operator *=(float d);
// C-api.h: #define SbV2fMultBy(_v, _s)
// C-api.h:     (((_v).vec[0] *= (_s)), ((_v).vec[1] *= (_s)))

    SbVec2f &	operator /=(float d)
	{ return *this *= (1.0f / d); }

    // Component-wise vector addition and subtraction operators
    SbVec2f &	operator +=(const SbVec2f &u);
    SbVec2f &	operator -=(const SbVec2f &u);

    // Nondestructive unary negation - returns a new vector
    SbVec2f	operator -() const;

    // Component-wise binary scalar multiplication and division operators
    friend INVENTOR_API SbVec2f	operator *(const SbVec2f &v, float d);
    friend INVENTOR_API SbVec2f	operator *(float d, const SbVec2f &v)
	{ return v * d; }
    friend INVENTOR_API SbVec2f	operator /(const SbVec2f &v, float d)
	{ return v * (1.0f / d); }

    // Component-wise binary vector addition and subtraction operators
    friend INVENTOR_API SbVec2f	operator +(const SbVec2f &v1, const SbVec2f &v2);
// C-api.h: #define SbV2fAdd(_dest, _src1, _src2)
// C-api.h:     (((_dest).vec[0] = (_src1).vec[0] + (_src2).vec[0]),
// C-api.h:      ((_dest).vec[1] = (_src1).vec[1] + (_src2).vec[1]))

    friend INVENTOR_API SbVec2f	operator -(const SbVec2f &v1, const SbVec2f &v2);
// C-api.h: #define SbV2fSub(_dest, _src1, _src2)
// C-api.h:     (((_dest).vec[0] = (_src1).vec[0] - (_src2).vec[0]),
// C-api.h:      ((_dest).vec[1] = (_src1).vec[1] - (_src2).vec[1]))

// C-api: begin
    // Equality comparison operator
    friend INVENTOR_API int		operator ==(const SbVec2f &v1, const SbVec2f &v2);
    friend INVENTOR_API int		operator !=(const SbVec2f &v1, const SbVec2f &v2)
	{ return !(v1 == v2); }

    // Equality comparison within given tolerance - the square of the
    // length of the maximum distance between the two vectors
    SbBool		equals(const SbVec2f v, float tolerance) const;

  protected:
    float	vec[2];		// Storage for vector components
};

#endif /* _SB_VEC2F_ */
