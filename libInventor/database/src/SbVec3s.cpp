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

/*
 * Copyright (C) 1990,91   Silicon Graphics, Inc.
 *
 _______________________________________________________________________
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 |
 |   $Revision: 1.1 $
 |
 |   Classes:
 |	SbVec3s
 |	SbVec2s
 |	SbVec2f
 |	SbVec4f
 |
 |   Author(s)		: Paul S. Strauss, Nick Thompson, Alain Dumesny
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

#include <Inventor/SbVec3s.h>

#include <math.h>

//
// Returns dot (inner) product of vector and another vector
//

short
SbVec3s::dot(const SbVec3s &v) const
{
    return (vec[0] * v.vec[0] +
            vec[1] * v.vec[1] +
            vec[2] * v.vec[2]);
}

//
// Returns 3 individual components
//

void
SbVec3s::getValue(short &x, short &y, short &z) const
{
    x = vec[0];
    y = vec[1];
    z = vec[2];
}

//
// Negates each component of vector in place
//

void
SbVec3s::negate()
{
    vec[0] = -vec[0];
    vec[1] = -vec[1];
    vec[2] = -vec[2];
}

//
// Component-wise scalar multiplication operator
//

SbVec3s &
SbVec3s::operator *=(short d)
{
    vec[0] *= d;
    vec[1] *= d;
    vec[2] *= d;

    return *this;
}

//
// Component-wise vector addition operator
//

SbVec3s &
SbVec3s::operator +=(SbVec3s v)
{
    vec[0] += v.vec[0];
    vec[1] += v.vec[1];
    vec[2] += v.vec[2];

    return *this;
}

//
// Component-wise vector subtraction operator
//

SbVec3s &
SbVec3s::operator -=(SbVec3s v)
{
    vec[0] -= v.vec[0];
    vec[1] -= v.vec[1];
    vec[2] -= v.vec[2];

    return *this;
}

//
// Nondestructive unary negation - returns a new vector
//

SbVec3s
SbVec3s::operator -() const
{
    return SbVec3s(-vec[0], -vec[1], -vec[2]);
}

//
// Component-wise binary scalar multiplication operator
//

SbVec3s
operator *(const SbVec3s &v, short d)
{
    return SbVec3s(v.vec[0] * d,
                   v.vec[1] * d,
                   v.vec[2] * d);
}

//
// Component-wise binary vector addition operator
//

SbVec3s
operator +(const SbVec3s &v1, const SbVec3s &v2)
{
    return SbVec3s(v1.vec[0] + v2.vec[0],
                   v1.vec[1] + v2.vec[1],
                   v1.vec[2] + v2.vec[2]);
}

//
// Component-wise binary vector subtraction operator
//

SbVec3s
operator -(const SbVec3s &v1, const SbVec3s &v2)
{
    return SbVec3s(v1.vec[0] - v2.vec[0],
                   v1.vec[1] - v2.vec[1],
                   v1.vec[2] - v2.vec[2]);
}

//
// Equality comparison operator. Componenents must match exactly.
//

int
operator ==(const SbVec3s &v1, const SbVec3s &v2)
{
    return (v1.vec[0] == v2.vec[0] &&
            v1.vec[1] == v2.vec[1] &&
            v1.vec[2] == v2.vec[2]);
}
