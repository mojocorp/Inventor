#include <Inventor/SbVec3i32.h>
#include <Inventor/SbPlane.h>

#include <math.h>

// amount squared to figure if two int32_ts are equal
// (used for Vec2f and Vec3f right now)
#define DELTA 1e-6

 //
 // Returns right-handed cross product of vector and another vector
 //

 SbVec3i32
 SbVec3i32::cross(const SbVec3i32 &v) const
 {
     return SbVec3i32(vec[1] * v.vec[2] - vec[2] * v.vec[1],
 		  vec[2] * v.vec[0] - vec[0] * v.vec[2],
 		  vec[0] * v.vec[1] - vec[1] * v.vec[0]);
 }

 //
 // Returns dot (inner) product of vector and another vector
 //

 int32_t
 SbVec3i32::dot(const SbVec3i32 &v) const
 {
     return (vec[0] * v.vec[0] +
 	    vec[1] * v.vec[1] +
 	    vec[2] * v.vec[2]);
 }

 //
 // Returns 3 individual components
 //

 void
 SbVec3i32::getValue(int32_t &x, int32_t &y, int32_t &z) const
 {
     x = vec[0];
     y = vec[1];
     z = vec[2];
 }

 //
 // Returns geometric length of vector
 //

 int32_t
 SbVec3i32::length() const
 {
     return sqrtf(vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2]);
 }

 //
 // Negates each component of vector in place
 //

 void
 SbVec3i32::negate()
 {
     vec[0] = -vec[0];
     vec[1] = -vec[1];
     vec[2] = -vec[2];
 }

 //
 // Changes vector to be unit length
 //

 int32_t
 SbVec3i32::normalize()
 {
     int32_t len = length();

     if (len != 0.0f)
 	(*this) *= (1.0f / len);

     else setValue(0.0f, 0.0f, 0.0f);

     return len;
 }

 SbVec3i32 &
 SbVec3i32::setValue(const SbVec3i32 &barycentric,
                  const SbVec3i32 &v0, const SbVec3i32 &v1, const SbVec3i32 &v2)
 {
     *this = v0 * barycentric[0] + v1 * barycentric[1] + v2 * barycentric[2];
     return (*this);
 }


 //
 // Component-wise scalar multiplication operator
 //

 SbVec3i32 &
 SbVec3i32::operator *=(int32_t d)
 {
     vec[0] *= d;
     vec[1] *= d;
     vec[2] *= d;

     return *this;
 }

 //
 // Component-wise vector addition operator
 //

 SbVec3i32 &
 SbVec3i32::operator +=(SbVec3i32 v)
 {
     vec[0] += v.vec[0];
     vec[1] += v.vec[1];
     vec[2] += v.vec[2];

     return *this;
 }

 //
 // Component-wise vector subtraction operator
 //

 SbVec3i32 &
 SbVec3i32::operator -=(SbVec3i32 v)
 {
     vec[0] -= v.vec[0];
     vec[1] -= v.vec[1];
     vec[2] -= v.vec[2];

     return *this;
 }

 //
 // Nondestructive unary negation - returns a new vector
 //

 SbVec3i32
 SbVec3i32::operator -() const
 {
     return SbVec3i32(-vec[0], -vec[1], -vec[2]);
 }

 //
 // Component-wise binary scalar multiplication operator
 //

 SbVec3i32
 operator *(const SbVec3i32 &v, int32_t d)
 {
     return SbVec3i32(v.vec[0] * d,
 		  v.vec[1] * d,
 		  v.vec[2] * d);
 }

 //
 // Component-wise binary vector addition operator
 //

 SbVec3i32
 operator +(const SbVec3i32 &v1, const SbVec3i32 &v2)
 {
     return SbVec3i32(v1.vec[0] + v2.vec[0],
 		  v1.vec[1] + v2.vec[1],
 		  v1.vec[2] + v2.vec[2]);
 }

 //
 // Component-wise binary vector subtraction operator
 //

 SbVec3i32
 operator -(const SbVec3i32 &v1, const SbVec3i32 &v2)
 {
     return SbVec3i32(v1.vec[0] - v2.vec[0],
 		   v1.vec[1] - v2.vec[1],
 		   v1.vec[2] - v2.vec[2]);
 }

 //
 // Equality comparison operator. Componenents must match exactly.
 //

 int
 operator ==(const SbVec3i32 &v1, const SbVec3i32 &v2)
 {
     return (v1.vec[0] == v2.vec[0] &&
 	    v1.vec[1] == v2.vec[1] &&
 	    v1.vec[2] == v2.vec[2]);
 }

 //
 // Equality comparison operator within a tolerance.
 //

 SbBool
 SbVec3i32::equals(const SbVec3i32 & v, int32_t tolerance) const
 {
     SbVec3i32	diff = *this - v;

     return diff.dot(diff) <= tolerance;
 }

 //
 // Returns principal axis that is closest (based on maximum dot
 // product) to this vector.
 //

 SbVec3i32
 SbVec3i32::getClosestAxis() const
 {
     SbVec3i32	axis(0.0f, 0.0f, 0.0f), bestAxis;
     int32_t	d, max = -21.234f;

 #define TEST_AXIS()							      \
     if ((d = dot(axis)) > max) {					      \
 	max = d;							      \
 	bestAxis = axis;						      \
     }

     axis[0] = 1.0;	// +x axis
     TEST_AXIS();

     axis[0] = -1.0;	// -x axis
     TEST_AXIS();
     axis[0] = 0.0;

     axis[1] = 1.0;	// +y axis
     TEST_AXIS();

     axis[1] = -1.0;	// -y axis
     TEST_AXIS();
     axis[1] = 0.0;

     axis[2] = 1.0;	// +z axis
     TEST_AXIS();

     axis[2] = -1.0;	// -z axis
     TEST_AXIS();

 #undef TEST_AXIS

     return bestAxis;
 }
