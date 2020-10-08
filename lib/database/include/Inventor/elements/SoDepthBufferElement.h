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
 |   $Revision: 1.1 $
 |
 |   Description:
 |	This file defines the SoDepthBufferElement class.
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

#pragma once

#include <Inventor/SbVec.h>
#include <Inventor/elements/SoSubElement.h>

class SoDepthBufferElement : public SoElement {

    SO_ELEMENT_HEADER(SoDepthBufferElement);

  public:
    enum DepthWriteFunction {
        NEVER,
        ALWAYS,
        LESS,
        LEQUAL,
        EQUAL,
        GEQUAL,
        GREATER,
        NOTEQUAL
    };

    /// Initializes element
    virtual void init(SoState *state);

    /// Comparison based on value of floats
    virtual SbBool matches(const SoElement *elt) const;

    /// Copy method, takes care copying the data.
    virtual SoElement *copyMatchInfo() const;

    /// Prints element (for debugging)
    virtual void print(FILE *fp) const;

    static void set(SoState *state, SbBool test, SbBool write,
                    DepthWriteFunction function,
                    const SbVec2f &    range = getDefaultRange());

    static void get(SoState *state, SbBool &test_out, SbBool &write_out,
                    DepthWriteFunction &function_out, SbVec2f &range_out);

    /// Returns each default value
    static bool               getDefaultTest() { return true; }
    static bool               getDefaultWrite() { return true; }
    static DepthWriteFunction getDefaultFunction() { return LESS; }
    static SbVec2f            getDefaultRange() { return SbVec2f(0.0f, 1.0f); }

    SoINTERNAL
  public:
    // Initializes the SoDepthBufferElement class
    static void initClass();

  protected:
    // Virtual set methods that subclasses can override.
    virtual void setElt(SbBool test, SbBool write, DepthWriteFunction function,
                        const SbVec2f &range);

    virtual ~SoDepthBufferElement();

    SbBool             test;
    SbBool             write;
    DepthWriteFunction function;
    SbVec2f            range;
};

