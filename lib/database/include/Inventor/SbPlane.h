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
 |	SbPlane
 |
 |   Author(s)		: Paul S. Strauss, Nick Thompson, 
 |			  David Mott, Alain Dumesny
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

#ifndef _SB_PLANE_
#define _SB_PLANE_

#include <Inventor/SbVec3f.h>

//////////////////////////////////////////////////////////////////////////////
//
//  Class: SbPlane
//
//  Represents an oriented plane in 3D space. The plane is defined by
//  a plane normal and a distance from the origin along that normal.
//  SbPlanes may be used to represent either planes or half-spaces. In
//  the latter case (as for the isInHalfSpace() method), the
//  half-space is defined to be all points on the plane or on the side
//  of the plane in the direction of the plane normal.
//
//  The 4 coefficients of the plane equation of an SbPlane can be
//  obtained easily as the 3 coordinates of the plane normal and the
//  distance, in that order.
//
//////////////////////////////////////////////////////////////////////////////
class SbLine;
class SbMatrix;

class INVENTOR_API SbPlane {
  public:
    SbPlane()	{}

    // Construct a plane given 3 points.
    // Orientation is computed by taking (p1 - p0) x (p2 - p0) and
    // pointing the normal in that direction.
    SbPlane(const SbVec3f &p0, const SbVec3f &p1, const SbVec3f &p2);

    // Construct a plane given normal and distance from origin along normal.
    // Orientation is given by the normal vector n.
    SbPlane(const SbVec3f &n, float d);

    // Construct a plane given normal and a point to pass through
    // Orientation is given by the normal vector n.
    SbPlane(const SbVec3f &n, const SbVec3f &p);

    // Offset a plane by a given distance.
    void		offset(float d);

    // Intersect line and plane, returning TRUE if there is an intersection
    // FALSE if line is parallel to plane
    SbBool		intersect(const SbLine &l,
				  SbVec3f &intersection) const;

    // Transforms the plane by the given matrix
    void		transform(const SbMatrix &matrix);

    // Returns TRUE if the given point is within the half-space
    // defined by the plane
    SbBool		isInHalfSpace(const SbVec3f &point) const;


    // Accessors
    const SbVec3f &	getNormal() const		{ return normalVec; }
    float		getDistanceFromOrigin() const	{ return distance; }

    // Equality/inequality comparison operators
    friend INVENTOR_API int		operator ==(const SbPlane &p1, const SbPlane &p2);
    friend INVENTOR_API int		operator !=(const SbPlane &p1, const SbPlane &p2)
	{ return !(p1 == p2); }

  private:
    // Plane is all p such that normalVec . p - distance = 0

    // Normal to the plane
    SbVec3f	normalVec;

    // Distance from origin to plane: distance * normalVec is on the plane
    float	distance;
};

#endif /* _SB_PLANE_ */
