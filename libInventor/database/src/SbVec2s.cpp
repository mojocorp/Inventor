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
 |	SbVec3f
 |	SbVec2s
 |	SbVec2f
 |	SbVec4f
 |
 |   Author(s)		: Paul S. Strauss, Nick Thompson, Alain Dumesny
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */
 
 #include <Inventor/SbVec2s.h>
 
 //
 // Returns dot (inner) product of vector and another vector
 //

 int32_t
 SbVec2s::dot(const SbVec2s &v) const
 {
     return vec[0] * v.vec[0] + vec[1] * v.vec[1];
 }

 //
 // Returns 2 individual components
 //

 void
 SbVec2s::getValue(short &x, short &y) const
 {
     x = vec[0];
     y = vec[1];
 }

 //
 // Negates each component of vector in place
 //

 void
 SbVec2s::negate()
 {
     vec[0] = -vec[0];
     vec[1] = -vec[1];
 }

 //
 // Sets value of vector from array of 2 components
 //

 SbVec2s &
 SbVec2s::setValue(const short v[2])	
 {
     vec[0] = v[0];
     vec[1] = v[1];

     return (*this);
 }

 //
 // Sets value of vector from 2 individual components
 //

 SbVec2s &
 SbVec2s::setValue(short x, short y)	
 {
     vec[0] = x;
     vec[1] = y;

     return (*this);
 }

 //
 // Component-wise scalar multiplication operator
 //
 // Note: didn't complain about assignment of int to short !
 SbVec2s &
 SbVec2s::operator *=(int d)
 {
     vec[0] *= d;
     vec[1] *= d;

     return *this;
 }

 SbVec2s &
 SbVec2s::operator *=(double d)
 {
     vec[0] = short(vec[0] * d);
     vec[1] = short(vec[1] * d);

     return *this;
 }

 //
 // Component-wise scalar division operator
 //

 SbVec2s &
 SbVec2s::operator /=(int d)
 {
     vec[0] /= d;
     vec[1] /= d;

     return *this;
 }

 //
 // Component-wise vector addition operator
 //

 SbVec2s &
 SbVec2s::operator +=(const SbVec2s &u)
 {
     vec[0] += u.vec[0];
     vec[1] += u.vec[1];

     return *this;
 }

 //
 // Component-wise vector subtraction operator
 //

 SbVec2s &
 SbVec2s::operator -=(const SbVec2s &u)
 {
     vec[0] -= u.vec[0];
     vec[1] -= u.vec[1];

     return *this;
 }

 //
 // Nondestructive unary negation - returns a new vector
 //

 SbVec2s
 SbVec2s::operator -() const
 {
     return SbVec2s(-vec[0], -vec[1]);
 }


 //
 // Component-wise binary scalar multiplication operator
 //

 SbVec2s
 operator *(const SbVec2s &v, int d)
 {
     return SbVec2s(v.vec[0] * d, v.vec[1] * d);
 }

 SbVec2s
 operator *(const SbVec2s &v, double d)
 {
     return SbVec2s(short(v.vec[0] * d), short(v.vec[1] * d));
 }

 //
 // Component-wise binary scalar division operator
 //

 SbVec2s
 operator /(const SbVec2s &v, int d)
 {
     return SbVec2s(v.vec[0] / d, v.vec[1] / d);
 }

 //
 // Component-wise binary vector addition operator
 //

 SbVec2s
 operator +(const SbVec2s &v1, const SbVec2s &v2)
 {
     return SbVec2s(v1.vec[0] + v2.vec[0],
 		  v1.vec[1] + v2.vec[1]);
 }

 //
 // Component-wise binary vector subtraction operator
 //

 SbVec2s
 operator -(const SbVec2s &v1, const SbVec2s &v2)
 {
     return SbVec2s(v1.vec[0] - v2.vec[0],
 		  v1.vec[1] - v2.vec[1]);
 }

 //
 // Equality comparison operator
 //

 int
 operator ==(const SbVec2s &v1, const SbVec2s &v2)
 {
     return (v1.vec[0] == v2.vec[0] &&
 	    v1.vec[1] == v2.vec[1]);
 }
 