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
 |	This file contains definitions of SbMatrixd
 |
 |   Classes:
 |	SbMatrixd
 |
 |   Author(s)		: Paul S. Strauss, Nick Thompson, 
 |			  David Mott, Alain Dumesny
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

#ifndef _SB_MATRIXD_
#define _SB_MATRIXD_

#include <Inventor/SbBasic.h>
#include <Inventor/SbMatrix.h>

class SbLine;
class SbRotation;
class SbVec3d;

typedef double SbMatd[4][4];

/// 4x4 matrix class (double precision).
/// \ingroup Basics
/// 4x4 matrix class/datatype used by many Inventor node and action classes.
/// The matrices are stored in row-major order.
/// \sa SbVec3d, SbRotation
class INVENTOR_API SbMatrixd {
  public:

    /// Default constructor
    SbMatrixd()						{ }

    /// Constructor given all 16 elements in row-major order
    SbMatrixd(double a11, double a12, double a13, double a14,
             double a21, double a22, double a23, double a24, 
             double a31, double a32, double a33, double a34, 
             double a41, double a42, double a43, double a44);

    /// Constructor from a 4x4 array of elements
    SbMatrixd(const SbMatd &m);

    /// Sets value from 4x4 array of elements
    void	setValue(const SbMatd &m);
    
    void	setValue(const SbMatrix &m);

    /// Sets matrix to be identity
    void	makeIdentity();

    /// Returns an identity matrix
    static SbMatrixd	identity();

    /// Sets matrix to rotate by given rotation
    void	setRotate(const SbRotation &q);

    /// Sets matrix to scale by given uniform factor
    void	setScale(double s);

    /// Sets matrix to scale by given vector
    void	setScale(const SbVec3d &s);

    /// Sets matrix to translate by given vector
    void	setTranslate(const SbVec3d &t);

    /// Composes the matrix based on a translation, rotation, scale,
    /// orientation for scale, and center.  The "center" is the
    /// center point for scaling and rotation.  The "scaleOrientation"
    /// chooses the primary axes for the scale.
    void	setTransform(
			const SbVec3d &translation,
			const SbRotation &rotation,
			const SbVec3d &scaleFactor,
			const SbRotation &scaleOrientation,
			const SbVec3d &center);
    /// Overloaded methods as a kludge because the compiler won't let
    /// us have SbVec3d(0,0,0) as a default value:
    void	setTransform(const SbVec3d &t, const SbRotation &r, const SbVec3d &s);
    void	setTransform(const SbVec3d &t, const SbRotation &r, const SbVec3d &s, const SbRotation &so);

    /// Decomposes the matrix into a translation, rotation, scale,
    /// and scale orientation.  Any projection information is discarded.
    /// The decomposition depends upon choice of center point for
    /// rotation and scaling, which is optional as the last parameter.
    /// Note that if the center is 0, decompose() is the same as
    /// factor() where "t" is translation, "u" is rotation, "s" is scaleFactor,
    /// and "r" is ScaleOrientattion.
    void	getTransform(SbVec3d &translation,
			  SbRotation &rotation,
			  SbVec3d &scaleFactor,
			  SbRotation &scaleOrientation,
			  const SbVec3d &center) const;

    /// Return translation, rotation, scale, and scale orientation
    /// components of the matrix.
    void	getTransform(SbVec3d &t, SbRotation &r, SbVec3d &s, SbRotation &so) const;


    // The following methods return matrix values and other info:

    /// Returns 4x4 array of elements
    void	getValue(SbMatd &m) const;
    
    /// Returns 4x4 array of elements
    const SbMatd &	getValue() const { return matrix; }

    /// Returns determinant of 3x3 submatrix composed of given row and
    /// column indices (0-3 for each).
    double	det3(int r1, int r2, int r3, int c1, int c2, int c3) const;

    /// Returns determinant of upper-left 3x3 submatrix
    double	det3() const { return det3(0, 1, 2, 0, 1, 2); }

    /// Returns determinant of entire matrix
    double	det4() const;

    /// Factors a matrix m into 5 pieces: m = r s r^ u t, where r^
    /// means transpose of r, and r and u are rotations, s is a scale,
    /// and t is a translation. Any projection information is returned
    /// in proj.
    SbBool	factor(SbMatrixd &r, SbVec3d &s, SbMatrixd &u, SbVec3d &t, SbMatrixd &proj) const;

    /// Returns inverse of matrix. Results are undefined for
    /// singular matrices.  Uses LU decompostion
    SbMatrixd	inverse() const;

    /// Perform in-place LU decomposition of matrix.  indx is index of rows
    /// in matrix. d is the parity of row swaps.  Returns FALSE if singular
    SbBool	LUDecomposition(int index[4], double &d);

    /// Perform back-substitution on LU-decomposed matrix. Index is
    /// permutation of rows from original matrix
    void	LUBackSubstitution(int index[4], double b[4]) const;

    /// Returns transpose of matrix
    SbMatrixd	transpose() const;


    // The following methods provide Mx/mx and mx/vec arithmetic:

    /// Multiplies matrix by given matrix on right or left
    SbMatrixd &	multRight(const SbMatrixd &m);	// this = this * m
    SbMatrixd &	multLeft(const SbMatrixd &m);	// this = m * this

    /// Multiplies matrix by given column vector, giving vector result
    void	multMatrixVec(const SbVec3d &src, SbVec3d &dst) const;

    /// Multiplies given row vector by matrix, giving vector result
    void	multVecMatrix(const SbVec3d &src, SbVec3d &dst) const;

    /// Multiplies given row vector by matrix, giving vector result
    /// src is assumed to be a direction vector, so translation part of
    /// matrix is ignored.
    void	multDirMatrix(const SbVec3d &src, SbVec3d &dst) const;

    /// Multiplies the given line's origin by the matrix, and the
    /// line's direction by the rotation portion of the matrix
    void	multLineMatrix(const SbLine &src, SbLine &dst) const;


    // The following methods are miscellaneous Mx functions:

    /// Prints a formatted version of the matrix to the given file pointer
    void	print(FILE *fp) const;


    /// Cast: returns pointer to storage of first element
    operator double *() 		{ return &matrix[0][0]; }

    /// Cast: returns reference to 4x4 array
    operator SbMatd &() 		{ return matrix; }

    /// Make it look like a usual matrix (so you can do m[3][2])
    double *	  operator [](int i) 	    { return &matrix[i][0]; }

    /// Make it look like a usual matrix (so you can do m[3][2])
    const double * operator [](int i) const  { return &matrix[i][0]; }

    /// Sets value from 4x4 array of elements
    SbMatrixd &	operator =(const SbMatd &m);

    /// Set the matrix from another \a SbMatrixd
    SbMatrixd &	operator =(const SbMatrixd &m);

    /// Sets value from a rotation
    SbMatrixd &	operator =(const SbRotation &q)	{ setRotate(q); return *this; }

    /// Performs right multiplication with another matrix
    SbMatrixd &	operator *=(const SbMatrixd &m)	{ return multRight(m); }

    /// Binary multiplication of matrices
    friend INVENTOR_API SbMatrixd	operator *(const SbMatrixd &m1, const SbMatrixd &m2);

    /// Equality comparison operator
    friend INVENTOR_API int		operator ==(const SbMatrixd &m1, const SbMatrixd &m2);

    /// Inequality comparison operator
    friend INVENTOR_API int		operator !=(const SbMatrixd &m1, const SbMatrixd &m2)
	{ return !(m1 == m2); }

    /// Equality comparison within given tolerance, for each component
    SbBool		equals(const SbMatrixd &m, double tolerance) const;

  private:
    SbMatd	matrix;		// Storage for 4x4 matrix

    // Diagonalizes 3x3 matrix
    void	jacobi3(double evalues[3], SbVec3d evectors[3], int &rots) const;

    SbBool affine_inverse(const SbMatrixd &in, SbMatrixd &out) const;
};

#endif /* _SB_MATRIX_ */
