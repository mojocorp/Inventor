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
 |	This file defines the SoTextDetail class.
 |
 |   Author(s)		: Thaddeus Beier, Dave Immel, Howard Look
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

#pragma once

#include <Inventor/details/SoSubDetail.h>
#include <Inventor/nodes/SoText3.h>

/// Stores detail information about a text node.
/// \ingroup Details
/// This class contains detail information about a point on a text shape
/// (<tt>SoText2</tt> or <tt>SoText3</tt>). It contains the part of the text,
/// string, and character that were hit or generated. \sa SoText2, SoText3,
/// SoDetail, SoPickedPoint, SoPrimitiveVertex
class INVENTOR_API SoTextDetail : public SoDetail {

    SO_DETAIL_HEADER(SoTextDetail);

  public:
    /// Constructor
    SoTextDetail();

    /// Destructor
    virtual ~SoTextDetail();

    /// Returns the index of the string within a multiple-value string
    /// fields of a text node
    int32_t getStringIndex() const { return stringIndex; }

    /// Returns the index of the character within the string. For
    /// example, if the character of detail was the "u" within
    /// "Splurmph", the character index would be 3.
    int32_t getCharacterIndex() const { return charIndex; }

    /// For SoText3, this returns which part was picked or generated.
    SoText3::Part getPart() const { return part; }

    /// Returns an instance that is a copy of this instance. The caller
    /// is responsible for deleting the copy when done.
    virtual SoDetail *copy() const;

    SoEXTENDER
  public:
    // For Text3, this sets which part is picked:
    void setPart(SoText3::Part p) { part = p; }

    // These set the string and character indices:
    void setStringIndex(int32_t i) { stringIndex = i; }
    void setCharacterIndex(int32_t i) { charIndex = i; }

    SoINTERNAL
  public:
    static void initClass();

  private:
    int32_t       stringIndex, charIndex;
    SoText3::Part part;
};

