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

#ifndef _SB_BOX2S_
#define _SB_BOX2s_

#include <Inventor/SbVec2s.h>

class INVENTOR_API SbBox2s {
  public:
    // Default constructor - leaves box totally empty
    SbBox2s()					{ makeEmpty(); };

    // Constructor given bounds 
    SbBox2s(short xmin, short ymin, short xmax, short ymax)
	{ min.setValue(xmin, ymin); max.setValue(xmax, ymax); }

    // Constructor given minimum and maximum points 
    SbBox2s(const SbVec2s &_min, const SbVec2s &_max)
	{ min = _min; max = _max; }

    ~SbBox2s()						{ }

    // Returns the min and max points
    const SbVec2s &	getMin() const			{ return min; }
    const SbVec2s &	getMax() const			{ return max; }

    // Extends Box2s (if necessary) to contain given 2D point
    void	extendBy(const SbVec2s &pt);

    // Extends Box2s (if necessary) to contain given Box2s
    void	extendBy(const SbBox2s &r);

    // Returns TRUE if intersection of given point and Box2s is not empty
    SbBool	intersect(const SbVec2s &pt) const;

    // Returns TRUE if intersection of given Box2s and Box2s is not empty
    SbBool	intersect(const SbBox2s &bb) const;

    // Common get and set functions
    void	setBounds(short xmin, short ymin, short xmax, short ymax)
	{ min.setValue(xmin, ymin); max.setValue(xmax, ymax); }

    void	setBounds(const SbVec2s &_min, const SbVec2s &_max)
	{ min = _min; max = _max; }

    void	getBounds(short &xmin, short &ymin,
			  short &xmax, short &ymax) const
	{ min.getValue(xmin, ymin); max.getValue(xmax, ymax); }

    void	getBounds(SbVec2s &_min, SbVec2s &_max) const
	{ _min = min; _max = max; }

    // Returns origin (minimum point) of box
    void	getOrigin(short &originX, short &originY) const
	{ originX = min[0]; originY = min[1]; }

    // Returns size of box
    void	getSize(short &sizeX, short &sizeY) const
	{ sizeX = max[0] - min[0]; sizeY = max[1] - min[1]; }

    // Returns aspect ratio (ratio of width to height) of box
    float	getAspectRatio() const
	{ return float(max[0] - min[0]) / float(max[1] - min[1]); }

    // Sets rect to contain nothing
    void	makeEmpty();

    // Equality comparisons
    friend INVENTOR_API int          operator ==(const SbBox2s &b1, const SbBox2s &b2);
    friend INVENTOR_API int          operator !=(const SbBox2s &b1, const SbBox2s &b2)
	{ return !(b1 == b2); }

  private:
    // Minimum and maximum points
    SbVec2s	min, max;
};

#endif /* _SB_BOX2S_ */
