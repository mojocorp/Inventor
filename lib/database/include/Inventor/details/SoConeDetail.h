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
 |   $Revision: 1.1 $
 |
 |   Description:
 |	This file defines the SoConeDetail class.
 |
 |   Author(s)		: Thaddeus Beier, Dave Immel, Howard Look
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

#ifndef _SO_CONE_DETAIL_
#define _SO_CONE_DETAIL_

#include <Inventor/details/SoSubDetail.h>

/// Stores detail information about the SoCone node.
/// \ingroup Details
/// This class contains detail information about a point on a cone.
/// It contains the part of the cone that was hit (sides or bottom).
/// \sa SoCone, SoDetail, SoPickedPoint, SoPrimitiveVertex
class INVENTOR_API SoConeDetail : public SoDetail {

    SO_DETAIL_HEADER(SoConeDetail);

  public:
    /// Constructor and destructor
    SoConeDetail();

    /// Destructor
    virtual ~SoConeDetail();

    /// Returns the part of detail for this shape, as one of the part masks
    /// of SoCone.
    int			getPart() const			{ return part; }

    /// Returns an instance that is a copy of this instance. The caller
    /// is responsible for deleting the copy when done.
    virtual SoDetail *	copy() const;

  SoEXTENDER public:
    // Sets the part of detail for the shape
    void		setPart(int _part)		{ part = _part; }

  SoINTERNAL public:
    static void		initClass();
    
  protected:
    int part;
};

#endif /* _SO_CONE_DETAIL_ */
