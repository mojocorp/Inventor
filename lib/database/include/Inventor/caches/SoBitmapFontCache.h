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

#pragma once

#include <string>
#include <vector>
#include <map>

#include <Inventor/caches/SoFontCache.h>
#include <Inventor/SbBox.h>

class SoGLDisplayList;

// An internal class that makes life easier:

// This very specialized cache class is used to cache bitmaps and GL
// display lists containing bitmaps.  It is strange because it doesn't
// use the normal list of elements used to determine validity, etc,
// and knows exactly which elements it depends on.

class INVENTOR_API SoBitmapFontCache : public SoFontCache {
  public:
    // Return a font (either a new one or an old one) that is valid
    // for the given state.
    static SoBitmapFontCache *getFont(SoState *state, SbBool forRender);

    // Use this when rendering to decide if this cache is valid (it
    // checks the GL cache context in addition to other elements)
    SbBool isRenderValid(SoState *state) const;

    // Returns the amount the current raster position will be advanced
    // after drawing the given character.
    SbVec3f getCharOffset(wchar_t c);

    // Get the pixel-space bounding box of a given character.
    SbBox3f getCharBbox(wchar_t c);

    // Gets the size (in pixels) of the given string
    SbVec2s getSize(const std::wstring &str);

    // Draws the given string
    void drawString(SoState *state, const std::wstring &string,
                    const SbVec3f &origin);

  protected:
    // Free up display lists before being deleted
    virtual void destroy(SoState *state);

  private:
    typedef struct FLbitmap {
        int                        width;
        int                        height;
        float                      xorig;
        float                      yorig;
        float                      xmove;
        float                      ymove;
        std::vector<unsigned char> bitmap;
    } FLbitmap;

    // Constructor.
    SoBitmapFontCache(SoState *state);

    // Destructor
    virtual ~SoBitmapFontCache();

    // Draws the given character (using GL)
    void drawCharacter(SoState *state, wchar_t c);

    const FLbitmap *getBitmap(wchar_t c);

    // Static list of all fonts.  OPTIMIZATION:  If there turn out to
    // be applications that use lots of fonts, we could change this
    // list into a dictionary keyed off the font name.
    static std::vector<SoBitmapFontCache *> fonts;

    int                                  context;
    std::map<wchar_t, SoGLDisplayList *> list;
    std::map<wchar_t, FLbitmap *> bitmaps; // Cached bitmaps for each character.
};
