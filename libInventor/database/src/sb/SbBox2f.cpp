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
 |   $Revision: 1.2 $
 |
 |   Classes:
 |	SbBox2f
 |
 |   Author(s)		: Paul S. Strauss, Nick Thompson
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */
 
#include <Inventor/SbBox2f.h>

#include <math.h>
#include <limits.h>
#include <float.h>    /* For FLT_MAX */

 //
 // Return the center of a box
 //

 SbVec2f
 SbBox2f::getCenter() const
 {
     return SbVec2f(0.5f * (min[0] + max[0]),
 		   0.5f * (min[1] + max[1]));
 }

 //////////////////////////////////////////////////////////////////////////////
 //
 //  Extends Box2f (if necessary) to contain given 2D point
 //
 void
 SbBox2f::extendBy(const SbVec2f &pt)
 //
 //////////////////////////////////////////////////////////////////////////////
 {
     if (pt[0] < min[0]) min[0] = pt[0];
     if (pt[0] > max[0]) max[0] = pt[0];

     if (pt[1] < min[1]) min[1] = pt[1];
     if (pt[1] > max[1]) max[1] = pt[1];
 }

 //////////////////////////////////////////////////////////////////////////////
 //
 // Extends Box2f (if necessary) to contain given Box2f
 //
 void
 SbBox2f::extendBy(const SbBox2f &r)
 //
 //////////////////////////////////////////////////////////////////////////////
 {
     if (r.min[0] < min[0]) min[0] = r.min[0];
     if (r.max[0] > max[0]) max[0] = r.max[0];
     if (r.min[1] < min[1]) min[1] = r.min[1];
     if (r.max[1] > max[1]) max[1] = r.max[1];
 }

 //////////////////////////////////////////////////////////////////////////////
 //
 // Returns TRUE if intersection of given point and Box2f is not empty
 //
 SbBool
 SbBox2f::intersect(const SbVec2f &pt) const
 //
 //////////////////////////////////////////////////////////////////////////////
 {
     return ((pt[0] >= min[0]) &&
 	    (pt[1] >= min[1]) &&
 	    (pt[0] <= max[0]) &&
 	    (pt[1] <= max[1]));
 }

 //////////////////////////////////////////////////////////////////////////////
 //
 // Returns TRUE if intersection of given Box2f and Box2f is not empty
 //
 SbBool
 SbBox2f::intersect(const SbBox2f &r) const
 //
 //////////////////////////////////////////////////////////////////////////////
 {
     return ((r.max[0] >= min[0]) && (r.min[0] <= max[0]) &&
 	    (r.max[1] >= min[1]) && (r.min[1] <= max[1]));
 }


 //////////////////////////////////////////////////////////////////////////////
 //
 // Sets Box2f to contain nothing
 //
 void
 SbBox2f::makeEmpty()
 //
 //////////////////////////////////////////////////////////////////////////////
 {
     min.setValue( FLT_MAX,  FLT_MAX);
     max.setValue(-FLT_MAX, -FLT_MAX);
 }

 //////////////////////////////////////////////////////////////////////////////
 //
 // Equality comparison operator. 
 //
 int
 operator ==(const SbBox2f &b1, const SbBox2f &b2)
 //////////////////////////////////////////////////////////////////////////////
 {
     return ( (b1.min == b2.min) && (b1.max == b2.max ) );
 }

 ////////////////////////////////////////////////////////////////////////
 //
 // Description:
 //  Gets the closest point to the box to the given point. If the
 //  given point is dead center, returns the point centered on the
 //  positive X side.
 //
 // Use: public

 SbVec2f
 SbBox2f::getClosestPoint(const SbVec2f &point)
 //
 ////////////////////////////////////////////////////////////////////////
 {
     SbVec2f result;

     // trivial cases first
     if (isEmpty())
         return point;
     else if (point == getCenter()) {
         // middle of x side
         result[0] = max[0];
         result[1] = (max[1] + min[1])/2.0f;
     }
     else if (min[0] == max[0]) {
         result[0] = min[0];
         result[1] = point[1];
     }
     else if (min[1] == max[1]) {
         result[0] = point[0];
         result[1] = min[1];
     }
     else {
         // Find the closest point on a unit box (from -1 to 1),
         // then scale up.

         // Find the vector from center to the point, then scale
         // to a unit box.
         SbVec2f vec = point - getCenter();
         SbVec2f size = getSize();
         float halfX = size[0]/2.0f;
         float halfY = size[1]/2.0f;
         if (halfX > 0.0)
             vec[0] /= halfX;
         if (halfY > 0.0)
             vec[1] /= halfY;

         // Side to snap to has greatest magnitude in the vector.
         float magX = float(fabs(vec[0]));
         float magY = float(fabs(vec[1]));

         if (magX > magY) {
             result[0] = (vec[0] > 0) ? 1.0f : -1.0f;
             if (magY > 1.0f)
                 magY = 1.0f;
             result[1] = (vec[1] > 0) ? magY : -magY;
         }
         else if (magY > magX) {
             if (magX > 1.0)
                 magX = 1.0;
             result[0] = (vec[0] > 0) ? magX : -magX;
             result[1] = (vec[1] > 0) ? 1.0f : -1.0f;
         }
         else {
             // must be one of the corners
             result[0] = (vec[0] > 0) ? 1.0f : -1.0f;
             result[1] = (vec[1] > 0) ? 1.0f : -1.0f;
         }

         // scale back the result and move it to the center of the box
         result[0] *= halfX;
         result[1] *= halfY;
         result += getCenter();
     }

     return result;
 }
