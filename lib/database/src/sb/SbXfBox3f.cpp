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
 |	SbXfBox3f
 |
 |   Author(s)		: Paul S. Strauss, Nick Thompson
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */
 
#include <Inventor/SbXfBox3f.h>

#include <limits.h>
#include <float.h>    /* For FLT_MAX */

 //
 // Default constructor - leaves box totally empty
 //
 SbXfBox3f::SbXfBox3f()
 {
     xform.makeIdentity();
     xformInv.makeIdentity();
     makeEmpty();
 }

 //
 // Constructor given minimum and maximum points 
 //
 SbXfBox3f::SbXfBox3f(const SbVec3f &_min, const SbVec3f &_max)
 {
     xform.makeIdentity();
     xformInv.makeIdentity();
     setBounds(_min, _max);
 }

 //
 // Constructor given Box3f
 //
 SbXfBox3f::SbXfBox3f(const SbBox3f &box)
 {
     xform.makeIdentity();
     xformInv.makeIdentity();
     *(SbBox3f *)this = box;
 }

 #define PRECISION_LIMIT (1.0e-13)

 //
 // Set the transformation on the box.  This is careful about
 // non-invertable transformations.
 //
 void
 SbXfBox3f::setTransform(const SbMatrix &m)
 {
     xform = m; 

     // Check for degenerate matrix:
     float det = m.det4();
     if (det < PRECISION_LIMIT && det > -PRECISION_LIMIT) {
 	// We'll mark inverse[0][0] with FLT_MAX (max floating point
 	// value) as special value to indicate degenerate transform.
 	xformInv = SbMatrix(FLT_MAX,0,0,0,
 			    0,0,0,0,   0,0,0,0,  0,0,0,0);
     } else {
 	xformInv = m.inverse();
     }
 }

 #undef PRECISION_LIMIT

 //
 // Return the center of a box
 //
 SbVec3f
 SbXfBox3f::getCenter() const
 {
     SbVec3f	p;

     // transform the center before returning it
     xform.multVecMatrix(.5 * (getMin() + getMax()), p);

     return p;
 }

 //
 // Extend (if necessary) to contain given 3D point
 //
 void
 SbXfBox3f::extendBy(const SbVec3f &pt)
 {
     // If our transform is degenerate, project this box, which will
     // transform min/max and get a box with identity xforms:
     if (xformInv[0][0] == FLT_MAX) {
 	*this = SbXfBox3f(this->project());
     }

     SbVec3f p;
     xformInv.multVecMatrix(pt, p);
     SbBox3f::extendBy(p);
 }

 //
 // Finds the volume of the box (0 for an empty box)
 //

 float
 SbXfBox3f::getVolume() const
 {
     if (isEmpty())
 	return 0.0;

     // The volume of a transformed box is just its untransformed
     // volume times the determinant of the upper-left 3x3 of
     // the xform matrix. Quoth Paul Strauss: "Pretty cool, indeed."
     float objVol = SbBox3f::getVolume();
     float factor = xform.det3();
     return factor * objVol;
 }

 //
 // Extends XfBox3f (if necessary) to contain given XfBox3f
 //

 void
 SbXfBox3f::extendBy(const SbXfBox3f &bb)
 {
     if (bb.isEmpty())			// bb is empty, no change
 	return;
     else if (isEmpty())			// we're empty, use bb
 	*this = bb;

     else if (xformInv[0][0] != FLT_MAX && bb.xformInv[0][0] != FLT_MAX) {
 	// Neither box is empty and they are in different spaces. To
 	// get the best results, we'll perform the merge of the two
 	// boxes in each of the two spaces. Whichever merge ends up
 	// being smaller is the one we'll use.
 	// Note that we don't perform a project() as part of the test.
 	// This is because projecting almost always adds a little extra
 	// space. It also gives an unfair advantage to the
 	// box more closely aligned with world space.  In the simplest
 	// case this might be preferable. However, over many objects,
 	// we are better off going with the minimum in local space,
 	// and not worrying about projecting until the very end.

 	SbXfBox3f	xfbox1, xfbox2;
 	SbBox3f		box1, box2;

 	// Convert bb into this's space to get box1
 	xfbox1 = bb;
 	// Rather than calling transform(), which calls inverse(),
 	// we'll do it ourselves, since we already know the inverse matrix.
 	// I.e., we could call: xfbox1.transform(xformInv);
 	xfbox1.xform *= xformInv;
 	xfbox1.xformInv.multRight(xform);
 	box1 = xfbox1.project();

 	// Convert this into bb's space to get box2
 	xfbox2 = *this;
 	// Same here for: xfbox2.transform(bb.xformInv);
 	xfbox2.xform *= bb.xformInv;
 	xfbox2.xformInv.multRight(bb.xform);
 	box2 = xfbox2.project();

 	// Extend this by box1 to get xfbox1
 	xfbox1 = *this;
 	xfbox1.SbBox3f::extendBy(box1);
 	// Use SbBox3f method; box1 is already in xfbox1's space
 	// (otherwise, we'll get an infinite loop!)

 	// Extend bb by box2 to get xfbox2
 	xfbox2 = bb;
 	xfbox2.SbBox3f::extendBy(box2);
 	// Use SbBox3f method; box2 is already in xfbox2's space
 	// (otherwise, we'll get an infinite loop!)

 	float vol1 = xfbox1.getVolume();
 	float vol2 = xfbox2.getVolume();

 	// Take the smaller result and extend appropriately
 	if (vol1 <= vol2) {
 	    SbBox3f::extendBy(box1);
 	}
 	else {
 	    *this = bb;
 	    SbBox3f::extendBy(box2);
 	}
     }
     else if (xformInv[0][0] == FLT_MAX) {
 	if (bb.xformInv[0][0] == FLT_MAX) {
 	    // Both boxes are degenerate; project them both and
 	    // combine them:
 	    SbBox3f box = this->project();
 	    box.extendBy(bb.project());
 	    *this = SbXfBox3f(box);
 	} else {
 	    // this is degenerate; transform our min/max into bb's
 	    // space, and combine there:
 	    SbBox3f box(getMin(), getMax());
 	    box.transform(xform*bb.xformInv);
 	    *this = bb;
 	    SbBox3f::extendBy(box);
 	}
     } else {
 	// bb is degenerate; transform it into our space and combine:
 	SbBox3f box(bb.getMin(), bb.getMax());
 	box.transform(bb.xform*xformInv);
 	SbBox3f::extendBy(box);
     }
 }

 //
 // Returns TRUE if intersection of given point and Box3f is not empty
 // (being careful about degenerate transformations...).
 //
 SbBool
 SbXfBox3f::intersect(const SbVec3f &pt) const
 {
     if (xformInv[0][0] != FLT_MAX) {
 	SbVec3f p;
 	xformInv.multVecMatrix(pt, p);
 	return SbBox3f::intersect(p);
     }
     SbBox3f box = this->project();  // Degenerate; project and test:
     return box.intersect(pt);
 }

 //
 // Transform this box by a matrix
 //
 void
 SbXfBox3f::transform(const SbMatrix &m) 
 {
     SbMatrix new_xf = xform*m;
     setTransform(new_xf);
 }


 //////////////////////////////////////////////////////////////////////////////
 //
 // Equality comparison operator. 
 //
 int
 operator ==(const SbXfBox3f &b1, const SbXfBox3f &b2)
 //////////////////////////////////////////////////////////////////////////////
 {
     SbBox3f b1Proj = b1.project();
     SbBox3f b2Proj = b2.project();
     return (b1Proj == b2Proj);
 }

 //
 // Projects an SbXfBox to an SbBox
 //

 SbBox3f
 SbXfBox3f::project() const
 {
     SbBox3f	box(getMin(), getMax());
     box.transform(xform);
     return box;
 }
 
