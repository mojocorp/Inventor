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
 |	SoTransparencyTypeElement - Element that stores the current transparency
 type.
 |
 |   Author(s)		: Morgan Leborgne
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

#pragma once

//////////////////////////////////////////////////////////////////////////////
//
//  Class: SoTransparencyTypeElement
//
//  Element that stores the current transparency type.
//
//////////////////////////////////////////////////////////////////////////////

#include <Inventor/elements/SoInt32Element.h>

/// Element that stores the current transparency type.
SoEXTENDER
class INVENTOR_API SoTransparencyTypeElement : public SoInt32Element {

    SO_ELEMENT_HEADER(SoTransparencyTypeElement);

  public:
    /// Various levels of transparency rendering quality
    enum TransparencyType {
        SCREEN_DOOR, ///< Use stipple patterns for screen-door transparency
        ADD,         ///< Use additive alpha blending
        DELAYED_ADD, ///< Uses additive blending, rendering all transparent
                     ///< objects after opaque ones
        SORTED_OBJECT_ADD, ///< Same as DELAYED_ADD, but sorts transparent
                           ///< objects by distances of bounding boxes from
                           ///< camera
        BLEND,             ///< Uses multiplicative alpha blending
        DELAYED_BLEND, ///< Uses multiplicative alpha blending, rendering all
                       ///< transparent objects after opaque ones
        SORTED_OBJECT_BLEND ///< Same as DELAYED_BLEND, but sorts transparent
                            ///< objects by distances of bounding boxes from
                            ///< camera
    };

    /// Initializes element
    virtual void init(SoState *state);

    /// Sets the current transparency type in the state
    static void set(SoState *state, TransparencyType type);

    /// Returns current transparency type from the state
    static TransparencyType get(SoState *state) {
        return (TransparencyType)SoInt32Element::get(classStackIndex, state);
    }

    /// Returns the default transparency type
    static TransparencyType getDefault() { return SCREEN_DOOR; }

    /// Prints element (for debugging)
    virtual void print(FILE *fp) const;

    SoINTERNAL
  public:
    // Initializes the SoTransparencyTypeElement class
    static void initClass();

  protected:
    virtual ~SoTransparencyTypeElement();
};

