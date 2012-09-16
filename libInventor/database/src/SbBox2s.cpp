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
 |	SbBox2s
 |
 |   Author(s)		: Paul S. Strauss, Nick Thompson
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

#include <Inventor/SbBox2s.h>

#include <limits.h>
#include <float.h>    /* For FLT_MAX */

//////////////////////////////////////////////////////////////////////////////
//
//  Extends Box2s (if necessary) to contain given 2D point
//
void
SbBox2s::extendBy(const SbVec2s &pt)
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
// Extends Box2s (if necessary) to contain given Box2s
//
void
SbBox2s::extendBy(const SbBox2s &r)
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
// Returns TRUE if intersection of given point and Box2s is not empty
//
SbBool
SbBox2s::intersect(const SbVec2s &pt) const
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
// Returns TRUE if intersection of given Box2s and Box2s is not empty
//
SbBool
SbBox2s::intersect(const SbBox2s &r) const
//
//////////////////////////////////////////////////////////////////////////////
{
    return ((r.max[0] >= min[0]) && (r.min[0] <= max[0]) &&
	    (r.max[1] >= min[1]) && (r.min[1] <= max[1]));
}


//////////////////////////////////////////////////////////////////////////////
//
// Sets Box2s to contain nothing
//
void
SbBox2s::makeEmpty()
//
//////////////////////////////////////////////////////////////////////////////
{
    min.setValue(SHRT_MAX, SHRT_MAX);
    max.setValue(SHRT_MIN, SHRT_MIN);
}

//////////////////////////////////////////////////////////////////////////////
//
// Equality comparison operator. 
//
int
operator ==(const SbBox2s &b1, const SbBox2s &b2)
//////////////////////////////////////////////////////////////////////////////
{
    return ( (b1.min == b2.min) && (b1.max == b2.max ) );
}
