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
 |	SbLine
 |
 |   Author(s)		: Paul S. Strauss, Nick Thompson, 
 |			  David Mott, Alain Dumesny
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

#ifndef _SB_LINE_
#define _SB_LINE_

#include <Inventor/SbBasic.h>
#include <Inventor/SbVec3f.h>
#include <Inventor/SbBox3f.h>

/// Directed line in 3D.
/// \ingroup Basics
/// Represents a directed line in 3D. This is a basic Inventor datatype that is
/// used for representing a 3D line. It is used as input and output by a variety of
/// Inventor classes.
/// \sa SbVec3f, SbPlane
class INVENTOR_API SbLine {
  public:
    SbLine()	{}

    /// Construct a line from two points lying on the line.  If you
    /// want to construct a line from a position and a direction, use
    /// SbLine(p, p + d).
    /// Line is directed from p0 to p1.
    SbLine(const SbVec3f &p0, const SbVec3f &p1);

    /// Sets line to pass through points \a p0 and \a p1.
    void		setValue(const SbVec3f &p0, const SbVec3f &p1);

    /// Finds the two closest points between this line and \a line2,
    /// and loads them into \a ptOnThis and \a ptOnLine2.
    /// Returns FALSE. if the lines are parallel (results undefined), and returns
    /// TRUE. otherwise.
    SbBool		getClosestPoints(const SbLine  &line2,
					 SbVec3f &ptOnThis,
					 SbVec3f &ptOnLine2 ) const;

    /// Returns the closest point on the line to the given point.
    SbVec3f		getClosestPoint(const SbVec3f &point) const;


    /// Returns position of line origin point.
    const SbVec3f &	getPosition() const	{ return pos; }
    
    /// Returns direction vector of line.
    const SbVec3f &	getDirection() const	{ return dir; }


  SoINTERNAL public:
    // Intersect the line with a box, point, line, and triangle.
    SbBool              intersect( const SbBox3f &box,
                                   SbVec3f &enter, SbVec3f &exit ) const;
    SbBool              intersect( float angle, const SbBox3f &box ) const;
    SbBool              intersect( float angle, const SbVec3f &point ) const;
    SbBool              intersect( float angle, const SbVec3f &v0,
                                   const SbVec3f &v1, SbVec3f &pt ) const;
    SbBool              intersect( const SbVec3f &v0,
				  const SbVec3f &v1,
				  const SbVec3f &v2,
				  SbVec3f &pt, SbVec3f &barycentric,
				  SbBool &front ) const;

  private:
    // Parametric description:
    //  l(t) = pos + t * dir
    SbVec3f	pos;
    SbVec3f	dir;
};

#endif /* _SB_LINE_ */
