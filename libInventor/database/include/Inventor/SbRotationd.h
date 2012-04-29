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
 |	SbRotationd
 |
 |   Author(s)		: Paul S. Strauss, Nick Thompson, 
 |			  David Mott, Alain Dumesny
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

#ifndef _SB_ROTATIOND_
#define _SB_ROTATIOND_

#include <Inventor/SbVec4f.h>

class SbMatrix;
class SbMatrixd;
class SbVec3d;

/// Class for representing a rotation.
/// \ingroup Basics
/// Object that stores a rotation. There are several ways to specify a
/// rotation: quaternion (4 doubles), 4x4 rotation matrix, or axis and angle.
/// All angles are in radians and all rotations are right-handed.
/// \par Notes:
/// Rotations are stored internally as quaternions.
/// \sa SbRotation, SbVec3f, SbVec4f, SbMatrix
class INVENTOR_API SbRotationd {
public:

    /// Default constructor
    SbRotationd() {
        setValue(0.0, 0.0, 0.0, 1.0);
    }

    /// Constructor given a quaternion as an array of 4 components
    SbRotationd(const double v[4]) {
        setValue(v);
    }

    /// Constructor given 4 individual components of a quaternion
    SbRotationd(double q0, double q1, double q2, double q3) {
        setValue(q0, q1, q2, q3);
    }

    /// Constructor given a rotation matrix
    SbRotationd(const SbMatrixd &m) {
        setValue(m);
    }

    /// Constructor given 3D rotation axis vector and angle in radians
    SbRotationd(const SbVec3d &axis, double radians) {
        setValue(axis, radians);
    }

    /// Constructor for rotation that rotates one direction vector to another
    SbRotationd(const SbVec3d &rotateFrom, const SbVec3d &rotateTo) {
        setValue(rotateFrom, rotateTo);
    }

    /// Returns pointer to array of 4 components defining quaternion
    const double * getValue() const {
        return (quat);
    }

    /// Returns 4 individual components of rotation quaternion
    void  getValue(double &q0, double &q1, double &q2, double &q3) const;

    /// Returns corresponding 3D rotation axis vector and angle in radians
    void  getValue(SbVec3d &axis, double &radians) const;

    /// Returns corresponding 4x4 rotation matrix
    void  getValue(SbMatrix &matrix) const;

    /// Returns corresponding 4x4 rotation matrix
    void  getValue(SbMatrixd &matrix) const;

    /// Changes a rotation to be its inverse
    SbRotationd & invert();

    /// Returns the inverse of a rotation
    SbRotationd  inverse() const {
        SbRotationd q = *this;
        return q.invert();
    }

    /// Sets value of rotation from array of 4 components of a quaternion
    SbRotationd & setValue(const double q[4]);

    /// Sets value of rotation from 4 individual components of a quaternion
    SbRotationd & setValue(double q0, double q1, double q2, double q3);

    /// Sets value of rotation from a rotation matrix
    /// I don't know what will happen if you call this with something
    /// that isn't a rotation.
    SbRotationd & setValue(const SbMatrix &m);

    /// Sets value of rotation from a rotation matrix
    SbRotationd & setValue(const SbMatrixd &m);

    /// Sets value of vector from 3D rotation axis vector and angle in radians
    SbRotationd & setValue(const SbVec3d &axis, double radians);

    /// Sets rotation to rotate one direction vector to another
    SbRotationd & setValue(const SbVec3d &rotateFrom,
                          const SbVec3d &rotateTo);

    /// Multiplies by another rotation; results in product of rotations
    SbRotationd &  operator *=(const SbRotationd &q);

    /// Equality comparison operator
    friend INVENTOR_API int operator ==(const SbRotationd &q1, const SbRotationd &q2);

    /// Inequality comparison operator
    friend INVENTOR_API int operator !=(const SbRotationd &q1, const SbRotationd &q2) {
        return !(q1 == q2);
    }

    /// Equality comparison within given tolerance - the square of the
    /// length of the maximum distance between the two quaternion vectors
    SbBool  equals(const SbRotationd &r, double tolerance) const;

    /// Multiplication of two rotations; results in product of rotations
    friend INVENTOR_API SbRotationd operator *(const SbRotationd &q1, const SbRotationd &q2);

    /// Puts the given vector through this rotation
    /// (Multiplies the given vector by the matrix of this rotation),.
    void multVec(const SbVec3d &src, SbVec3d &dst) const;

    /// Keep the axis the same. Multiply the angle of rotation by
    /// the amount 'scaleFactor'
    void scaleAngle( double scaleFactor );

    /// Spherical linear interpolation: as t goes from 0 to 1, returned
    /// value goes from rot0 to rot1
    static SbRotationd slerp(const SbRotationd &rot0,
                            const SbRotationd &rot1, double t);

    /// Returns a null rotation.
    static SbRotationd identity() {
        return SbRotationd(0.0, 0.0, 0.0, 1.0);
    }

private:
    double quat[4]; // Storage for quaternion components

    // Returns the norm (square of the 4D length) of a rotation's quaterion
    double norm() const;

    // Normalizes a rotation quaternion to unit 4D length
    void normalize();
};

#endif /* _SB_ROTATIOND_ */
