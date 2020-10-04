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
 |   $Revision: 1.6 $
 |
 |   Classes:
 |      SoBitmapFontCache
 |
 |   Author(s)          : Gavin Bell
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */
#include <cstring>
#include <algorithm>
#include <Inventor/SbViewportRegion.h>
#include <Inventor/caches/SoBitmapFontCache.h>
#include <Inventor/elements/SoCacheElement.h>
#include <Inventor/elements/SoGLDisplayList.h>
#include <Inventor/elements/SoGLCacheContextElement.h>
#include <Inventor/elements/SoViewportRegionElement.h>
#include <Inventor/elements/SoFontSizeElement.h>
#include <Inventor/errors/SoDebugError.h>
#include <glad/gl.h>

#include <ft2build.h>
#include FT_FREETYPE_H

//////////////////////////////////////////////////////////////////////////////
/////////////////////    SoBitmapFontCache  //////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
// Static variables for SoBitmapFontCache
std::vector<SoBitmapFontCache *> SoBitmapFontCache::fonts;

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Get a font cache suitable for using with the given state.
//
// Use: static, internal, public

SoBitmapFontCache *
SoBitmapFontCache::getFont(SoState *state, SbBool forRender)
//
////////////////////////////////////////////////////////////////////////
{
    SoBitmapFontCache *result = NULL;
    for (size_t i = 0; i < fonts.size() && result == NULL; i++) {
        SoBitmapFontCache *fc = fonts[i];
        if (forRender ? fc->isRenderValid(state) : fc->isValid(state)) {
            result = fc;
            result->ref();
        }
    }
    if (result == NULL) {
        result = new SoBitmapFontCache(state);

        // If error:
        if (result->face == 0) {
            delete result;
            return NULL;
        }
    }
    return result;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Construct a bitmap font cache, given the state and a dummy
//    (empty) list of overridden elements (needed only to pass to the
//    SoCache constructor).
//
// Use: internal, private

SoBitmapFontCache::SoBitmapFontCache(SoState *state)
    : SoFontCache(state), context(-1)
//
////////////////////////////////////////////////////////////////////////
{
    ref();

    // Grab all the stuff we'll need to determine our validity from
    // the state.
    const SbViewportRegion &vpr = SoViewportRegionElement::get(state);
    addElement(
        state->getConstElement(SoViewportRegionElement::getClassStackIndex()));

    const float fontSize =
        SoFontSizeElement::get(state) * vpr.getPixelsPerPoint();

    FT_Set_Pixel_Sizes(face, 0, (FT_UInt)fontSize);

    fonts.push_back(this);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Destructor.  Clean up-- GL stuff, font library stuff, and
//    internally generated stuff.
//
// Use: internal, private

SoBitmapFontCache::~SoBitmapFontCache()
//
////////////////////////////////////////////////////////////////////////
{
    if (face) {
        std::map<wchar_t, FLbitmap *>::iterator it;
        for (it = bitmaps.begin(); it != bitmaps.end(); ++it) {
            delete it->second;
        }
        // Only destroy the font library font if no other font caches
        // are using the same font identifier:
        SbBool otherUsing = FALSE;
        for (size_t i = 0; i < fonts.size(); i++) {
            SoBitmapFontCache *t = fonts[i];
            if (t != this && t->face == face)
                otherUsing = TRUE;
        }
        if (!otherUsing) {
            FT_Done_Face(face);
            face = NULL;
        }
        fonts.erase(std::find(fonts.begin(), fonts.end(), this));
    }
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Destroy this cache.  Called by unref(); frees up OpenGL display
//    lists.
//
// Use: protected, virtual

void
SoBitmapFontCache::destroy(SoState *)
//
////////////////////////////////////////////////////////////////////////
{
    // Pass in NULL to unref because this cache may be destroyed
    // from an action _other_ than GLRender:
    std::map<wchar_t, SoGLDisplayList *>::iterator it;
    for (it = list.begin(); it != list.end(); ++it) {
        it->second->unref(NULL);
    }
    list.clear();

    SoCache::destroy(NULL);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Returns TRUE if this cache is valid
//
// Use: internal, public

SbBool
SoBitmapFontCache::isRenderValid(SoState *state) const
//
////////////////////////////////////////////////////////////////////////
{
    if (list.empty())
        return isValid(state);
    else
        return (context == SoGLCacheContextElement::get(state) &&
                isValid(state));
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Returns the pixel-space bounding box of given character.
//
// Use: internal, public

SbBox3f
SoBitmapFontCache::getCharBbox(wchar_t c)
//
////////////////////////////////////////////////////////////////////////
{
    const FLbitmap *bmap = getBitmap(c);
    if (bmap == NULL)
        return SbBox3f();

    return SbBox3f(0, 0, 0, bmap->width, bmap->height, 0);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Returns the amount the current raster position will be advanced
//    after drawing the given character.
//
// Use: internal, public

SbVec3f
SoBitmapFontCache::getCharOffset(wchar_t c)
//
////////////////////////////////////////////////////////////////////////
{
    const FLbitmap *bmap = getBitmap(c);
    if (bmap != NULL)
        return SbVec3f(bmap->xmove, bmap->ymove, 0);

    return SbVec3f(0, 0, 0);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Returns the size of given string.
//
// Use: internal, public

SbVec2s
SoBitmapFontCache::getSize(const std::wstring &str)
//
////////////////////////////////////////////////////////////////////////
{
    SbVec2s result(0, 0);

    for (size_t i = 0; i < str.size(); i++) {
        const FLbitmap *bmap = getBitmap(str[i]);
        if (bmap != NULL) {
            result[0] += bmap->xmove;
            result[1] = std::max(result[1], (short)bmap->height);
        }
    }
    return result;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Draws a bitmap
//
// Use: internal public

void
SoBitmapFontCache::drawCharacter(SoState *state, wchar_t c)
//
////////////////////////////////////////////////////////////////////////
{
    // If we have a display list, call it
    SoGLDisplayList *displaylist = list[c];
    if (displaylist) {
        displaylist->call(state);
        return;
    }

    // If we don't and we can build one.
    if (!SoCacheElement::anyOpen(state)) {
        context = SoGLCacheContextElement::get(state);

        // Build one:
        displaylist = new SoGLDisplayList(state, SoGLDisplayList::DISPLAY_LIST);
        list[c] = displaylist;
        displaylist->ref();
        displaylist->addDependency(state);
        glNewList(displaylist->getFirstIndex(), GL_COMPILE);
    }

    const FLbitmap *bmap = getBitmap(c);
    if (bmap != NULL) {
        glBitmap(0, 0, 0, 0, bmap->xorig, bmap->yorig, NULL);
        if (!bmap->bitmap.empty()) {
            glDrawPixels(bmap->width, bmap->height, GL_LUMINANCE_ALPHA,
                         GL_UNSIGNED_BYTE, bmap->bitmap.data());
        }
        glBitmap(0, 0, 0.0f, 0.0f, bmap->xmove - bmap->xorig,
                 bmap->ymove - bmap->yorig, NULL);
    }

    if (!SoCacheElement::anyOpen(state)) {
        displaylist->close(state);
        displaylist->call(state);
    }
}
////////////////////////////////////////////////////////////////////////
//
// Description:
//    Draws a whole string.  Tries to build display lists, if it can.
//
// Use: internal public

void
SoBitmapFontCache::drawString(SoState *state, const std::wstring &string,
                              const SbVec3f &origin)
//
////////////////////////////////////////////////////////////////////////
{
    //
    // Set up OpenGL state for rendering text, and push
    // attributes so that we can restore when done.
    //
    glPushAttrib(GL_ENABLE_BIT | GL_PIXEL_MODE_BIT | GL_COLOR_BUFFER_BIT);
    glPushClientAttrib(GL_CLIENT_PIXEL_STORE_BIT);

    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.3f);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glRasterPos3fv(&origin[0]);

    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 2);

    for (size_t i = 0; i < string.size(); i++) {
        drawCharacter(state, string[i]);
    }

    //
    // Restore OpenGL state.
    //
    glPopClientAttrib();
    glPopAttrib();
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Returns a bitmap.
//
// Use: private
const SoBitmapFontCache::FLbitmap *
SoBitmapFontCache::getBitmap(wchar_t c)
//
////////////////////////////////////////////////////////////////////////
{
    FLbitmap *flbitmap = bitmaps[c];
    if (flbitmap)
        return flbitmap;

    flbitmap = new FLbitmap;
    bitmaps[c] = flbitmap;
    flbitmap->bitmap.clear();

    if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
#ifdef DEBUG
        SoDebugError::post("SoBitmapFontCache::getBitmap",
                           "FT_Load_Char failed");
#endif
    }

    FT_GlyphSlot glyph = face->glyph;
    FT_Bitmap    bitmap = glyph->bitmap;

    if (glyph->bitmap.pixel_mode != FT_PIXEL_MODE_GRAY) {
        SoError::post("SoBitmapFontCache::getBitmap", "Unsupported pixel mode");
    }

    flbitmap->width = bitmap.width;
    flbitmap->height = bitmap.rows;

    flbitmap->xorig = glyph->bitmap_left;
    flbitmap->yorig = float(glyph->bitmap_top) - glyph->bitmap.rows;
    flbitmap->xmove = glyph->advance.x >> 6;
    flbitmap->ymove = glyph->advance.y >> 6;

    if (bitmap.width && bitmap.rows) {
        // Allocate memory for the texture data.
        flbitmap->bitmap.resize(2 * flbitmap->width * flbitmap->height);

        unsigned char *src = face->glyph->bitmap.buffer;
        unsigned char *dest = flbitmap->bitmap.data();

        for (int y = 0; y < flbitmap->height; y++) {
            for (int x = 0; x < flbitmap->width; x++) {
                dest[2 * (x + y * bitmap.width) + 0] = 255;
                dest[2 * (x + y * bitmap.width) + 1] =
                    src[x + (bitmap.rows - 1 - y) * bitmap.width];
            }
        }
    }
    return flbitmap;
}
