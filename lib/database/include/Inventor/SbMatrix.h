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
 |	SbMatrix
 |
 |   Author(s)		: Paul S. Strauss, Nick Thompson, 
 |			  David Mott, Alain Dumesny
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

#ifndef _SB_MATRIX_
#define _SB_MATRIX_

#include <Inventor/SbBasic.h>

//////////////////////////////////////////////////////////////////////////////
//
//  Class: SbMatrix
//
//  4x4 matrix of floating-point elements.
//
//////////////////////////////////////////////////////////////////////////////
class SbLine;
class SbRotation;
class SbVec3f;
class SbVec4f;

typedef float SbMat[4][4];

class INVENTOR_API SbMatrix {
  public:

    // Default constructor
    SbMatrix()						{ }

    // Constructor given all 16 elements in row-major order
    SbMatrix(float a11, float a12, float a13, float a14,
	     float a21, float a22, float a23, float a24, 
	     float a31, float a32, float a33, float a34, 
	     float a41, float a42, float a43, float a44);

    // Constructor from a 4x4 array of elements
    SbMatrix(const SbMat &m);

    // Sets value from 4x4 array of elements
    void	setValue(const SbMat &m);

    public:

    // Sets matrix to be identity
    void	makeIdentity();

    // Returns an identity matrix 
    static SbMatrix	identity();

    // Sets matrix to rotate by given rotation
    void	setRotate(const SbRotation &q);

    // Sets matrix to scale by given uniform factor
    void	setScale(float s);

    // Sets matrix to scale by given vector
    void	setScale(const SbVec3f &s);

    // Sets matrix to translate by given vector
    void	setTranslate(const SbVec3f &t);

    // Composes the matrix based on a translation, rotation, scale,
    // orientation for scale, and center.  The "center" is the
    // center point for scaling and rotation.  The "scaleOrientation"
    // chooses the primary axes for the scale.
    void	setTransform(
			const SbVec3f &translation,
			const SbRotation &rotation,
			const SbVec3f &scaleFactor,
			const SbRotation &scaleOrientation,
			const SbVec3f &center);
    // Overloaded methods as a kludge because the compiler won't let
    // us have SbVec3f(0,0,0) as a default value:
    void	setTransform(const SbVec3f &t, const SbRotation &r, const SbVec3f &s);
    void	setTransform(const SbVec3f &t, const SbRotation &r, const SbVec3f &s, const SbRotation &so);

    // Decomposes the matrix into a translation, rotation, scale,
    // and scale orientation.  Any projection information is discarded.
    // The decomposition depends upon choice of center point for
    // rotation and scaling, which is optional as the last parameter.
    // Note that if the center is 0, decompose() is the same as
    // factor() where "t" is translation, "u" is rotation, "s" is scaleFactor,
    // and "r" is ScaleOrientattion.
    void	getTransform(SbVec3f &translation,
			  SbRotation &rotation,
			  SbVec3f &scaleFactor,
			  SbRotation &scaleOrientation,
			  const SbVec3f &center) const;
    void	getTransform(SbVec3f &t, SbRotation &r,
                          SbVec3f &s, SbRotation &so) const;


    // The following methods return matrix values and other info:

    // Returns 4x4 array of elements
    void	getValue(SbMat &m) const;
    const SbMat &	getValue() const { return matrix; }

    // Returns determinant of 3x3 submatrix composed of given row and
    // column indices (0-3 for each).
    float	det3(int r1, int r2, int r3, int c1, int c2, int c3) const;

    // Returns determinant of upper-left 3x3 submatrix
    float	det3() const { return det3(0, 1, 2, 0, 1, 2); }

    // Returns determinant of entire matrix
    float	det4() const;

    // Factors a matrix m into 5 pieces: m = r s r^ u t, where r^
    // means transpose of r, and r and u are rotations, s is a scale,
    // and t is a translation. Any projection information is returned
    // in proj.
    SbBool	factor(SbMatrix &r, SbVec3f &s, SbMatrix &u,
		       SbVec3f &t, SbMatrix &proj) const;

    // Returns inverse of matrix. Results are undefined for
    // singular matrices.  Uses LU decompostion
    SbMatrix	inverse() const;

    // Perform in-place LU decomposition of matrix.  indx is index of rows
    // in matrix. d is the parity of row swaps.  Returns FALSE if singular
    SbBool	LUDecomposition(int index[4], float &d);

    // Perform back-substitution on LU-decomposed matrix. Index is
    // permutation of rows from original matrix
    void	LUBackSubstitution(int index[4], float b[4]) const;

    // Returns transpose of matrix
    SbMatrix	transpose() const;


    // The following methods provide Mx/mx and mx/vec arithmetic:

    // Multiplies matrix by given matrix on right or left
    SbMatrix &	multRight(const SbMatrix &m);	// this = this * m
    SbMatrix &	multLeft(const SbMatrix &m);	// this = m * this

    // Multiplies matrix by given column vector, giving vector result
    void	multMatrixVec(const SbVec3f &src, SbVec3f &dst) const;

    // Multiplies given row vector by matrix, giving vector result
    void	multVecMatrix(const SbVec3f &src, SbVec3f &dst) const;

    // Multiplies given row vector by matrix, giving vector result
    // src is assumed to be a direction vector, so translation part of
    // matrix is ignored.
    void	multDirMatrix(const SbVec3f &src, SbVec3f &dst) const;

    // Multiplies the given line's origin by the matrix, and the
    // line's direction by the rotation portion of the matrix
    void	multLineMatrix(const SbLine &src, SbLine &dst) const;


    // The following methods are miscellaneous Mx functions:

    // Prints a formatted version of the matrix to the given file pointer
    void	print(FILE *fp) const;


    // Cast: returns pointer to storage of first element
    operator float *() 		{ return &matrix[0][0]; }

    // Cast: returns reference to 4x4 array
    operator SbMat &() 		{ return matrix; }

    // Make it look like a usual matrix (so you can do m[3][2])
    float *	  operator [](int i) 	    { return &matrix[i][0]; }
    const float * operator [](int i) const  { return &matrix[i][0]; }

    // Sets value from 4x4 array of elements
    SbMatrix &	operator =(const SbMat &m);

    SbMatrix &	operator =(const SbMatrix &m);
    // This silly little inline method is needed because some
    // compilers won't use the above method...
#ifdef __sgi
    SbMatrix &  operator =(      SbMat &m) { return *this = (const SbMat &)m; }
#endif // __sgi

    // Sets value from a rotation
    SbMatrix &	operator =(const SbRotation &q)	{ setRotate(q); return *this; }

    // Performs right multiplication with another matrix
    SbMatrix &	operator *=(const SbMatrix &m)	{ return multRight(m); }

    // Binary multiplication of matrices
    friend INVENTOR_API SbMatrix	operator *(const SbMatrix &m1, const SbMatrix &m2);

    // Equality comparison operator
    friend INVENTOR_API int		operator ==(const SbMatrix &m1, const SbMatrix &m2);
    friend INVENTOR_API int		operator !=(const SbMatrix &m1, const SbMatrix &m2)
	{ return !(m1 == m2); }

    // Equality comparison within given tolerance, for each component
    SbBool		equals(const SbMatrix &m, float tolerance) const;

  private:
    SbMat	matrix;		// Storage for 4x4 matrix

    // Diagonalizes 3x3 matrix
    void	jacobi3(float evalues[3], SbVec3f evectors[3], int &rots) const;

    SbBool affine_inverse(const SbMatrix &in, SbMatrix &out) const;
};

#endif /* _SB_MATRIX_ */
