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
 | This file defines the SoOutlineFontCache class.
 |
 |   Author(s)  :
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

#ifndef  _SO_OUTLINE_FONT_CACHE_
#define  _SO_OUTLINE_FONT_CACHE_

#include <Inventor/misc/SoGL.h>
#include <Inventor/SbVec2f.h>
#include <Inventor/SbVec3f.h>
#include <Inventor/SbBox2f.h>
#include <Inventor/SbString.h>
#include <Inventor/caches/SoCache.h>

#include <vector>
#include <map>

#include <ft2build.h>
#include FT_FREETYPE_H

class SoFontOutline;
class SoGLDisplayList;

// Callback function for sides of characters-- passed the number of
// points going back, and points and normals on either edge of the
// strip.  tTexCoords[0] and [1] are for the two edges, and the
// sTexCoords are the same for both edges.
typedef void SideCB(int nPoints,
                    const SbVec3f *points1, const SbVec3f *norms1,
                    const SbVec3f *points2, const SbVec3f *norms2,
                    const float *sTexCoords, const float *tTexCoords);

// This is pretty heavyweight-- it is responsible for doing all of the
// grunt work of figuring out the polygons making up the characters in
// the font.
class SoOutlineFontCache : public SoCache
{
  public:
    // Given a state, find an appropriate outline font.
    static SoOutlineFontCache	*getFont(SoState *, SbBool forRender);

    // Figures out if this cache is valid for rendering (the base
    // class isValid can be used for all other actions)
    SbBool	isRenderValid(SoState *state) const;

    // Returns the width of given string
    float	getWidth(const SbString &string);

    // Returns height of font
    float	getHeight() const { return fontSize; }

    // Returns the 2D bounding box of a character
    void	getCharBBox(const wchar_t c, SbBox2f &result);
    // ... and the bounding box of the font's bevel
    void	getProfileBBox(SbBox2f &result);

    // Return the first/last point in the profile:
    void	getProfileBounds(float &firstZ, float &lastZ);

    // Returns TRUE if there _is_ any profile
    // (if not, act as if SIDES of text are off)
    SbBool	hasProfile() const { return  (nProfileVerts > 1); }

    // Returns how far to advance after drawing given character:
    SbVec2f	getCharOffset(const wchar_t c);

    // Uses the given glu tesselator to generate triangles for the
    // given character.  This is used for both rendering and
    // generating primitives, with just different callback routines
    // registered.
    void	generateFrontChar(const wchar_t c, GLUtesselator *tobj);
    // Ditto, for sides of characters:
    void	generateSideChar(const wchar_t c, SideCB callbackFunc);

    // Renders a string in cases where display lists can't be buit.
    void	renderFront(SoState *state, const SbString &string, GLUtesselator *tobj);
    void	renderSide(SoState *state, const SbString &string, SideCB callbackFunc);

    // Callback registered with GLU used to detect tesselation errors.
    static void errorCB(GLenum whichErr);

  private:
    // Constructor
    SoOutlineFontCache(SoState *);
    // Destructor
    ~SoOutlineFontCache();

    // Returns TRUE if this font cache has a display list for the
    // given character.  It will try to build a display list, if it
    // can.
    SbBool	hasFrontDisplayList(SoState *state, const wchar_t c, GLUtesselator *tobj);
    SbBool	hasSideDisplayList(SoState *state, const wchar_t c, SideCB callbackFunc);

    // Return a convnient little class representing a character's
    // outline.
    SoFontOutline *getOutline(const wchar_t c);

    // Some helper routines for generateSide:
    void figureSegmentNorms(std::vector<SbVec2f> & result, int nPoints, const SbVec2f *points, float cosCreaseAngle, SbBool isClosed);
    void figureSegmentTexCoords(std::vector<float> & texCoords, int nPoints, const SbVec2f *points, SbBool isClosed);
    void fillBevel(SbVec3f *result, int nPoints, const SbVec2f *points, const SbVec2f &translation, const SbVec2f &n1, const SbVec2f &n2);
    void fillBevelN(SbVec3f *result, const std::vector<SbVec2f> & norms, const SbVec2f &n);

    // Texture coordinates in side display lists
    int		sidesHaveTexCoords;

    // Display lists for fronts, sides:
    int context;
    std::map<wchar_t, SoGLDisplayList*> frontList;
    std::map<wchar_t, SoGLDisplayList*> sideList;

    // Profile information:
    float	cosCreaseAngle;
    int32_t	nProfileVerts;	// Number of points in profile
    SbVec2f	*profileVerts;	// Profile vertices
    std::vector<float>   sTexCoords;	// Texture coordinates along profile (nProfileVerts of them)
    std::vector<SbVec2f> profileNorms;	// Profile normals ((nProfileVerts-1)*2 of them)

    // List of outlines; these are also cached and created when needed.
    std::map<wchar_t, SoFontOutline*> outlines;

    // Font size
    float	fontSize;

    // Flag used to detect tesselation errors:
    static SbBool tesselationError;

    // Font library identifier for this font
    FT_Face	face;

    // Font library context for all outline fonts
    static FT_Library	library;

    // Global list of available fonts; a 'font' in this case is a
    // unique set of font name, font size, complexity value/type, and
    // set of profiles-- if any of these changes, the set of polygons
    // representing the font will change, and a different font will be
    // used.
    static std::vector<SoOutlineFontCache*>	fonts;

    std::vector<unsigned char> buffer;
};

#endif /* _SO_OUTLINE_FONT_CACHE_ */

