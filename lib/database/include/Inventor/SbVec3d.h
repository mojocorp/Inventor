#ifndef _SB_VEC3D_
#define _SB_VEC3D_

#include <Inventor/SbBasic.h>
#include <Inventor/SbVec3f.h>
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
/// \sa SbVec2f, SbVec3d, SbVec4f, SbVec2s, SbRotation
class INVENTOR_API SbVec3d {
public:
    /// Default constructor
    SbVec3d() { }

    /// Constructor given an array of 3 components
    SbVec3d(const double v[3]) {
        vec[0] = v[0];
        vec[1] = v[1];
        vec[2] = v[2];
    }

    /// Constructor given 3 individual components
    SbVec3d(double x, double y, double z) {
        vec[0] = x;
        vec[1] = y;
        vec[2] = z;
    }

    /// Constructor given 3 planes
    SbVec3d(SbPlane &p0, SbPlane &p1, SbPlane &p2);

    /// Constructs an SbVec3d instance from an SbVec3f instance.
    explicit SbVec3d(const SbVec3f & v) {
        setValue(v);
    }

    /// Returns right-handed cross product of vector and another vector
    SbVec3d cross(const SbVec3d &v) const;

    /// Returns dot (inner) product of vector and another vector
    double dot(const SbVec3d &v) const;

    /// Returns pointer to array of 3 components
    const double *getValue() const {
        return vec;
    }

    /// Returns 3 individual components
    void getValue(double &x, double &y, double &z) const;

    /// Returns geometric length of vector
    double length() const;

    /// Changes vector to be unit length, returning the length before normalization.
    double normalize();

    /// Negates each component of vector in place
    void negate();

    /// Sets value of vector from array of 3 components
    SbVec3d & setValue(const double v[3]) {
        vec[0] = v[0];
        vec[1] = v[1];
        vec[2] = v[2];
        return *this;
    }

    /// Sets value of vector from 3 individual components
    SbVec3d & setValue(double x, double y, double z) {
        vec[0] = x;
        vec[1] = y;
        vec[2] = z;
        return *this;
    }

    /// Sets value of vector to be convex combination of 3 other
    /// vectors, using barycentic coordinates
    SbVec3d & setValue(const SbVec3d &barycentic, const SbVec3d &v0, const SbVec3d &v1, const SbVec3d &v2);

    /// Sets value of vector from a single precision vector.
    SbVec3d &   setValue (const SbVec3f & v) {
        vec[0] = v[0] ;
        vec[1] = v[1] ;
        vec[2] = v[2] ;
        return (*this) ;
    }

    /// Accesses indexed component of vector
    double &   operator [](int i) {
        return (vec[i]);
    }

    /// Accesses indexed component of vector
    const double & operator [](int i) const {
        return (vec[i]);
    }

    /// Component-wise scalar multiplication operator
    SbVec3d & operator *=(double d);

    /// Component-wise scalar division operator
    SbVec3d & operator /=(double d) {
        return *this *= (1.0 / d);
    }

    /// Component-wise vector addition operator
    SbVec3d & operator +=(SbVec3d v);

    /// Component-wise vector subtraction operator
    SbVec3d & operator -=(SbVec3d v);

    /// Nondestructive unary negation - returns a new vector
    SbVec3d operator -() const;

    /// Component-wise binary scalar multiplication operator
    friend INVENTOR_API SbVec3d operator *(const SbVec3d &v, double d);

    /// Component-wise binary scalar multiplication operator
    friend INVENTOR_API SbVec3d operator *(double d, const SbVec3d &v) {
        return v * d;
    }

    /// Component-wise binary scalar division operator
    friend INVENTOR_API SbVec3d operator /(const SbVec3d &v, double d) {
        return v * (1.0f / d);
    }

    /// Component-wise binary vector addition operator
    friend INVENTOR_API SbVec3d operator +(const SbVec3d &v1, const SbVec3d &v2);

    /// Component-wise binary vector subtraction operator
    friend INVENTOR_API SbVec3d operator -(const SbVec3d &v1, const SbVec3d &v2);

    /// Equality comparison operator
    friend INVENTOR_API int  operator ==(const SbVec3d &v1, const SbVec3d &v2);

    /// Inequality comparison operator
    friend INVENTOR_API int  operator !=(const SbVec3d &v1, const SbVec3d &v2) {
        return !(v1 == v2);
    }

    /// Equality comparison within given tolerance - the square of the
    /// length of the maximum distance between the two vectors
    SbBool  equals(const SbVec3d & v, double tolerance) const;

    /// Returns principal axis that is closest (based on maximum dot
    /// product) to this vector
    SbVec3d  getClosestAxis() const;

protected:
    double vec[3];  // Storage for vector components
};

#endif /* _SB_VEC3D_ */
