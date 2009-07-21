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
 |	SbVec2s
 |
 |   Author(s)		: Paul S. Strauss, Nick Thompson, 
 |			  David Mott, Alain Dumesny
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

#ifndef _SB_VEC2S_
#define _SB_VEC2S_

#include <Inventor/SbBasic.h>

//////////////////////////////////////////////////////////////////////////////
//
//  Class: SbVec2s
//
//  2D vector used to represet points or directions. Each component of
//  the vector is a (short) integer.
//
//////////////////////////////////////////////////////////////////////////////

class INVENTOR_API SbVec2s {
  public:

    // Default constructor
    SbVec2s()						{ }

    // Constructor given an array of 2 components
    SbVec2s(const short v[2])				{ setValue(v); }

    // Constructor given 2 individual components
    SbVec2s(short x, short y)				{ setValue(x, y); }

    // Returns dot (inner) product of vector and another vector
    int32_t	dot(const SbVec2s &v) const;
// C-api.h: #define SbV2sDot(_v0, _v1)
// C-api.h:     ((_v0).vec[0] * (_v1).vec[0] + (_v0).vec[1] * (_v1).vec[1])

    // Returns pointer to array of 2 components
    const short	*getValue() const			{ return vec; }
// C-api.h: #define SbV2sGetXY(_dest, _src)
// C-api.h:     (((_dest)[0] = (_src).vec[0]), ((_dest)[1] = (_src).vec[1]))

    // Returns 2 individual components
    void	getValue(short &x, short &y) const;
// C-api.h: #define SbV2sGetX_Y(_x, _y, _src)
// C-api.h:     (((_x) = (_src).vec[0]), ((_y) = (_src).vec[1]))

    // Negates each component of vector in place
    void	negate();
// C-api.h: #define SbV2sNegate(_v)
// C-api.h: 	SbV2sMultBy(_v, -1.0)

    // Sets value of vector from array of 2 components
    SbVec2s &	setValue(const short v[2]);
// C-api.h: #define SbV2sSetXY(_dest, _src)
// C-api.h:     (((_dest).vec[0] = (_src)[0]), ((_dest)[1] = (_src)[1]))

    // Sets value of vector from 2 individual components
    SbVec2s &	setValue(short x, short y);
// C-api.h: #define SbV2sSetX_Y(_dest, _x, _y)
// C-api.h:     (((_dest).vec[0] = (_x)), ((_dest).vec[1] = (_y)))

    // Accesses indexed component of vector
    short &	  operator [](int i) 		{ return (vec[i]); }
    const short & operator [](int i) const 	{ return (vec[i]); }

    // Component-wise scalar multiplication and division operators
    SbVec2s &	operator *=(int d);
    SbVec2s &	operator *=(double d);
// C-api.h: #define SbV2sMultBy(_v, _s)
// C-api.h:     (((_v).vec[0] *= (_s)), ((_v).vec[1] *= (_s)))

    SbVec2s &	operator /=(int d);
    SbVec2s &	operator /=(double d)
	{ return *this *= (1.0 / d); }
// C-api.h: #define SbV2sDivBy(_v, _s)
// C-api.h:     (((_v).vec[0] /= (_s)), ((_v).vec[1] /= (_s)))

    // Component-wise vector addition and subtraction operators
    SbVec2s &	operator +=(const SbVec2s &u);
    SbVec2s &	operator -=(const SbVec2s &u);

    // Nondestructive unary negation - returns a new vector
    SbVec2s	operator -() const;

    // Component-wise binary scalar multiplication and division operators
    friend INVENTOR_API SbVec2s	operator *(const SbVec2s &v, int d);
    friend INVENTOR_API SbVec2s	operator *(const SbVec2s &v, double d);
    friend INVENTOR_API SbVec2s	operator *(int d, const SbVec2s &v)
	{ return v * d; }
    friend INVENTOR_API SbVec2s	operator *(double d, const SbVec2s &v)
	{ return v * d; }
    friend INVENTOR_API SbVec2s	operator /(const SbVec2s &v, int d);
    friend INVENTOR_API SbVec2s	operator /(const SbVec2s &v, double d)
	{ return v * (1.0 / d); }

    // Component-wise binary vector addition and subtraction operators
    friend INVENTOR_API SbVec2s	operator +(const SbVec2s &v1, const SbVec2s &v2);
// C-api.h: #define SbV2sAdd(_dest, _src1, _src2)
// C-api.h:     (((_dest).vec[0] = (_src1).vec[0] + (_src2).vec[0]),
// C-api.h:      ((_dest).vec[1] = (_src1).vec[1] + (_src2).vec[1]))

    friend INVENTOR_API SbVec2s	operator -(const SbVec2s &v1, const SbVec2s &v2);
// C-api.h: #define SbV2sSub(_dest, _src1, _src2)
// C-api.h:     (((_dest).vec[0] = (_src1).vec[0] - (_src2).vec[0]),
// C-api.h:      ((_dest).vec[1] = (_src1).vec[1] - (_src2).vec[1]))

// C-api: begin
    // Equality comparison operator
    friend INVENTOR_API int		operator ==(const SbVec2s &v1, const SbVec2s &v2);
    friend INVENTOR_API int		operator !=(const SbVec2s &v1, const SbVec2s &v2)
	{ return !(v1 == v2); }

  protected:
    short	vec[2];		// Storage for vector components
};

#endif /* _SB_VEC2S_ */
