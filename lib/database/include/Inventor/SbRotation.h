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
 |	SbRotation
 |
 |   Author(s)		: Paul S. Strauss, Nick Thompson,
 |			  David Mott, Alain Dumesny
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

#pragma once

#include <Inventor/SbVec.h>

class SbMatrix;

/// Class for representing a rotation.
/// \ingroup Basics
/// Object that stores a rotation. There are several ways to specify a
/// rotation: quaternion (4 floats), 4x4 rotation matrix, or axis and angle.
/// All angles are in radians and all rotations are right-handed.
/// \par Notes:
/// Rotations are stored internally as quaternions.
/// \sa SbVec3f, SbMatrix
class SbRotation {
  public:
    /// Default constructor
    SbRotation() {}

    /// Constructor given a quaternion as an array of 4 components
    SbRotation(const float v[4]) { setValue(v); }

    /// Constructor given 4 individual components of a quaternion
    SbRotation(float q0, float q1, float q2, float q3) {
        setValue(q0, q1, q2, q3);
    }

    /// Constructor given a rotation matrix
    SbRotation(const SbMatrix &m) { setValue(m); }

    /// Constructor given 3D rotation axis vector and angle in radians
    SbRotation(const SbVec3f &axis, float radians) { setValue(axis, radians); }

    /// Constructor for rotation that rotates one direction vector to another
    SbRotation(const SbVec3f &rotateFrom, const SbVec3f &rotateTo) {
        setValue(rotateFrom, rotateTo);
    }

    /// Returns pointer to array of 4 components defining quaternion
    const float *getValue() const { return (quat); }

    /// Returns 4 individual components of rotation quaternion
    void getValue(float &q0, float &q1, float &q2, float &q3) const;

    /// Returns corresponding 3D rotation axis vector and angle in radians
    void getValue(SbVec3f &axis, float &radians) const;

    /// Returns corresponding 4x4 rotation matrix
    void getValue(SbMatrix &matrix) const;

    /// Changes a rotation to be its inverse
    SbRotation &invert();

    /// Returns the inverse of a rotation
    SbRotation inverse() const {
        SbRotation q = *this;
        return q.invert();
    }

    /// Sets value of rotation from array of 4 components of a quaternion
    SbRotation &setValue(const float q[4]);

    /// Sets value of rotation from 4 individual components of a quaternion
    SbRotation &setValue(float q0, float q1, float q2, float q3);

    /// Sets value of rotation from a rotation matrix
    /// I don't know what will happen if you call this with something
    /// that isn't a rotation.
    SbRotation &setValue(const SbMatrix &m);

    /// Sets value of vector from 3D rotation axis vector and angle in radians
    SbRotation &setValue(const SbVec3f &axis, float radians);

    /// Sets rotation to rotate one direction vector to another
    SbRotation &setValue(const SbVec3f &rotateFrom, const SbVec3f &rotateTo);

    /// Multiplies by another rotation; results in product of rotations
    SbRotation &operator*=(const SbRotation &q);

    /// Equality comparison operator
    friend int operator==(const SbRotation &q1, const SbRotation &q2);

    /// Inequality comparison operator
    friend int operator!=(const SbRotation &q1, const SbRotation &q2) {
        return !(q1 == q2);
    }

    /// Equality comparison within given tolerance - the square of the
    /// length of the maximum distance between the two quaternion vectors
    SbBool equals(const SbRotation &r, float tolerance) const;

    /// Multiplication of two rotations; results in product of rotations
    friend SbRotation operator*(const SbRotation &q1, const SbRotation &q2);

    /// Puts the given vector through this rotation
    /// (Multiplies the given vector by the matrix of this rotation),.
    void multVec(const SbVec3f &src, SbVec3f &dst) const;

    /// Keep the axis the same. Multiply the angle of rotation by
    /// the amount 'scaleFactor'
    void scaleAngle(float scaleFactor);

    /// Spherical linear interpolation: as t goes from 0 to 1, returned
    /// value goes from rot0 to rot1
    static SbRotation slerp(const SbRotation &rot0, const SbRotation &rot1,
                            float t);

    /// Returns a null rotation.
    static SbRotation identity() { return SbRotation(0.0, 0.0, 0.0, 1.0); }

  private:
    float quat[4]; // Storage for quaternion components

    // Returns the norm (square of the 4D length) of a rotation's quaterion
    float norm() const;

    // Normalizes a rotation quaternion to unit 4D length
    void normalize();
};

