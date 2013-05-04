#include <Inventor/SbVec3d.h>
#include <Inventor/SbPlane.h>

#include <math.h>

// amount squared to figure if two doubles are equal
// (used for Vec2f and Vec3f right now)
#define DELTA 1e-6

//
// constructor that creates vector from intersection of three planes
//

 #define DET3(m) (( 			\
     m[0][0] * m[1][1] * m[2][2]	\
   + m[0][1] * m[1][2] * m[2][0]	\
   + m[0][2] * m[1][0] * m[2][1]	\
   - m[2][0] * m[1][1] * m[0][2]	\
   - m[2][1] * m[1][2] * m[0][0]	\
   - m[2][2] * m[1][0] * m[0][1]))

 SbVec3d::SbVec3d(SbPlane &p0, SbPlane &p1, SbPlane &p2)
 {
     double	v[3], del, mx[3][3];

     // create 3x3 matrix of normal coefficients
     mx[0][0] = p0.getNormal()[0];
     mx[0][1] = p0.getNormal()[1];
     mx[0][2] = p0.getNormal()[2];
     mx[1][0] = p1.getNormal()[0];
     mx[1][1] = p1.getNormal()[1];
     mx[1][2] = p1.getNormal()[2];
     mx[2][0] = p2.getNormal()[0];
     mx[2][1] = p2.getNormal()[1];
     mx[2][2] = p2.getNormal()[2];

     // find determinant of matrix to use for divisor
     del = DET3(mx);

     //    printf("mx = %10.5f %10.5f %10.5f\n", mx[0][0], mx[0][1], mx[0][2]);
     //    printf("     %10.5f %10.5f %10.5f\n", mx[1][0], mx[1][1], mx[1][2]);
     //    printf("     %10.5f %10.5f %10.5f\n", mx[2][0], mx[2][1], mx[2][2]);
     if(del > -DELTA && del < DELTA) {	// if singular, just set to the origin
         vec[0] = 0;
         vec[1] = 0;
         vec[2] = 0;
     }
     else {
         v[0] = p0.getDistanceFromOrigin();
         v[1] = p1.getDistanceFromOrigin();
         v[2] = p2.getDistanceFromOrigin();

         //	printf("v = %10.5f\n    %10.5f\n    %10.5f\n", v[0], v[1], v[2]);
         double mi[3][3];
         mi[0][0] = v[0]; mi[0][1] = mx[0][1]; mi[0][2] = mx[0][2];
         mi[1][0] = v[1]; mi[1][1] = mx[1][1]; mi[1][2] = mx[1][2];
         mi[2][0] = v[2]; mi[2][1] = mx[2][1]; mi[2][2] = mx[2][2];

         //	printf("mi = %10.5f %10.5f %10.5f\n", mi[0][0], mi[0][1], mi[0][2]);
         //	printf("     %10.5f %10.5f %10.5f\n", mi[1][0], mi[1][1], mi[1][2]);
         //	printf("     %10.5f %10.5f %10.5f\n", mi[2][0], mi[2][1], mi[2][2]);

         vec[0] = DET3(mi) / del;
         mi[0][0] = mx[0][0]; mi[0][1] = v[0]; mi[0][2] = mx[0][2];
         mi[1][0] = mx[1][0]; mi[1][1] = v[1]; mi[1][2] = mx[1][2];
         mi[2][0] = mx[2][0]; mi[2][1] = v[2]; mi[2][2] = mx[2][2];

         //	printf("mi = %10.5f %10.5f %10.5f\n", mi[0][0], mi[0][1], mi[0][2]);
         //	printf("     %10.5f %10.5f %10.5f\n", mi[1][0], mi[1][1], mi[1][2]);
         //	printf("     %10.5f %10.5f %10.5f\n", mi[2][0], mi[2][1], mi[2][2]);

         vec[1] = DET3(mi) / del;
         mi[0][0] = mx[0][0]; mi[0][1] = mx[0][1]; mi[0][2] = v[0];
         mi[1][0] = mx[1][0]; mi[1][1] = mx[1][1]; mi[1][2] = v[1];
         mi[2][0] = mx[2][0]; mi[2][1] = mx[2][1]; mi[2][2] = v[2];

         //	printf("mi = %10.5f %10.5f %10.5f\n", mi[0][0], mi[0][1], mi[0][2]);
         //	printf("     %10.5f %10.5f %10.5f\n", mi[1][0], mi[1][1], mi[1][2]);
         //	printf("     %10.5f %10.5f %10.5f\n", mi[2][0], mi[2][1], mi[2][2]);

         vec[2] = DET3(mi) / del;
     }

     //    printf("%10.5f %10.5f %10.5f\n", vec[0], vec[1], vec[2]);
 }

 //
 // Returns right-handed cross product of vector and another vector
 //

 SbVec3d
 SbVec3d::cross(const SbVec3d &v) const
 {
     return SbVec3d(vec[1] * v.vec[2] - vec[2] * v.vec[1],
 		  vec[2] * v.vec[0] - vec[0] * v.vec[2],
 		  vec[0] * v.vec[1] - vec[1] * v.vec[0]);
 }

 //
 // Returns dot (inner) product of vector and another vector
 //

 double
 SbVec3d::dot(const SbVec3d &v) const
 {
     return (vec[0] * v.vec[0] +
 	    vec[1] * v.vec[1] +
 	    vec[2] * v.vec[2]);
 }

 //
 // Returns 3 individual components
 //

 void
 SbVec3d::getValue(double &x, double &y, double &z) const
 {
     x = vec[0];
     y = vec[1];
     z = vec[2];
 }

 //
 // Returns geometric length of vector
 //

 double
 SbVec3d::length() const
 {
     return sqrt(vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2]);
 }

 //
 // Negates each component of vector in place
 //

 void
 SbVec3d::negate()
 {
     vec[0] = -vec[0];
     vec[1] = -vec[1];
     vec[2] = -vec[2];
 }

 //
 // Changes vector to be unit length
 //

 double
 SbVec3d::normalize()
 {
     double len = length();

     if (len != 0.0f)
 	(*this) *= (1.0f / len);

     else setValue(0.0f, 0.0f, 0.0f);

     return len;
 }

 SbVec3d &
 SbVec3d::setValue(const SbVec3d &barycentric,
                  const SbVec3d &v0, const SbVec3d &v1, const SbVec3d &v2)
 {
     *this = v0 * barycentric[0] + v1 * barycentric[1] + v2 * barycentric[2];
     return (*this);
 }


 //
 // Component-wise scalar multiplication operator
 //

 SbVec3d &
 SbVec3d::operator *=(double d)
 {
     vec[0] *= d;
     vec[1] *= d;
     vec[2] *= d;

     return *this;
 }

 //
 // Component-wise vector addition operator
 //

 SbVec3d &
 SbVec3d::operator +=(SbVec3d v)
 {
     vec[0] += v.vec[0];
     vec[1] += v.vec[1];
     vec[2] += v.vec[2];

     return *this;
 }

 //
 // Component-wise vector subtraction operator
 //

 SbVec3d &
 SbVec3d::operator -=(SbVec3d v)
 {
     vec[0] -= v.vec[0];
     vec[1] -= v.vec[1];
     vec[2] -= v.vec[2];

     return *this;
 }

 //
 // Nondestructive unary negation - returns a new vector
 //

 SbVec3d
 SbVec3d::operator -() const
 {
     return SbVec3d(-vec[0], -vec[1], -vec[2]);
 }

 //
 // Component-wise binary scalar multiplication operator
 //

 SbVec3d
 operator *(const SbVec3d &v, double d)
 {
     return SbVec3d(v.vec[0] * d,
 		  v.vec[1] * d,
 		  v.vec[2] * d);
 }

 //
 // Component-wise binary vector addition operator
 //

 SbVec3d
 operator +(const SbVec3d &v1, const SbVec3d &v2)
 {
     return SbVec3d(v1.vec[0] + v2.vec[0],
 		  v1.vec[1] + v2.vec[1],
 		  v1.vec[2] + v2.vec[2]);
 }

 //
 // Component-wise binary vector subtraction operator
 //

 SbVec3d
 operator -(const SbVec3d &v1, const SbVec3d &v2)
 {
     return SbVec3d(v1.vec[0] - v2.vec[0],
 		   v1.vec[1] - v2.vec[1],
 		   v1.vec[2] - v2.vec[2]);
 }

 //
 // Equality comparison operator. Componenents must match exactly.
 //

 int
 operator ==(const SbVec3d &v1, const SbVec3d &v2)
 {
     return (v1.vec[0] == v2.vec[0] &&
 	    v1.vec[1] == v2.vec[1] &&
 	    v1.vec[2] == v2.vec[2]);
 }

 //
 // Equality comparison operator within a tolerance.
 //

 SbBool
 SbVec3d::equals(const SbVec3d & v, double tolerance) const
 {
     for (int i=0; i<3; i++) {
         double diff = vec[i] - v.vec[i];
         if ((diff < 0.0 ? -diff : diff) > tolerance)
            return FALSE;
     }

     return TRUE;
 }

 //
 // Returns principal axis that is closest (based on maximum dot
 // product) to this vector.
 //

 SbVec3d
 SbVec3d::getClosestAxis() const
 {
     SbVec3d	axis(0.0f, 0.0f, 0.0f), bestAxis;
     double	d, max = -21.234f;

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
