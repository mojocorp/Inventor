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
 |	This file defines the SoText3 node class.
 |
 |   Author(s)		: Gavin Bell
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

#pragma once

#include <Inventor/SbBox.h>
#include <Inventor/fields/SoMFString.h>
#include <Inventor/fields/SoSFEnum.h>
#include <Inventor/fields/SoSFBitMask.h>
#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/nodes/SoShape.h>

class SoOutlineFontCache;
class SoPrimitiveVertex;
class SoTextureCoordinateElement;

/// 3D text shape node.
/// \ingroup Nodes
/// This node defines one or more strings of 3D text. In contrast with
/// <tt>SoText2</tt>, 3D text can be rotated, scaled, lighted, and textured,
/// just like all other 3D shapes. Each character in a 3D text string is
/// created by extruding an outlined version of the character (in the
/// current typeface) along the current profile, as defined by nodes
/// derived from <tt>SoProfile</tt>. The default text profile, if none is
/// specified, is a straight line segment one unit long.
///
/// The text origin is at (0,0,0) after applying the current
/// transformation. The scale of the text is affected by the <b>size</b>
/// field of the current <tt>SoFont</tt> as well as the current transformation.
///
/// <tt>SoText3</tt> uses the current set of materials when rendering. If the
/// material binding is <b>OVERALL</b>, then the whole text is drawn with the
/// first material. If it is <b>PER_PART</b> or <b>PER_PART_INDEXED</b>, the
/// front part of the text is drawn with the first material, the sides
/// with the second, and the back with the third.
///
/// Textures are applied to 3D text as follows.  On the front and back
/// faces of the text, the texture origin is at the base point of the
/// first string; the base point is at the lower left for justification
/// <b>LEFT</b>, at the lower right for <b>RIGHT</b>, and at the lower center
/// for <b>CENTER</b>. The texture is scaled equally in both S and T
/// dimensions, with the font height representing 1 unit. S increases to
/// the right on the front faces and to the left on the back faces. On the
/// sides, the texture is scaled the same as on the front and back. S is
/// equal to 0 at the rear edge of the side faces. The T origin can occur
/// anywhere along each character, depending on how that character's
/// outline is defined.
///
/// \par Action behavior:
/// <b>SoGLRenderAction</b>
/// Draws text based on the current font, profiles, transformation,
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
/// SoText3 {
///    string           ""
///    spacing          1
///    justification    LEFT
///    parts            FRONT
/// }
/// \endcode
/// \sa SoFont,SoProfile,SoText2,SoTextDetail
class SoText3 : public SoShape {

    SO_NODE_HEADER(SoText3);

  public:
    /// Justification types
    enum Justification {
        LEFT = 0x01,  ///< Left edges of all strings are aligned
        RIGHT = 0x02, ///< Right edges of all strings are aligned
        CENTER = 0x03 ///< Centers of all strings are aligned
    };

    /// Justification types
    enum Part {
        FRONT = 0x01, ///< Front faces of characters
        SIDES = 0x02, ///< Extruded sides of characters
        BACK = 0x04,  ///< Back faces of characters
        ALL = 0x07    ///< All parts
    };

    // Fields
    /// The text string(s) to display. Each string will appear on its own line.
    SoMFString string;

    /// Defines the distance (in the negative y direction) between the base
    /// points of successive strings, measured with respect to the current
    /// font height. A value of 1 indicates single spacing, a value of 2
    /// indicates double spacing, and so on.
    SoSFFloat spacing;

    /// Which parts of text are visible. Note that, for speed, the default for
    /// this field is <b>FRONT</b> only.
    SoSFBitMask parts;
    /// Indicates placement and alignment of strings. With <b>LEFT</b>
    /// justification, the left edge of the first line is at the (transformed)
    /// origin, and all left edges are aligned. <b>RIGHT</b> justification is
    /// similar.  <b>CENTER</b> justification places the center of the first
    /// string at the (transformed) origin, with the centers of all remaining
    /// strings aligned under it.
    SoSFEnum justification;

    /// Creates a 3D text node with default settings.
    SoText3();

    SoEXTENDER
  public:
    // Method to get the bounds of the given character in the given
    // string.  This must be called during the application of an
    // action; use a callbackAction applied to a Path to the text node
    // if you have to (this is really designed to be used in a Text3
    // manipulator, which will have direct access to the state).
    // The bounds are based on the font metric information, not the
    // geometric bounding box of the character.  So, for example,
    // while a space character has an empty bounding box,
    // getCharacterBounds() will return a box as high as the font
    // height, as wide as a space, and as deep as the profile.
    SbBox3f getCharacterBounds(SoState *state, int stringIndex, int charIndex);

    virtual void GLRender(SoGLRenderAction *action);
    virtual void rayPick(SoRayPickAction *action);

    SoINTERNAL
  public:
    static void initClass();

  protected:
    // Computes bounding box of text
    virtual void computeBBox(SoAction *action, SbBox3f &box, SbVec3f &center);

    // Generates primitives
    virtual void generatePrimitives(SoAction *);

    virtual ~SoText3();

  private:
    // Setup the internal font cache
    SbBool setupFontCache(SoState *state, SbBool forRender = FALSE);

    // Return bounding box of the fronts of all characters.
    SbBox2f getFrontBBox();

    // Figure out how much each line of text is offset (based on width
    // of characters and justification)
    SbVec2f getStringOffset(int line);

    // Figure out where a particular character in a particular line
    // starts:
    SbVec2f getCharacterOffset(int line, int whichChar);

    // Creates a text detail when picking:
    SoDetail *createTriangleDetail(SoRayPickAction *, const SoPrimitiveVertex *,
                                   const SoPrimitiveVertex *,
                                   const SoPrimitiveVertex *, SoPickedPoint *);

    // Generates the fronts of the characters, by getting the outlines
    // and calling the glu tesselation code:
    void generateFront(int line);

    // Generates the sides (bevel) of the characters
    void generateSide(int line);

    // Callback functions, so the same routines (inside the internal
    // SoOutlineFontCache class) can be used to generate primitives
    // and render.
    static void generateSideTris(int nPoints, const SbVec3f *p1,
                                 const SbVec3f *n1, const SbVec3f *p2,
                                 const SbVec3f *n2, const float *sTexCoords,
                                 const float *tTexCoords);
    static void renderSideTris(int nPoints, const SbVec3f *p1,
                               const SbVec3f *n1, const SbVec3f *p2,
                               const SbVec3f *n2, const float *sTexCoords,
                               const float *tTexCoords);

    // Static callbacks invoked by the glu tesselation code:
    static void beginCB(unsigned int primType);
    static void endCB();
    static void vtxCB(void *vertex);

    // Private data:
    // SoOutlineFontCache is an internal, opaque class used to
    // maintain gl display lists and other information for each
    // character in a font.
    SoOutlineFontCache *fontCache;

    // All this stuff is used while generating primitives:
    static SoText3 *                         currentGeneratingNode;
    static SoPrimitiveVertex *               genPrimVerts[3];
    static SbVec3f                           genTranslate;
    static int                               genWhichVertex;
    static uint32_t                          genPrimType;
    static SoAction *                        genAction;
    static SbBool                            genBack;
    static SbBool                            genTexCoord;
    static const SoTextureCoordinateElement *tce;
};

