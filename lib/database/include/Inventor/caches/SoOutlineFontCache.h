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

#include <GL/glu.h>
#include <Inventor/SbBox.h>
#include <Inventor/caches/SoCache.h>
#include <Inventor/fields/SoMFString.h>

// Font library:
#include <flclient.h>
#include <iconv.h>

class SoFontOutline;
class SoGLDisplayList;

//
// Internal class: SoOutlineFontCache
//

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

    // Checks to see if this font is valid
    SbBool	isValid(const SoState *state) const;

    // Figures out if this cache is valid for rendering (the base
    // class isValid can be used for all other actions)
    SbBool	isRenderValid(SoState *state) const;

    // Returns the width of specified line number
    float	getWidth(int line);

    // Returns height of font
    float	getHeight() { return fontSize; }

    // Returns the 2D bounding box of a UCS character
    void	getCharBBox(const char* c, SbBox2f &result);
    // ... and the bounding box of the font's bevel
    void	getProfileBBox(SbBox2f &result);

    // Return the first/last point in the profile:
    void	getProfileBounds(float &firstZ, float &lastZ);

    // Returns TRUE if there _is_ any profile
    // (if not, act as if SIDES of text are off)
    SbBool	hasProfile() const { return  (nProfileVerts > 1); }

    // Returns how far to advance after drawing given character:
    SbVec2f	getCharOffset(const char* c);

    // Uses the given glu tesselator to generate triangles for the
    // given character.  This is used for both rendering and
    // generating primitives, with just different callback routines
    // registered.
    void	generateFrontChar(const char* c, GLUtesselator *tobj);
    // Ditto, for sides of characters:
    void	generateSideChar(const char* c, SideCB callbackFunc);

    // Set up for GL rendering:
    void	setupToRenderFront(SoState *state);
    void	setupToRenderSide(SoState *state, SbBool willTexture);

    // Returns TRUE if this font cache has a display list for the
    // given UCS character.  It will try to build a display list, if it
    // can.
    SbBool	hasFrontDisplayList(const char* c, GLUtesselator *tobj);
    SbBool	hasSideDisplayList(const char* c, SideCB callbackFunc);

    // Renders an entire line by using the GL callList() function.
    void	callFrontLists(int line);
    void	callSideLists(int line);

    // Renders a  UCS string in cases where display lists can't be buit.
    void	renderFront(int line,   GLUtesselator *tobj);
    void	renderSide(int line,  SideCB callbackFunc);

    // Callback registered with GLU used to detect tesselation errors.
    static void errorCB(GLenum whichErr);

    //Convert string to UCS format, keep a copy in this cache.
    //Use nodeid to know when to reconvert.
    void	convertToUCS(uint32_t nodeid, const SoMFString& string);

    //Returns line of UCS-2 text
    char *	getUCSString(int line)
    { return (char*)UCSStrings[line];}

    int		getNumUCSChars(int line)
    { return (int)(long)UCSNumChars[line];}

    GLubyte *createUniFontList(const char* fontNameList);

  protected:

    // Free up display lists before being deleted
    virtual void	destroy(SoState *state);

  private:
    // Constructor
    SoOutlineFontCache(SoState *);
    // Destructor
    ~SoOutlineFontCache();

    // Return a convnient little class representing a UCS character's
    // outline.
    SoFontOutline	*getOutline(const char* c);

    // Some helper routines for generateSide:
    void figureSegmentNorms(SbVec2f *result, int nPoints,
     const SbVec2f *points, float cosCreaseAngle, SbBool isClosed);
    void figureSegmentTexCoords(float *texCoords, int nPoints,
     const SbVec2f *points, SbBool isClosed);
    void fillBevel(SbVec3f *result, int nPoints,
           const SbVec2f *points, const SbVec2f &translation,
           const SbVec2f &n1, const SbVec2f &n2);
    void fillBevelN(SbVec3f *result, int nPoints,
            const SbVec2f *norms, const SbVec2f &n);

    // Texture coordinates in side display lists
    int		sidesHaveTexCoords;

    // Number of characters in this font.

    int		numChars;

    // Display lists for fronts, sides:
    SoGLDisplayList *frontList;
    SoGLDisplayList *sideList;

    // Profile information:
    float	cosCreaseAngle;
    int32_t	nProfileVerts;	// Number of points in profile
    SbVec2f	*profileVerts;	// Profile vertices
    float	*sTexCoords;	// Texture coordinates along profile
                    // (nProfileVerts of them)
    SbVec2f	*profileNorms;	// Profile normals
                // ((nProfileVerts-1)*2 of them)

    // This flag will be true if there is another cache open (if
    // building GL display lists for render caching, that means we
    // can't also build display lists).
    SbBool	otherOpen;

    // And tables telling us if a display list has been created for
    // each character in the font (we do that lazily since it is
    // expensive):
    SbDict* frontDict;
    SbDict* sideDict;

    // dictionary of outlines; these are also cached and created when
    // needed.
    SbDict* outlineDict;

    // Remember nodeId that was used to do UCS translation.
    uint32_t	currentNodeId;

    static iconv_t	conversionCode;

    // Font size
    float	fontSize;

    // Flag used to detect tesselation errors:
    static SbBool tesselationError;

    // List of font numbers for this font-list
    GLubyte* fontNumList;
    SbPList* fontNums;

    // char* pointers of UCS-2 strings:
    SbPList	UCSStrings;
    // size of these strings, in UCS-2 characters:
    SbPList	UCSNumChars;

    // Font library context for all outline fonts
    static FLcontext	context;

    // Global list of available fonts; a 'font' in this case is a
    // unique set of font name, font size, complexity value/type, and
    // set of profiles-- if any of these changes, the set of polygons
    // representing the font will change, and a different font will be
    // used.
    static SbPList	*fonts;
    // Free up an outline (invoked by SbDict::applyToAll):
    static void freeOutline( unsigned long,  void* value);
};

#endif /* _SO_OUTLINE_FONT_CACHE_ */
