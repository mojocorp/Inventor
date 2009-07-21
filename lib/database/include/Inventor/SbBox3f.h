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

#ifndef _SB_BOX3F_
#define _SB_BOX3F_

#include <Inventor/SbVec3f.h>

class SbMatrix;

class INVENTOR_API SbBox3f {
  public:
    // Default constructor - leaves box totally empty
    SbBox3f()						{ makeEmpty(); }

    // Constructor given bounds 
    // C-api: name=CreateBounds
    SbBox3f(float xmin, float ymin, float zmin,
	    float xmax, float ymax, float zmax)
	{ min.setValue(xmin, ymin, zmin); max.setValue(xmax, ymax, zmax); }

    // Constructor given minimum and maximum points 
    // C-api: name=CreateMinMax
    SbBox3f(const SbVec3f &_min, const SbVec3f &_max)
	{ min = _min; max = _max; }

    ~SbBox3f()						{ }

    // Returns the min and max points
    // C-api: name=getMinConst
    const SbVec3f &	getMin() const { return min; }
    // C-api: name=getMaxConst
    const SbVec3f &	getMax() const { return max; }
    SbVec3f &	        getMin()       { return min; }
    SbVec3f &	        getMax()       { return max; }

    // Returns the center of a box
    SbVec3f	getCenter() const;

    // Extends Box3f (if necessary) to contain given 3D point
    // C-api: name=extendByPt
    void	extendBy(const SbVec3f &pt);

    // Extends Box3f (if necessary) to contain given Box3f
    // C-api: name=extendByBox
    void	extendBy(const SbBox3f &bb);

    // Returns TRUE if intersection of given point and Box3f is not empty
    // C-api: name=intersectPt
    SbBool	intersect(const SbVec3f &pt) const;

    // Returns TRUE if intersection of given Box3f and Box3f is not empty
    // C-api: name=intersectBox
    SbBool	intersect(const SbBox3f &bb) const;

    // Returns TRUE if bounding box is completely outside the
    // view-volume defined by the model+view+projection matrix given.
    // "cullBits" keeps track of which view-volume clipping planes the
    // box is completely inside of; if a 'parent' bounding box of this
    // bounding box was found to be completely inside the left/right
    // clipping planes, the low bit of cullBits will be set to zero,
    // and this routine will not test this box against those planes.
    // When cullBits is zero, it is a waste of time to call this
    // function!  Before calling this the first time, set cullBits to
    // 7 (SoGLRenderAction and SoSeparator will handle this
    // automatically for render culling).
    SbBool	outside(const SbMatrix &MVP, int &cullBits) const;

    // Common get and set functions
    void	setBounds(float xmin, float ymin, float zmin,
			  float xmax, float ymax, float zmax)
	{ min.setValue(xmin, ymin, zmin); max.setValue(xmax, ymax, zmax); }

    // C-api: name=setBoundsMinMax
    void	setBounds(const SbVec3f &_min, const SbVec3f &_max)
	{ min = _min; max = _max; }

    void	getBounds(float &xmin, float &ymin, float &zmin,
			  float &xmax, float &ymax, float &zmax) const
	{ min.getValue(xmin, ymin, zmin); max.getValue(xmax, ymax, zmax); }

    // C-api: name=getBoundsMinMax
    void	getBounds(SbVec3f &_min, SbVec3f &_max) const
	{ _min = min; _max = max; }

    // Returns the closest point on the box to the given point.
    // (Returns the point on the center of the Z face if passed the center.)
    // C-api: name=getClosestPt
    SbVec3f	getClosestPoint(const SbVec3f &point);
    
    // Returns origin (minimum point) of box
    void	getOrigin(float &originX,
			  float &originY,
			  float &originZ) const
	{ originX = min[0]; originY = min[1]; originZ = min[2]; }

    // Returns size of box
    void	getSize(float &sizeX, float &sizeY, float &sizeZ) const
	{ sizeX = max[0] - min[0];
	  sizeY = max[1] - min[1];
          sizeZ = max[2] - min[2]; }

    // Sets Box3f to contain nothing
    void	makeEmpty();

    // Checks if the box is empty (degenerate)
    // note that this relies on boxes being completely degenerate if
    // they are degenerate at all.  All member functions preserve this
    // invariant.
    SbBool	isEmpty()  const		{ return max[0] < min[0]; }

    // Checks if the box has volume; i.e., all three dimensions have
    // positive size
    // C-api: name=hasVol
    SbBool	hasVolume() const
	{ return (max[0] > min[0] && max[1] > min[1] && max[2] > min[2] ); }

    // Finds the extent of a box along a particular direction
    void	getSpan(const SbVec3f &direction,
			float &dMin, float &dMax) const;

    // Transforms Box3f by matrix, enlarging Box3f to contain result
    // C-api: name=xf
    void	transform(const SbMatrix &m);

    // Gives the volume of the box (0 for an empty box)
    // C-api: name=getVol
    float	getVolume() const;

    // Equality comparisons
    friend INVENTOR_API int          operator ==(const SbBox3f &b1, const SbBox3f &b2);
    friend INVENTOR_API int          operator !=(const SbBox3f &b1, const SbBox3f &b2)
	{ return !(b1 == b2); }

  private:
    // Minimum and maximum points
    SbVec3f	min, max;
};
#endif /* _SB_BOX3F_ */

