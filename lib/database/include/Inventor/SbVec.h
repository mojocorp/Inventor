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
 |	SbVec2f
 |	SbVec2s
 |	SbVec4f
 |
 |   Author(s)		: Paul S. Strauss, Nick Thompson,
 |			  David Mott, Alain Dumesny
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */
#ifndef _SB_VEC_
#define _SB_VEC_

#include <cmath>
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

template <typename Type>
class SbVec3 {
  public:
    // Default constructor
    SbVec3()						{ }

    // Constructor given an array of 3 components
    SbVec3(const Type v[3])
     { vec[0] = v[0]; vec[1] = v[1]; vec[2] = v[2]; }

    // Constructor given 3 individual components
    SbVec3(Type x, Type y, Type z)
     { vec[0] = x; vec[1] = y; vec[2] = z; }

    // Constructor given 3 planes
    SbVec3(SbPlane &p0, SbPlane &p1, SbPlane &p2);

    // Returns right-handed cross product of vector and another vector
    SbVec3<Type>	cross(const SbVec3<Type> &v) const
    {
        return SbVec3<Type>(vec[1] * v.vec[2] - vec[2] * v.vec[1],
                            vec[2] * v.vec[0] - vec[0] * v.vec[2],
                            vec[0] * v.vec[1] - vec[1] * v.vec[0]);
    }

    // Returns dot (inner) product of vector and another vector
    Type	dot(const SbVec3<Type> &v) const
    { return (vec[0] * v.vec[0] + vec[1] * v.vec[1] + vec[2] * v.vec[2]); }

    // Returns pointer to array of 3 components
    const Type	*getValue() const			{ return vec; }

    // Returns 3 individual components
    void	getValue(Type &x, Type &y, Type &z) const
    {
        x = vec[0];
        y = vec[1];
        z = vec[2];
    }

    // Returns geometric length of vector
    Type	length() const
    { return std::sqrt(vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2]); }

    // Changes vector to be unit length
    Type	normalize()
    {
        Type len = length();

        if (len != 0.0)
        (*this) *= (1.0 / len);

        else setValue(0.0, 0.0, 0.0);

        return len;
    }

    // Negates each component of vector in place
    void	negate()
    {
        vec[0] = -vec[0];
        vec[1] = -vec[1];
        vec[2] = -vec[2];
    }

    // Sets value of vector from array of 3 components
    SbVec3<Type> &	setValue(const Type v[3])
     { vec[0] = v[0]; vec[1] = v[1]; vec[2] = v[2]; return *this; }

    // Sets value of vector from 3 individual components
    SbVec3<Type> &	setValue(Type x, Type y, Type z)
     { vec[0] = x; vec[1] = y; vec[2] = z; return *this; }

    // Sets value of vector to be convex combination of 3 other
    // vectors, using barycentic coordinates
    SbVec3<Type> &	setValue(const SbVec3<Type> &barycentric,
        const SbVec3<Type> &v0, const SbVec3<Type> &v1, const SbVec3<Type> &v2)
    {
        *this = v0 * barycentric[0] + v1 * barycentric[1] + v2 * barycentric[2];
        return (*this);
    }

    // Accesses indexed component of vector
    Type &	  operator [](int i) 		{ return (vec[i]); }
    const Type & operator [](int i) const	{ return (vec[i]); }

    // Component-wise scalar multiplication and division operators
    SbVec3<Type> &	operator *=(Type d)
    {
        vec[0] *= d;
        vec[1] *= d;
        vec[2] *= d;

        return *this;
    }

    SbVec3<Type> &	operator /=(Type d)
    { return *this *= (1.0 / d); }

    // Component-wise vector addition and subtraction operators
    SbVec3<Type> &	operator +=(SbVec3<Type> v)
    {
        vec[0] += v.vec[0];
        vec[1] += v.vec[1];
        vec[2] += v.vec[2];

        return *this;
    }
    SbVec3<Type> &	operator -=(SbVec3<Type> v)
    {
        vec[0] -= v.vec[0];
        vec[1] -= v.vec[1];
        vec[2] -= v.vec[2];

        return *this;
    }

    // Nondestructive unary negation - returns a new vector
    SbVec3<Type>	operator -() const
    { return SbVec3<Type>(-vec[0], -vec[1], -vec[2]); }

    // Component-wise binary scalar multiplication and division operators
    friend SbVec3<Type>	operator *(const SbVec3<Type> &v, Type d)
    {
        return SbVec3<Type>(v.vec[0] * d,
                            v.vec[1] * d,
                            v.vec[2] * d);
    }
    friend SbVec3<Type>	operator *(Type d, const SbVec3<Type> &v)
    { return v * d; }
    friend SbVec3<Type>	operator /(const SbVec3<Type> &v, Type d)
    { return v * (1.0 / d); }

    // Component-wise binary vector addition and subtraction operators
    friend SbVec3<Type>	operator +(const SbVec3<Type> &v1, const SbVec3<Type> &v2)
    {
        return SbVec3<Type>(v1.vec[0] + v2.vec[0],
                            v1.vec[1] + v2.vec[1],
                            v1.vec[2] + v2.vec[2]);
    }

    friend SbVec3<Type>	operator -(const SbVec3<Type> &v1, const SbVec3<Type> &v2)
    {
        return SbVec3<Type>(v1.vec[0] - v2.vec[0],
                            v1.vec[1] - v2.vec[1],
                            v1.vec[2] - v2.vec[2]);
    }

    // Equality comparison operator
    friend int		operator ==(const SbVec3<Type> &v1, const SbVec3<Type> &v2)
    {
        return (v1.vec[0] == v2.vec[0] &&
            v1.vec[1] == v2.vec[1] &&
            v1.vec[2] == v2.vec[2]);
    }

    friend int		operator !=(const SbVec3<Type> &v1, const SbVec3<Type> &v2)
    { return !(v1 == v2); }

    // Equality comparison within given tolerance - the square of the
    // length of the maximum distance between the two vectors
    SbBool		equals(const SbVec3<Type> &v, float tolerance) const
    {
        SbVec3<Type>	diff = *this - v;
        return diff.dot(diff) <= tolerance;
    }

    // Returns principal axis that is closest (based on maximum dot
    // product) to this vector
    SbVec3<Type>		getClosestAxis() const;

  protected:
    Type	vec[3];		// Storage for vector components
};

typedef SbVec3<float> SbVec3f;
//class SbVec3f : public SbVec3<float> { using SbVec3::SbVec3; };

//////////////////////////////////////////////////////////////////////////////
//
//  Class: SbVec2f
//
//  2D vector used to represet points or directions. Each component of
//  the vector is a float.
//
//////////////////////////////////////////////////////////////////////////////



class SbVec2f {
  public:

    // Default constructor
    SbVec2f()						{ }

    // Constructor given an array of 2 components
    SbVec2f(const float v[2])				{ setValue(v); }

    // Constructor given 2 individual components
    SbVec2f(float x, float y)				{ setValue(x, y); }

    // Returns dot (inner) product of vector and another vector
    float	dot(const SbVec2f &v) const;

    // Returns pointer to array of 2 components
    const float	*getValue() const			{ return vec; }

    // Returns 2 individual components
    void	getValue(float &x, float &y) const;

    // Returns geometric length of vector
    float	length() const;

    // Negates each component of vector in place
    void	negate();

    // Changes vector to be unit length
    float	normalize();

    // Sets value of vector from array of 2 components
    SbVec2f &	setValue(const float v[2]);

    // Sets value of vector from 2 individual components
    SbVec2f &	setValue(float x, float y);

    // Accesses indexed component of vector
    float &	  operator [](int i) 		{ return (vec[i]); }
    const float & operator [](int i) const 	{ return (vec[i]); }

    // Component-wise scalar multiplication and division operators
    SbVec2f &	operator *=(float d);

    SbVec2f &	operator /=(float d)
    { return *this *= (1.0 / d); }

    // Component-wise vector addition and subtraction operators
    SbVec2f &	operator +=(const SbVec2f &u);
    SbVec2f &	operator -=(const SbVec2f &u);

    // Nondestructive unary negation - returns a new vector
    SbVec2f	operator -() const;

    // Component-wise binary scalar multiplication and division operators
    friend SbVec2f	operator *(const SbVec2f &v, float d);
    friend SbVec2f	operator *(float d, const SbVec2f &v)
    { return v * d; }
    friend SbVec2f	operator /(const SbVec2f &v, float d)
    { return v * (1.0 / d); }

    // Component-wise binary vector addition and subtraction operators
    friend SbVec2f	operator +(const SbVec2f &v1, const SbVec2f &v2);

    friend SbVec2f	operator -(const SbVec2f &v1, const SbVec2f &v2);

    // Equality comparison operator
    friend int		operator ==(const SbVec2f &v1, const SbVec2f &v2);
    friend int		operator !=(const SbVec2f &v1, const SbVec2f &v2)
    { return !(v1 == v2); }

    // Equality comparison within given tolerance - the square of the
    // length of the maximum distance between the two vectors
    SbBool		equals(const SbVec2f v, float tolerance) const;

  protected:
    float	vec[2];		// Storage for vector components
};

//////////////////////////////////////////////////////////////////////////////
//
//  Class: SbVec2s
//
//  2D vector used to represet points or directions. Each component of
//  the vector is a (short) integer.
//
//////////////////////////////////////////////////////////////////////////////



class SbVec2s {
  public:

    // Default constructor
    SbVec2s()						{ }

    // Constructor given an array of 2 components
    SbVec2s(const short v[2])				{ setValue(v); }

    // Constructor given 2 individual components
    SbVec2s(short x, short y)				{ setValue(x, y); }

    // Returns dot (inner) product of vector and another vector
    int32_t	dot(const SbVec2s &v) const;

    // Returns pointer to array of 2 components
    const short	*getValue() const			{ return vec; }

    // Returns 2 individual components
    void	getValue(short &x, short &y) const;

    // Negates each component of vector in place
    void	negate();

    // Sets value of vector from array of 2 components
    SbVec2s &	setValue(const short v[2]);

    // Sets value of vector from 2 individual components
    SbVec2s &	setValue(short x, short y);

    // Accesses indexed component of vector
    short &	  operator [](int i) 		{ return (vec[i]); }
    const short & operator [](int i) const 	{ return (vec[i]); }

    // Component-wise scalar multiplication and division operators
    SbVec2s &	operator *=(int d);
    SbVec2s &	operator *=(double d);

    SbVec2s &	operator /=(int d);
    SbVec2s &	operator /=(double d)
    { return *this *= (1.0 / d); }

    // Component-wise vector addition and subtraction operators
    SbVec2s &	operator +=(const SbVec2s &u);
    SbVec2s &	operator -=(const SbVec2s &u);

    // Nondestructive unary negation - returns a new vector
    SbVec2s	operator -() const;

    // Component-wise binary scalar multiplication and division operators
    friend SbVec2s	operator *(const SbVec2s &v, int d);
    friend SbVec2s	operator *(const SbVec2s &v, double d);
    friend SbVec2s	operator *(int d, const SbVec2s &v)
    { return v * d; }
    friend SbVec2s	operator *(double d, const SbVec2s &v)
    { return v * d; }
    friend SbVec2s	operator /(const SbVec2s &v, int d);
    friend SbVec2s	operator /(const SbVec2s &v, double d)
    { return v * (1.0 / d); }

    // Component-wise binary vector addition and subtraction operators
    friend SbVec2s	operator +(const SbVec2s &v1, const SbVec2s &v2);

    friend SbVec2s	operator -(const SbVec2s &v1, const SbVec2s &v2);

    // Equality comparison operator
    friend int		operator ==(const SbVec2s &v1, const SbVec2s &v2);
    friend int		operator !=(const SbVec2s &v1, const SbVec2s &v2)
    { return !(v1 == v2); }

  protected:
    short	vec[2];		// Storage for vector components
};

//////////////////////////////////////////////////////////////////////////////
//
//  Class: SbVec4f
//
//  4D vector used to represet rational points or directions. Each component of
//  the vector is a float.
//
//////////////////////////////////////////////////////////////////////////////



class SbVec4f {
  public:

    // Default constructor
    SbVec4f()						{ }

    // Constructor given an array of 4 components
    SbVec4f(const float v[4])				{ setValue(v); }

    // Constructor given 4 individual components
    SbVec4f(float x, float y, float z, float w)	      { setValue(x, y, z, w); }

    // Returns dot (inner) product of vector and another vector
    float	dot(const SbVec4f &v) const;

    // Returns the real portion of the vector by dividing by the fourth value
    void	getReal(SbVec3f &v) const;

    // Returns pointer to array of 4 components
    const float	*getValue() const			{ return vec; }

    // Returns 4 individual components
    void	getValue(float &x, float &y, float &z, float &w) const;

    // Returns geometric length of vector
    float	length() const;

    // Negates each component of vector in place
    void	negate();

    // Changes vector to be unit length
    float	normalize();

    // Sets value of vector from array of 4 components
    SbVec4f &	setValue(const float v[4]);

    // Sets value of vector from 4 individual components
    SbVec4f &	setValue(float x, float y, float z, float w);

    // Accesses indexed component of vector
    float &	  operator [](int i) 		{ return (vec[i]); }
    const float & operator [](int i) const	{ return (vec[i]); }

    // Component-wise scalar multiplication and division operators
    SbVec4f &	operator *=(float d);

    SbVec4f &	operator /=(float d)
    { return *this *= (1.0 / d); }

    // Component-wise vector addition and subtraction operators
    SbVec4f &	operator +=(const SbVec4f &u);
    SbVec4f &	operator -=(const SbVec4f &u);

    // Nondestructive unary negation - returns a new vector
    SbVec4f	operator -() const;

    // Component-wise binary scalar multiplication and division operators
    friend SbVec4f	operator *(const SbVec4f &v, float d);
    friend SbVec4f	operator *(float d, const SbVec4f &v)
    { return v * d; }
    friend SbVec4f	operator /(const SbVec4f &v, float d)
    { return v * (1.0 / d); }

    // Component-wise binary vector addition and subtraction operators
    friend SbVec4f	operator +(const SbVec4f &v1, const SbVec4f &v2);

    friend SbVec4f	operator -(const SbVec4f &v1, const SbVec4f &v2);

    // Equality comparison operator
    friend int		operator ==(const SbVec4f &v1, const SbVec4f &v2);
    friend int		operator !=(const SbVec4f &v1, const SbVec4f &v2)
    { return !(v1 == v2); }

    // Equality comparison within given tolerance - the square of the
    // length of the maximum distance between the two vectors
    SbBool		equals(const SbVec4f v, float tolerance) const;

  protected:
    float	vec[4];		// Storage for vector components
};

#endif /* _SB_VEC_ */
