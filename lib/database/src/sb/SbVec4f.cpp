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

#include <Inventor/SbVec4f.h>
#include <Inventor/SbVec3f.h>

#include <math.h>

 //
 // Returns dot (inner) product of vector and another vector
 //

 float
 SbVec4f::dot(const SbVec4f &v) const
 {
     return vec[0] * v.vec[0] + vec[1] * v.vec[1] + 
            vec[2] * v.vec[2] + vec[3] * v.vec[3] ;
 }

 //
 // Returns 4 individual components
 //

 void
 SbVec4f::getValue(float &x, float &y, float &z, float &w) const
 {
     x = vec[0];
     y = vec[1];
     z = vec[2];
     w = vec[3];
 }


 //
 // Returns geometric length of vector
 //

 float
 SbVec4f::length() const
 {
     return sqrtf(vec[0] * vec[0] + vec[1] * vec[1]
 		 + vec[2] * vec[2] + vec[3] * vec[3]);
 }

 //
 // Negates each component of vector in place
 //

 void
 SbVec4f::negate()
 {
     vec[0] = -vec[0];
     vec[1] = -vec[1];
     vec[2] = -vec[2];
     vec[3] = -vec[3];
 }

 //
 // Changes vector to be unit length
 //

 float
 SbVec4f::normalize()
 {
     float len = length();

     if (len != 0.0f)
 	(*this) *= (1.0f / len);

     else setValue(0.0f, 0.0f, 0.0f, 0.0f);

     return len;
 }

 //
 // Sets value of vector from array of 4 components
 //

 SbVec4f &
 SbVec4f::setValue(const float v[4])	
 {
     vec[0] = v[0];
     vec[1] = v[1];
     vec[2] = v[2];
     vec[3] = v[3];

     return (*this);
 }

 //
 // Sets value of vector from 4 individual components
 //

 SbVec4f &
 SbVec4f::setValue(float x, float y, float z, float w)	
 {
     vec[0] = x;
     vec[1] = y;
     vec[2] = z;
     vec[3] = w;

     return (*this);
 }

 //
 // Returns the real portion of the vector by dividing the first three
 // values by the fourth.
 //

 void
 SbVec4f::getReal(SbVec3f &v ) const
 {
     v[0] = vec[0]/vec[3];
     v[1] = vec[1]/vec[3];
     v[2] = vec[2]/vec[3];
 }

 //
 // Component-wise scalar multiplication operator
 //

 SbVec4f &
 SbVec4f::operator *=(float d)
 {
     vec[0] *= d;
     vec[1] *= d;
     vec[2] *= d;
     vec[3] *= d;

     return *this;
 }

 //
 // Component-wise vector addition operator
 //

 SbVec4f &
 SbVec4f::operator +=(const SbVec4f &u)
 {
     vec[0] += u.vec[0];
     vec[1] += u.vec[1];
     vec[2] += u.vec[2];
     vec[3] += u.vec[3];

     return *this;
 }

 //
 // Component-wise vector subtraction operator
 //

 SbVec4f &
 SbVec4f::operator -=(const SbVec4f &u)
 {
     vec[0] -= u.vec[0];
     vec[1] -= u.vec[1];
     vec[2] -= u.vec[2];
     vec[3] -= u.vec[3];

     return *this;
 }

 //
 // Nondestructive unary negation - returns a new vector
 //

 SbVec4f
 SbVec4f::operator -() const
 {
     return SbVec4f(-vec[0], -vec[1], -vec[2], -vec[3]);
 }


 //
 // Component-wise binary scalar multiplication operator
 //

 SbVec4f
 operator *(const SbVec4f &v, float d)
 {
     return SbVec4f(v.vec[0] * d, v.vec[1] * d, v.vec[2] * d, v.vec[3] * d);
 }

 //
 // Component-wise binary vector addition operator
 //

 SbVec4f
 operator +(const SbVec4f &v1, const SbVec4f &v2)
 {
     return SbVec4f(v1.vec[0] + v2.vec[0],
  		   v1.vec[1] + v2.vec[1],
  		   v1.vec[2] + v2.vec[2],
  		   v1.vec[3] + v2.vec[3]);
 }

 //
 // Component-wise binary vector subtraction operator
 //

 SbVec4f
 operator -(const SbVec4f &v1, const SbVec4f &v2)
 {
     return SbVec4f(v1.vec[0] - v2.vec[0],
 		   v1.vec[1] - v2.vec[1],
 		   v1.vec[2] - v2.vec[2],
 		   v1.vec[3] - v2.vec[3]);
 }

 //
 // Equality comparison operator. Componenents must match exactly.
 //

 int
 operator ==(const SbVec4f &v1, const SbVec4f &v2)
 {
     return (v1.vec[0] == v2.vec[0] &&
 	    v1.vec[1] == v2.vec[1] &&
 	    v1.vec[2] == v2.vec[2] &&
 	    v1.vec[3] == v2.vec[3]);
 }

 //
 // Equality comparison operator within a tolerance.
 //

 SbBool
 SbVec4f::equals(const SbVec4f & v, float tolerance) const
 {
     SbVec4f	diff = *this - v;

     return diff.dot(diff) <= tolerance;
 }
 
