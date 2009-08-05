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
 |   $Revision: 1.2 $
 |
 |   Description:
 |	This file contains the SbColor class definition.
 |
 |   Author(s)	: Alain Dumesny
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

#ifndef  _SB_COLOR_H_
#define  _SB_COLOR_H_

#include <Inventor/SbVec3f.h>


//////////////////////////////////////////////////////////////////////////////
//
//  Class: SbColor
//
//  3D vector used to represent an RGB color. Each component of the vector is 
// a floating-point number between 0.0 and 1.0 .
//
//////////////////////////////////////////////////////////////////////////////

class SbColor;




class INVENTOR_API SbColor : public SbVec3f {
  public:
    // Default constructor
    SbColor();

    // Constructor given an SbVec3f
    SbColor(const SbVec3f & vec3f) { setValue(vec3f.getValue()); }

    // Constructor given an array of 3 components
    SbColor(const float rgb[3])				{ setValue(rgb); }

    // Constructor given 3 individual components
    SbColor(float r, float g, float b)			{ setValue(r, g, b); }

    //
    // HSV routines. Those are 3 floats containing the Hue, Saturation and
    // Value (same as brightness) of the color.
    //

    // Sets value of color vector from 3 hsv components
    SbColor &	setHSVValue(float h, float s, float v);

    // Sets value of color vector from array of 3 hsv components
    SbColor &	setHSVValue(const float hsv[3])
			{ return setHSVValue(hsv[0], hsv[1], hsv[2]); }

    // Returns 3 individual hsv components
    void	getHSVValue(float &h, float &s, float &v) const;

    // Returns an array of 3 hsv components
    void	getHSVValue(float hsv[3]) const
 			{ getHSVValue(hsv[0], hsv[1], hsv[2]); }
    
    //
    // RGBA Packed integer color routines. The color format expressed in 
    // hexadecimal is 0xrrggbbaa, where
    //	    aa 	is the alpha value
    //	    bb 	is the blue value
    //	    gg 	is the green value
    //	    rr 	is the red value
    // RGBA component values range from 0 to 0xFF (255).
    //
    
    // Sets value from ordered RGBA packed color. Alpha value is used for
    // transparency.
    SbColor &	setPackedValue(uint32_t orderedRGBA, float& transparency);

    // Returns ordered RGBA packed color. Alpha is 1 - transparency, scaled
    // between 0 and 255 = 0xFF.
    uint32_t   getPackedValue(float transparency = 0.0) const;

// NOTE: These Macros are stolen from SbVec3f because the C-api
// generator does NOT generate macros for macros defined by a parent
// class. Thus they must be done `by hand'.  Yes, this does suck.









};

#endif /* _SB_COLOR_H_ */

