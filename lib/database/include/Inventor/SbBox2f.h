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

#ifndef _SB_BOX2F_
#define _SB_BOX2F_

#include <Inventor/SbBasic.h>
#include <Inventor/SbVec2f.h>

/// 2D box class.
/// \ingroup Basics
/// 2D box which has planes parallel to the major axes and is specified by
/// two points (specified as floating point) on a diagonal. This class is part
/// of the standard Inventor datatype classes and is used as input and output
/// to geometry operations.
/// \sa SbBox3f, SbXfBox3f, SbBox2s, SbVec3f, SbVec2f, SbVec2s, SbMatrix
class INVENTOR_API SbBox2f {
  public:
    /// Default constructor - leaves box totally empty
    SbBox2f()					{ makeEmpty(); };

    /// Constructor given bounds
    /// \a xmin, \a ymin, \a xmax, and \a ymax are the bounds of the box.
    SbBox2f(float xmin, float ymin, float xmax, float ymax)
	{ min.setValue(xmin, ymin); max.setValue(xmax, ymax); }

    /// Constructor given minimum and maximum points
    /// \a min and \a max are the corners of the diagonal that define the box.
    SbBox2f(const SbVec2f &_min, const SbVec2f &_max)
	{ min = _min; max = _max; }

    /// Destructor
    ~SbBox2f()						{ }

    /// Returns the minimum point of the box.  The minimum point is
    /// the corner of the box with the lowest X, Y, and Z values.
    const SbVec2f &	getMin() const			{ return min; }

    /// Returns the maximum point of the box.  The maximum
    /// point is the corner of the box with the highest X, Y, and Z values.
    const SbVec2f &	getMax() const			{ return max; }

    /// Returns the center of the box
    SbVec2f		getCenter() const;

    /// Extends Box2f (if necessary) to contain given 2D point
    void	extendBy(const SbVec2f &pt);

    /// Extends Box2f (if necessary) to contain given Box2f
    void	extendBy(const SbBox2f &r);

    /// Returns TRUE if intersection of given point and Box2f is not empty
    SbBool	intersect(const SbVec2f &pt) const;

    /// Returns TRUE if intersection of given Box2f and Box2f is not empty
    SbBool	intersect(const SbBox2f &bb) const;

    /// Sets the corners of the box.
    void	setBounds(float xmin, float ymin, float xmax, float ymax)
	{ min.setValue(xmin, ymin); max.setValue(xmax, ymax); }

    /// Sets the corners of the box.
    void	setBounds(const SbVec2f &_min, const SbVec2f &_max)
	{ min = _min; max = _max; }

    /// Gets the corners of the box.
    void	getBounds(float &xmin, float &ymin,
			  float &xmax, float &ymax) const
	{ min.getValue(xmin, ymin); max.getValue(xmax, ymax); }

    /// Gets the corners of the box.
    void	getBounds(SbVec2f &_min, SbVec2f &_max) const
	{ _min = min; _max = max; }

    /// Returns the closest point on the box to the given point.
    /// (Returns the point on the center of the X side if passed the center.)
    SbVec2f	getClosestPoint(const SbVec2f &point);
    
    /// Returns origin (minimum point) of box
    void	getOrigin(float &originX, float &originY) const
	{ originX = min[0]; originY = min[1]; }

    /// Returns size of box
    void	getSize(float &sizeX, float &sizeY) const
	{ sizeX = max[0] - min[0]; sizeY = max[1] - min[1]; }

    /// Returns aspect ratio (ratio of width to height) of box
    float	getAspectRatio() const
	{ return (max[0] - min[0]) / (max[1] - min[1]); }

    /// Sets box to contain nothing
    void makeEmpty();

    /// Checks if the box is empty (degenerate)
    /// note that this relies on boxes being completely degenerate if
    /// they are degenerate at all.  All member functions preserve this
    /// invariant.
    SbBool	isEmpty() const		{ return max[0] < min[0]; }

    /// Checks if the box has area; i.e., both dimensions have positive size
    SbBool	hasArea() const
	{ return (max[0] > min[0] && max[1] > min[1]); }

    /// Equality comparison
    friend INVENTOR_API int          operator ==(const SbBox2f &b1, const SbBox2f &b2);

    /// Inequality comparison
    friend INVENTOR_API int          operator !=(const SbBox2f &b1, const SbBox2f &b2)
	{ return !(b1 == b2); }

  private:
    // Minimum and maximum points
    SbVec2f	min, max;
};

#endif /* _SB_BOX2F_ */
