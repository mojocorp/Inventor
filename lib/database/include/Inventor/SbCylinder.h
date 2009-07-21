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
 |	SbCylinder
 |
 |   Author(s)		: Paul S. Strauss, Nick Thompson, 
 |			  David Mott, Alain Dumesny
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

#ifndef _SB_CYLINDER_
#define _SB_CYLINDER_

#include <Inventor/SbBasic.h>
#include <Inventor/SbLine.h>

//////////////////////////////////////////////////////////////////////////////
//
//  Class: SbCylinder
//
//  Represents a cylinder in 3D space.
//
//////////////////////////////////////////////////////////////////////////////

class INVENTOR_API SbCylinder {
  public:
// C-api: end
    // Constructor
    SbCylinder();

    // Construct a cylinder given its axis and radius
    SbCylinder(const SbLine &a, float r);

    // Change the axis and radius
    void 	setValue(const SbLine &a, float r);

    // Set just the axis or radius
    void	setAxis(const SbLine &a);
    void	setRadius(float r);

    // Return the axis and radius
    const SbLine & 	getAxis() const			{ return axis; }
    float		getRadius() const		{ return radius; }

// C-api: begin
    // Intersect line and cylinder, returning TRUE if there is an intersection
    SbBool	intersect(const SbLine &l, SbVec3f &intersection) const;
    // C-api: name=intersect2
    SbBool	intersect(const SbLine &l,
			  SbVec3f &enter, SbVec3f &exit) const;

  private:
    SbLine	axis;
    float	radius;

    static SbBool	unitCylinderIntersect(const SbLine &l,
					      SbVec3f &in, SbVec3f &out);
};

#endif /* _SB_CYLINDER_ */
