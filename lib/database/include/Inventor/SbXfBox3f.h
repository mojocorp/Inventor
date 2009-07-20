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
 
 #ifndef _SB_XFBOX3F_
 #define _SB_XFBOX3F_
 
#include <Inventor/SbVec3f.h>
#include <Inventor/SbBox3f.h>
#include <Inventor/SbMatrix.h>
 
class INVENTOR_API SbXfBox3f : public SbBox3f {
  public:
    // Default constructor - leaves box totally empty
    SbXfBox3f();

    // Constructor given minimum and maximum points 
    SbXfBox3f(const SbVec3f &_min, const SbVec3f &_max);

    // Constructor given Box3f
    SbXfBox3f(const SbBox3f &box);

    ~SbXfBox3f()						{ }

    // Set the transformation on the box
    void		setTransform(const SbMatrix &m);

    // Get the transformation on the box, and its inverse.
    const SbMatrix &	getTransform() const	    { return xform; }
    const SbMatrix &	getInverse() const	    { return xformInv; }
    
    // Returns the center of a box
    SbVec3f		getCenter() const;

    // Extends XfBox3f (if necessary) to contain given 3D point
    void		extendBy(const SbVec3f &pt);

    // Extends XfBox3f (if necessary) to contain given Box3f
    void		extendBy(const SbBox3f &bb)
	{ extendBy(SbXfBox3f(bb)); }

    // Extends XfBox3f (if necessary) to contain given XfBox3f
    void		extendBy(const SbXfBox3f &bb);

    // Returns TRUE if intersection of given point and Box3f is not empty
    SbBool		intersect(const SbVec3f &pt) const;

    // Returns TRUE if intersection of given XfBox3f and Box3f is not empty
    SbBool		intersect(const SbBox3f &bb) const
	{ return project().intersect(bb); }

    // Common get and set functions
    void	setBounds(float xmin, float ymin, float zmin,
			  float xmax, float ymax, float zmax)
	{ SbBox3f::setBounds(xmin, ymin, zmin, xmax, ymax, zmax); }

    void	setBounds(const SbVec3f &_min, const SbVec3f &_max)
	{ SbBox3f::setBounds(_min, _max); }

    void	getBounds(float &xmin, float &ymin, float &zmin,
			  float &xmax, float &ymax, float &zmax) const
	{ SbBox3f::getBounds(xmin, ymin, zmin, xmax, ymax, zmax); }

    void	getBounds(SbVec3f &_min, SbVec3f &_max) const
	{ SbBox3f::getBounds(_min, _max); }

    // Returns origin (minimum point) of box
    void	getOrigin(float &originX,
			  float &originY,
			  float &originZ)
	{ SbBox3f::getOrigin(originX, originY, originZ); }

    // Returns size of box
    void	getSize(float &sizeX, float &sizeY, float &sizeZ)
	{ SbBox3f::getSize(sizeX, sizeY, sizeZ); }

    // Gives the volume of the box (0 for an empty box)
    float	getVolume() const;

    // Sets Box3f to contain nothing
    void		makeEmpty()		{ SbBox3f::makeEmpty(); }

    // Checks if the box is empty (degenerate)
    // note that this relies on boxes being completely degenerate if
    // they are degenerate at all.  All member functions preserve this
    // invariant.
    SbBool		isEmpty() const	{ return SbBox3f::isEmpty(); }

    // Checks if the box has volume; i.e., all three dimensions have
    // positive size
    SbBool		hasVolume() const { return SbBox3f::hasVolume(); }

    // Finds the extent of a box along a particular direction
    void		getSpan(const SbVec3f &direction,
				float &dMin, float &dMax) const
	{ project().getSpan(direction, dMin, dMax); }

    // Transforms Box3f by matrix
    void		transform(const SbMatrix &m);

    // Projects an SbXfBox3f to an SbBox3f
    SbBox3f		project() const;

    // Equality comparisons
    friend INVENTOR_API int          operator ==(const SbXfBox3f &b1, const SbXfBox3f &b2);
    friend INVENTOR_API int          operator !=(const SbXfBox3f &b1, const SbXfBox3f &b2)
	{ return !(b1 == b2); }

  private:
    // These are incorrect for SbXfBox3f, so we hide them
    const SbVec3f &	getMin() const			{ return SbBox3f::getMin(); }
    const SbVec3f &	getMax() const			{ return SbBox3f::getMax(); }

    // The box is transformed by this xform
    SbMatrix		xform;
    SbMatrix		xformInv;
};
#endif /* _SB_XFBOX3F_ */
