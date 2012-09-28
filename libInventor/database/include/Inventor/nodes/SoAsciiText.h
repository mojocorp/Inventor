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

/**************************************************************************
 * 		 Copyright (C) 1991-94, Silicon Graphics, Inc.		  *
 *  These coded instructions, statements, and computer programs  contain  *
 *  unpublished  proprietary  information of Silicon Graphics, Inc., and  *
 *  are protected by Federal copyright law.  They  may  not be disclosed  *
 *  to  third  parties  or copied or duplicated in any form, in whole or  *
 *  in part, without the prior written consent of Silicon Graphics, Inc.  *
 **************************************************************************/

//  -*- C++ -*-

/*
 *
 _______________________________________________________________________
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 |
 |   $Revision: 1.1 $
 |
 |   Description:
 |	This file defines the SoAsciiText node class.
 |
 |   Author(s)		: Chris Marrin,  Gavin Bell
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

#ifndef  _SO_ASCIITEXT_
#define  _SO_ASCIITEXT_

#include <Inventor/SbBox2f.h>
#include <Inventor/fields/SoMFString.h>
#include <Inventor/fields/SoSFEnum.h>
#include <Inventor/fields/SoSFBitMask.h>
#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/fields/SoMFFloat.h>
#include <Inventor/nodes/SoShape.h>

#include <Inventor/misc/SoGL.h>   // For GLenum declaration

//////////////////////////////////////////////////////////////////////////////
//
//  Class: SoAsciiText
//
//  Specify 3D text for Vrml.
//
//////////////////////////////////////////////////////////////////////////////

class SoOutlineFontCache;
class SoPrimitiveVertex;
class SoTextureCoordinateElement;

/// Simple 3D text shape node.
/// \ingroup Nodes
/// This node defines one or more strings of 3D text. In contrast with
/// <tt>SoText2</tt>, 3D text can be rotated, scaled, lighted, and textured,
/// just like all other 3D shapes. In constrast with
/// <tt>SoText3</tt>, this 3D ascii text cannot be extruded.
///
///
/// The text origin is at (0,0,0) after applying the current
/// transformation. The scale of the text is affected by the #size
/// field of the current <tt>SoFont</tt> as well as the current transformation.
///
///
/// <tt>SoAsciiText</tt> uses the current material when rendering.
/// Textures are applied to ascii text as follows.  On the front
/// face of the text, the texture origin is at the base point of the
/// first string; the base point is at the lower left for justification
/// <b>LEFT</b>, at the lower right for <b>RIGHT</b>, and at the lower center
/// for <b>CENTER</b>. The texture is scaled equally in both S and T
/// dimensions, with the font height representing 1 unit. S increases to
/// the right on the front face.
///
/// \par Action behavior:
/// <b>SoGLRenderAction</b>
/// Draws text based on the current font, transformation,
/// drawing style, material, texture, complexity, and so on.
/// <b>SoRayPickAction</b>
/// Performs a pick on the text. The string index and character position
/// are available from the <tt>SoTextDetail</tt>.
/// <b>SoGetBoundingBoxAction</b>
/// Computes the bounding box that encloses the text.
/// <b>SoCallbackAction</b>
/// If any triangle callbacks are registered with the action, they will be
/// invoked for each successive triangle used to approximate the text
/// geometry.
///
/// \par File format/defaults:
/// \code
/// SoAsciiText {
///    string           ""
///    spacing          1
///    justification    LEFT
///    width            0
/// }
/// \endcode
/// \sa SoFont,SoFontStyle,SoText2,SoText3,SoTextDetail
class INVENTOR_API SoAsciiText : public SoShape {

    SO_NODE_HEADER(SoAsciiText);

  public:
    /// Justification types
    enum Justification {
	LEFT	= 0x01,
	RIGHT	= 0x02,
	CENTER	= 0x03
    };

    // Fields
    SoMFString		string;		///< the strings to display
    SoSFFloat		spacing;	///< interval between strings
    SoSFEnum		justification;
    SoMFFloat		width;		///< width of each string

    /// Constructor
    SoAsciiText();

  SoEXTENDER public:

    virtual void	GLRender(SoGLRenderAction *action);
    virtual void	rayPick(SoRayPickAction *action);

  SoINTERNAL public:
    static void		initClass();

  protected:
    // Computes bounding box of text
    virtual void	computeBBox(SoAction *action, SbBox3f &box,
				    SbVec3f &center);

    // Generates primitives
    virtual void	generatePrimitives(SoAction *);
    
    virtual ~SoAsciiText();

  private:
    // Setup the internal font cache
    SbBool setupFontCache(SoState *state, SbBool forRender = FALSE);

    // Return bounding box of the fronts of all characters.
    void getFrontBBox(SbBox2f &result);

    // Extend a 

    // Figure out how much each line of text is offset (based on width
    // of characters and justification)
    SbVec2f getStringOffset(int line, float width);
    
    // Creates a text detail when picking:
    SoDetail * createTriangleDetail(SoRayPickAction *,
				    const SoPrimitiveVertex *,
				    const SoPrimitiveVertex *,
				    const SoPrimitiveVertex *,
				    SoPickedPoint *);

    // Generates the fronts of the characters, by getting the outlines
    // and calling the glu tesselation code:
    void generateFront(const SbString &string, float width);
    
    // Static callbacks invoked by the glu tesselation code:
    static void beginCB(GLenum primType);
    static void endCB();
    static void vtxCB(void *vertex);
    
    // Private data:
    // MyOutlineFontCache is an internal, opaque class used to
    // maintain gl display lists and other information for each
    // character in a font.
    SoOutlineFontCache *myFont;

    // All this stuff is used while generating primitives:
    static SoAsciiText *currentGeneratingNode;
    static SoPrimitiveVertex *genPrimVerts[3];
    static SbVec3f genTranslate;
    static int genWhichVertex;
    static uint32_t genPrimType;
    static SoAction *genAction;
    static SbBool genBack;
    static SbBool genTexCoord;
    static const SoTextureCoordinateElement *tce;
};

#endif /* _SO_ASCIITEXT_ */
