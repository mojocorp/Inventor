#ifndef _SB_VEC2I32_
#define _SB_VEC2I32_

#include <Inventor/SbBasic.h>

/// 2D vector class.
/// \ingroup Basics
/// 2D vector class used to store 2D integer vectors and points. This class is used
/// throughout Inventor for arguments and return values.
/// \sa SbVec3f, SbVec4f, SbVec2f
class INVENTOR_API SbVec2i32 {
  public:

    /// Default constructor
    SbVec2i32()						{ }

    /// Constructor given an array of 2 components
    SbVec2i32(const int32_t v[2])				{ setValue(v); }

    /// Constructor given 2 individual components
    SbVec2i32(int32_t x, int32_t y)				{ setValue(x, y); }

    /// Returns dot (inner) product of vector and another vector
    int32_t	dot(const SbVec2i32 &v) const;

    /// Returns pointer to array of 2 components
    const int32_t	*getValue() const			{ return vec; }

    /// Returns 2 individual components
    void	getValue(int32_t &x, int32_t &y) const;

    /// Negates each component of vector in place
    void	negate();

    /// Sets value of vector from array of 2 components
    SbVec2i32 &	setValue(const int32_t v[2]);

    /// Sets value of vector from 2 individual components
    SbVec2i32 &	setValue(int32_t x, int32_t y);

    /// Accesses indexed component of vector
    int32_t &	  operator [](int i) 		{ return (vec[i]); }

    /// Accesses indexed component of vector
    const int32_t & operator [](int i) const 	{ return (vec[i]); }

    /// Component-wise scalar multiplication operator
    SbVec2i32 &	operator *=(int d);

    /// Component-wise scalar multiplication operator
    SbVec2i32 &	operator *=(double d);

    /// Component-wise scalar division operator
    SbVec2i32 &	operator /=(int d);

    /// Component-wise scalar division operator
    SbVec2i32 &	operator /=(double d)
	{ return *this *= (1.0 / d); }

    /// Component-wise vector addition operator
    SbVec2i32 &	operator +=(const SbVec2i32 &u);

    /// Component-wise vector subtraction operator
    SbVec2i32 &	operator -=(const SbVec2i32 &u);

    /// Nondestructive unary negation - returns a new vector
    SbVec2i32	operator -() const;

    /// Component-wise binary scalar multiplication operator
    friend INVENTOR_API SbVec2i32	operator *(const SbVec2i32 &v, int d);

    /// Component-wise binary scalar multiplication operator
    friend INVENTOR_API SbVec2i32	operator *(const SbVec2i32 &v, double d);

    /// Component-wise binary scalar multiplication operator
    friend INVENTOR_API SbVec2i32	operator *(int d, const SbVec2i32 &v) { return v * d; }

    /// Component-wise binary scalar multiplication operator
    friend INVENTOR_API SbVec2i32	operator *(double d, const SbVec2i32 &v) { return v * d; }

    /// Component-wise binary scalar division operator
    friend INVENTOR_API SbVec2i32	operator /(const SbVec2i32 &v, int d);

    /// Component-wise binary scalar division operator
    friend INVENTOR_API SbVec2i32	operator /(const SbVec2i32 &v, double d) { return v * (1.0 / d); }

    /// Component-wise binary vector addition operator
    friend INVENTOR_API SbVec2i32	operator +(const SbVec2i32 &v1, const SbVec2i32 &v2);

    /// Component-wise binary vector subtraction operator
    friend INVENTOR_API SbVec2i32	operator -(const SbVec2i32 &v1, const SbVec2i32 &v2);

    /// Equality comparison operator
    friend INVENTOR_API int		operator ==(const SbVec2i32 &v1, const SbVec2i32 &v2);
    
    /// Inequality comparison operator
    friend INVENTOR_API int		operator !=(const SbVec2i32 &v1, const SbVec2i32 &v2) { return !(v1 == v2); }

  protected:
    int32_t	vec[2];		// Storage for vector components
};

#endif /* _SB_VEC2S_ */
