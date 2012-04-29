#include <Inventor/SbVec2i32.h>

//
// Returns dot (inner) product of vector and another vector
//

int32_t
SbVec2i32::dot(const SbVec2i32 &v) const
{
    return vec[0] * v.vec[0] + vec[1] * v.vec[1];
}

//
// Returns 2 individual components
//

void
SbVec2i32::getValue(int32_t &x, int32_t &y) const
{
    x = vec[0];
    y = vec[1];
}

//
// Negates each component of vector in place
//

void
SbVec2i32::negate()
{
    vec[0] = -vec[0];
    vec[1] = -vec[1];
}

//
// Sets value of vector from array of 2 components
//

SbVec2i32 &
SbVec2i32::setValue(const int32_t v[2])
{
    vec[0] = v[0];
    vec[1] = v[1];

    return (*this);
}

//
// Sets value of vector from 2 individual components
//

SbVec2i32 &
SbVec2i32::setValue(int32_t x, int32_t y)
{
    vec[0] = x;
    vec[1] = y;

    return (*this);
}

//
// Component-wise scalar multiplication operator
//
// Note: didn't complain about assignment of int to int32_t !
SbVec2i32 &
SbVec2i32::operator *=(int d)
{
    vec[0] *= d;
    vec[1] *= d;

    return *this;
}

SbVec2i32 &
SbVec2i32::operator *=(double d)
{
    vec[0] = int32_t(vec[0] * d);
    vec[1] = int32_t(vec[1] * d);

    return *this;
}

//
// Component-wise scalar division operator
//

SbVec2i32 &
SbVec2i32::operator /=(int d)
{
    vec[0] /= d;
    vec[1] /= d;

    return *this;
}

//
// Component-wise vector addition operator
//

SbVec2i32 &
SbVec2i32::operator +=(const SbVec2i32 &u)
{
    vec[0] += u.vec[0];
    vec[1] += u.vec[1];

    return *this;
}

//
// Component-wise vector subtraction operator
//

SbVec2i32 &
SbVec2i32::operator -=(const SbVec2i32 &u)
{
    vec[0] -= u.vec[0];
    vec[1] -= u.vec[1];

    return *this;
}

//
// Nondestructive unary negation - returns a new vector
//

SbVec2i32
SbVec2i32::operator -() const
{
    return SbVec2i32(-vec[0], -vec[1]);
}


//
// Component-wise binary scalar multiplication operator
//

SbVec2i32
operator *(const SbVec2i32 &v, int d)
{
    return SbVec2i32(v.vec[0] * d, v.vec[1] * d);
}

SbVec2i32
operator *(const SbVec2i32 &v, double d)
{
    return SbVec2i32(int32_t(v.vec[0] * d), int32_t(v.vec[1] * d));
}

//
// Component-wise binary scalar division operator
//

SbVec2i32
operator /(const SbVec2i32 &v, int d)
{
    return SbVec2i32(v.vec[0] / d, v.vec[1] / d);
}

//
// Component-wise binary vector addition operator
//

SbVec2i32
operator +(const SbVec2i32 &v1, const SbVec2i32 &v2)
{
    return SbVec2i32(v1.vec[0] + v2.vec[0],
                   v1.vec[1] + v2.vec[1]);
}

//
// Component-wise binary vector subtraction operator
//

SbVec2i32
operator -(const SbVec2i32 &v1, const SbVec2i32 &v2)
{
    return SbVec2i32(v1.vec[0] - v2.vec[0],
                   v1.vec[1] - v2.vec[1]);
}

//
// Equality comparison operator
//

int
operator ==(const SbVec2i32 &v1, const SbVec2i32 &v2)
{
    return (v1.vec[0] == v2.vec[0] &&
 	    v1.vec[1] == v2.vec[1]);
}
