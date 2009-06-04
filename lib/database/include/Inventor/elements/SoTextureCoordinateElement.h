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
 |	This file defines the SoTextureCoordinateElement class.
 |   TextureCoordinates can be specified in several different ways,
 |   from several different nodes:  they can be explicitly given by
 |   TextureCoordinate2/4 nodes, they can be automatically generated
 |   (the TextureCoordinateFunction nodes), or they can be DEFAULT,
 |   meaning that each shape must generate its own texture
 |   coordinates (can be set by TextureCoordinateFunctionDefault
 |   node).
 |
 |   Shapes will have to call TextureCoordinateElement::getType() and
 |   then call either generate default texture coordinates if it
 |   returns DEFAULT, call get2/get4 if it returns EXPLICIT, or call
 |   get(point, normal) if it returns FUNCTION.
 |
 |   Author(s)		: Gavin Bell
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

#ifndef  _SO_TEXTURE_COORDINATE_ELEMENT
#define  _SO_TEXTURE_COORDINATE_ELEMENT

#include <Inventor/SbLinear.h>
#include <Inventor/elements/SoReplacedElement.h>

//////////////////////////////////////////////////////////////////////////////
//
//  Class: SoTextureCoordinateElement
//
//  Element storing the current texture coordinates
//
//////////////////////////////////////////////////////////////////////////////

// Function that TextureCoordinateFunction nodes register to compute
// texture coordinates.  Given the point and normal, compute a texture
// coordinate and return it:
// C-api: end
typedef const SbVec4f &
	SoTextureCoordinateFunctionCB(void *userdata, const SbVec3f &point,
				      const SbVec3f &normal);
// C-api: begin

SoEXTENDER class INVENTOR_API SoTextureCoordinateElement : public SoReplacedElement {

    SO_ELEMENT_HEADER(SoTextureCoordinateElement);

  public:
    // The (seemingly random) choice of values is for compatibility
    // with Inventor 2.0 binary format files:
    enum CoordType {
	EXPLICIT = 1,	// Coordinates stored in state
        FUNCTION = 2	// Coordinates generated by software function
    };

    // Initializes element.
    virtual void	init(SoState *state);

    // Sets the current texture coordinates, in any of several ways:
    // EXPLICIT, no coordinates (shapes will generate):
    static void		setDefault(SoState *state, SoNode *node);

    // FUNCTION:
    static void		setFunction(SoState *state, SoNode *node,
				    SoTextureCoordinateFunctionCB *func,
				    void *userData);
    // EXPLICIT:
    static void		set2(SoState *state, SoNode *node,
			     int32_t numCoords, const SbVec2f *coords);
    static void		set4(SoState *state, SoNode *node,
			     int32_t numCoords, const SbVec4f *coords);

    // Returns code indicating what has been set in state/element
    static CoordType	getType(SoState *state);
    virtual CoordType	getType() const;

    // Returns the top (current) instance of the element in the state.
    // The get routines are called on an instance because it is more
    // efficient than calling a static method that looks up the
    // element in the state for every coordinate.
    static const SoTextureCoordinateElement * getInstance(SoState *state);

    // Get routine for FUNCTION case:

    // Given point and normal, returns texture coordinate. (The r
    // coordinate will be 0.0 and the q will be 1.0 until we get 3D
    // texture coordinate functions.)
    const SbVec4f &	get(const SbVec3f &point, const SbVec3f &normal) const;

    // Get routines for EXPLICIT case:

    // Returns the number of coordinate points in an instance.
    int32_t		getNum() const		{ return numCoords; }

    SbBool		is2D() const		{ return coordsAre2D; }

    // Returns the indexed coordinate from an element as a 2- or
    // 4-vector, converting if necessary.
    const SbVec2f &	get2(int index) const;
    const SbVec4f &	get4(int index) const;

    virtual void	print(FILE *fp) const;

  SoINTERNAL public:
    // Initializes the SoTextureCoordinateElement class
    static void		initClass();

  protected:
    // What kind of coordinates will be done:
    CoordType		whatKind;

    // Storage for FUNCTION:
    SoTextureCoordinateFunctionCB	*funcCB;
    void				*funcCBData;

    // Storage for EXPLICIT:
    int32_t 		numCoords;
    const SbVec2f	*coords2;
    const SbVec4f	*coords4;
    SbBool		coordsAre2D;

    virtual ~SoTextureCoordinateElement();

  private:
    SbVec2f		convert2;	// To convert from 4-D to 2-D
    SbVec4f		convert4;	// To convert from 2-D to 4-D
};

#endif /* _SO_TEXTURE_COORDINATE_ELEMENT */
