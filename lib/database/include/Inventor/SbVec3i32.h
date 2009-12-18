#ifndef _SB_VEC3I32_
#define _SB_VEC3I32_

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
/// \sa SbVec2f, SbVec4f, SbVec2s, SbRotation
class INVENTOR_API SbVec3i32 {
  public:
    /// Default constructor
    SbVec3i32()						{ }

    /// Constructor given an array of 3 components
    SbVec3i32(const int32_t v[3])
	 { vec[0] = v[0]; vec[1] = v[1]; vec[2] = v[2]; }

    /// Constructor given 3 individual components
    SbVec3i32(int32_t x, int32_t y, int32_t z)
	 { vec[0] = x; vec[1] = y; vec[2] = z; }

    /// Returns right-handed cross product of vector and another vector
    SbVec3i32	cross(const SbVec3i32 &v) const;

    /// Returns dot (inner) product of vector and another vector
    int32_t	dot(const SbVec3i32 &v) const;

    /// Returns pointer to array of 3 components
    const int32_t	*getValue() const			{ return vec; }

    /// Returns 3 individual components
    void	getValue(int32_t &x, int32_t &y, int32_t &z) const;

    /// Negates each component of vector in place
    void	negate();

    /// Sets value of vector from array of 3 components
    SbVec3i32 &	setValue(const int32_t v[3])
	 { vec[0] = v[0]; vec[1] = v[1]; vec[2] = v[2]; return *this; }

    /// Sets value of vector from 3 individual components
    SbVec3i32 &	setValue(int32_t x, int32_t y, int32_t z)
	 { vec[0] = x; vec[1] = y; vec[2] = z; return *this; }

    /// Sets value of vector to be convex combination of 3 other
    /// vectors, using barycentic coordinates
    SbVec3i32 &	setValue(const SbVec3i32 &barycentic, const SbVec3i32 &v0, const SbVec3i32 &v1, const SbVec3i32 &v2);

    /// Accesses indexed component of vector
    int32_t &	  operator [](int i) 		{ return (vec[i]); }

    /// Accesses indexed component of vector
    const int32_t & operator [](int i) const	{ return (vec[i]); }

    /// Component-wise scalar multiplication operator
    SbVec3i32 &	operator *=(int32_t d);

    /// Component-wise scalar multiplication operator
    SbVec3i32 &	operator *=(double d);

    /// Component-wise scalar division operator
    SbVec3i32 &	operator /=(int32_t d) { return *this *= (1.0 / d); }

    /// Component-wise scalar division operator
    SbVec3i32 &	operator /=(double d) { return *this *= (1.0 / d); }

    /// Component-wise vector addition operator
    SbVec3i32 &	operator +=(SbVec3i32 v);

    /// Component-wise vector subtraction operator
    SbVec3i32 &	operator -=(SbVec3i32 v);

    /// Nondestructive unary negation - returns a new vector
    SbVec3i32	operator -() const;

    /// Component-wise binary scalar multiplication operator
    friend INVENTOR_API SbVec3i32	operator *(const SbVec3i32 &v, int32_t d);

    /// Component-wise binary scalar multiplication operator
    friend INVENTOR_API SbVec3i32	operator *(int32_t d, const SbVec3i32 &v) { return v * d; }

    /// Component-wise binary scalar division operator
    friend INVENTOR_API SbVec3i32	operator /(const SbVec3i32 &v, int32_t d) { SbVec3i32 val(v); val /= d; return val; }

    /// Component-wise binary vector addition operator
    friend INVENTOR_API SbVec3i32	operator +(const SbVec3i32 &v1, const SbVec3i32 &v2);

    /// Component-wise binary vector subtraction operator
    friend INVENTOR_API SbVec3i32	operator -(const SbVec3i32 &v1, const SbVec3i32 &v2);

    /// Equality comparison operator
    friend INVENTOR_API int		operator ==(const SbVec3i32 &v1, const SbVec3i32 &v2);

    /// Inequality comparison operator
    friend INVENTOR_API int		operator !=(const SbVec3i32 &v1, const SbVec3i32 &v2) { return !(v1 == v2); }

    /// Equality comparison within given tolerance - the square of the
    /// length of the maximum distance between the two vectors
    SbBool		equals(const SbVec3i32 & v, int32_t tolerance) const;

  protected:
    int32_t	vec[3];		// Storage for vector components
};

#endif /* _SB_VEC3I32_ */
