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
 | This file defines the SoText2 node class.
 |
 |   Author(s)  : Thad Beier
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

#ifndef  _SO_BITMAP_FONT_CACHE_
#define  _SO_BITMAP_FONT_CACHE_

#include <Inventor/caches/SoCache.h>
#include <Inventor/SbBox3f.h>
#include <Inventor/fields/SoMFString.h>
#include <Inventor/SbStdint.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <vector>
#include <map>

// An internal class that makes life easier:

// This very specialized cache class is used to cache bitmaps and GL
// display lists containing bitmaps.  It is strange because it doesn't
// use the normal list of elements used to determine validity, etc,
// and knows exactly which elements it depends on.
class SoGLDisplayList;

class SoBitmapFontCache : public SoCache {
public:
    // Return a font (either a new one or an old one) that is valid
    // for the given state.
    static SoBitmapFontCache * getFont(SoState *state, SbBool forRender);

    // Use this when rendering to decide if this cache is valid (it
    // checks the GL cache context in addition to other elements)
    SbBool isRenderValid(SoState *state) const;

    // Returns the amount the current raster position will be advanced
    // after drawing the given character.
    SbVec3f getCharOffset(char c);

    // Get the pixel-space bounding box of a given character.
    void getCharBbox(char c, SbBox3f &box);

    // Gets the width (in pixels) of the given string
    float getWidth(const SbString &str);

    // Gets the height of the font, in pixels
    float getHeight();

    // Draws the given string
    void drawString(SoState *state, const SbString &string);


protected:
    // Free up display lists before being deleted
    virtual void destroy(SoState *state);

private:
    typedef struct FLbitmap {
         int width;
         int height;
         float xorig;
         float yorig;
         float xmove;
         float ymove;
         unsigned char *bitmap;
     } FLbitmap;

    // Constructor.
    SoBitmapFontCache(SoState *state);

    // Destructor
    virtual ~SoBitmapFontCache();

    // Draws the given character (using GL)
    void drawCharacter(unsigned char c);

    // Returns TRUE if this font cache has a display list for the
    // given character.  It will try to build a display list, if it
    // can.
    SbBool hasDisplayList(SoState *state, unsigned char c);

    const FLbitmap *getBitmap(unsigned char c);

    // Static list of all fonts.  OPTIMIZATION:  If there turn out to
    // be applications that use lots of fonts, we could change this
    // list into a dictionary keyed off the font name.
    static std::vector<SoBitmapFontCache*> fonts;

    int context;
    std::map<char, SoGLDisplayList*> list;
    std::map<char, FLbitmap*> bitmaps; // Cached bitmaps for each character.

    // And some font library stuff:
    static FT_Library library;
    FT_Face face;

    std::vector<unsigned char> buffer;
};
#endif /* _SO_BITMAP_FONT_CACHE_ */
