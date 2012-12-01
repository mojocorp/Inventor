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
 |      SoText2
 |
 |   Author(s)          : Gavin Bell
 |
 ______________  S I L I C O N   G R A P H I C S   I N C .  ____________
 _______________________________________________________________________
 */

#include <Inventor/misc/SoGL.h>
#include <Inventor/nodes/SoFont.h>
#include <Inventor/caches/SoBitmapFontCache.h>
#include <Inventor/caches/SoGLDisplayList.h>

#include <Inventor/elements/SoFontNameElement.h>
#include <Inventor/elements/SoViewportRegionElement.h>
#include <Inventor/elements/SoFontSizeElement.h>
#include <Inventor/elements/SoGLCacheContextElement.h>
#include <Inventor/elements/SoCacheElement.h>

#include <Inventor/errors/SoDebugError.h>

#include "utopia-regular.cpp"

//////////////////////////////////////////////////////////////////////////////
/////////////////////    SoBitmapFontCache  //////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
// Static variables for SoBitmapFontCache
std::vector<SoBitmapFontCache*> SoBitmapFontCache::fonts;
FT_Library SoBitmapFontCache::library = NULL;

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
    if (library == NULL) {
        // One-time font library initialization
        if (FT_Init_FreeType( &library )) {
#ifdef DEBUG
            SoDebugError::post("SoBitmapFontCache::getFont",
                               "FT_Init_FreeType failed");
#endif
            return NULL;
        }
    }

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
    : SoCache(state), context(-1)
//
////////////////////////////////////////////////////////////////////////
{
    ref();

    // Grab all the stuff we'll need to determine our validity from
    // the state.
    const SbViewportRegion &vpr = SoViewportRegionElement::get(state);
    addElement(state->getConstElement(SoViewportRegionElement::getClassStackIndex()));

    float fontSize = SoFontSizeElement::get(state) * vpr.getPixelsPerPoint();
    addElement(state->getConstElement(SoFontSizeElement::getClassStackIndex()));

    const SbName & fontName = SoFontNameElement::get(state);

    addElement(state->getConstElement(SoFontNameElement::getClassStackIndex()));
    if (fontName == SoFontNameElement::getDefault() || fontName == "Utopia-Regular") {
        if (FT_New_Memory_Face(library, binary_utopia_regular, BINARY_UTOPIA_REGULAR_SIZE, 0, &face)) {
#ifdef DEBUG
            SoDebugError::post("SoBitmapFontCache::getFont",
                               "Couldn't load embeded font Utopia-Regular!");
#endif
        }
    } else {
        SbFile file;
        if (file.open(SoFont::getFontFileName(fontName), "rb")) {
            buffer.resize(SbFile::size(SoFont::getFontFileName(fontName)));
            file.read(&buffer[0], 1, buffer.size());

            if (FT_New_Memory_Face(library, &buffer[0], (FT_Long)buffer.size(), 0, &face)) {
#ifdef DEBUG
                SoDebugError::post("SoBitmapFontCache::getFont",
                                   "Couldn't find font %s, replacing with Utopia-Regular", fontName.getString());
#endif
            }
        }
    }

    if (!face && FT_New_Memory_Face(library, binary_utopia_regular, BINARY_UTOPIA_REGULAR_SIZE, 0, &face)) {
#ifdef DEBUG
        SoDebugError::post("SoText2::getFont",
                           "Couldn't find font Utopia-Regular!");
#endif
    }

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
        std::map<wchar_t, FLbitmap*>::iterator it;
        for (it = bitmaps.begin(); it != bitmaps.end(); it++) {
            delete [] it->second->bitmap;
            delete it->second;
        }
        // Only destroy the font library font if no other font caches
        // are using the same font identifier:
        SbBool otherUsing = FALSE;
        for (size_t i = 0; i < fonts.size(); i++) {
            SoBitmapFontCache *t = fonts[i];
            if (t != this && t->face == face) otherUsing = TRUE;
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
    std::map<wchar_t, SoGLDisplayList*>::iterator it;
    for (it=list.begin(); it != list.end(); it++) {
        it->second->unref(NULL);
    }

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
    if (list.empty()) return isValid(state);
    else
        return (context == SoGLCacheContextElement::get(state)
                 && isValid(state));
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Returns TRUE if a display lists exists for given character.
//    Tries to build a display list, if it can.
//
// Use: internal

SbBool
SoBitmapFontCache::hasDisplayList(SoState *state, wchar_t c)
//
////////////////////////////////////////////////////////////////////////
{
    // If we have one, return TRUE
    if (list[c])
        return TRUE;

    // If we don't and we can't build one, return FALSE.
    if (SoCacheElement::anyOpen(state))
        return FALSE;

    context = SoGLCacheContextElement::get(state);

    // Build one:
    list[c] = new SoGLDisplayList(state, SoGLDisplayList::DISPLAY_LIST);
    list[c]->ref();
    list[c]->addDependency(state);
    glNewList(list[c]->getFirstIndex(), GL_COMPILE);
    drawCharacter(c);
    list[c]->close(state);

    return TRUE;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Returns the pixel-space bounding box of given character.
//
// Use: internal, public

void
SoBitmapFontCache::getCharBbox(wchar_t c, SbBox3f &box)
//
////////////////////////////////////////////////////////////////////////
{
    box.makeEmpty();

    const FLbitmap *bmap = getBitmap(c);
    if (bmap == NULL)
        return;

    box.setBounds(0,0,0, bmap->width, bmap->height, 0);
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
    else
        return SbVec3f(0,0,0);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Returns the width of given string.
//
// Use: internal, public

float
SoBitmapFontCache::getWidth(const SbString &str)
//
////////////////////////////////////////////////////////////////////////
{
    float result = 0.0;

    std::wstring chars = str.toStdWString();
    for (size_t i = 0; i < str.getLength(); i++) {
        const FLbitmap *bmap = getBitmap(chars[i]);
        if (bmap != NULL)
            result += bmap->xmove;
    }
    return result;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Returns the height of given string.
//
// Use: internal, public

float
SoBitmapFontCache::getHeight()
//
////////////////////////////////////////////////////////////////////////
{
    const FLbitmap *bmap = getBitmap('M');
    if (bmap != NULL)
        return bmap->height - bmap->yorig;
    else return 0;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Draws a bitmap
//
// Use: internal public

void
SoBitmapFontCache::drawCharacter(wchar_t c)
//
////////////////////////////////////////////////////////////////////////
{
    const FLbitmap *bmap = getBitmap(c);

    if (bmap != NULL) {
        glBitmap(0, 0, 0, 0,bmap->xorig, bmap->yorig, NULL);
        if (bmap->bitmap) {
            glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
            glPixelStorei(GL_UNPACK_ALIGNMENT, 2);
            glDrawPixels(bmap->width, bmap->height, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, bmap->bitmap);
        }
        glBitmap(0, 0, 0.0f, 0.0f, bmap->xmove-bmap->xorig, bmap->ymove-bmap->yorig, NULL);
    }
}
////////////////////////////////////////////////////////////////////////
//
// Description:
//    Draws a whole string.  Tries to build display lists, if it can.
//
// Use: internal public

void
SoBitmapFontCache::drawString(SoState *state, const SbString &string)
//
////////////////////////////////////////////////////////////////////////
{
    //
    // Set up OpenGL state for rendering text, and push
    // attributes so that we can restore when done.
    //
    glPushAttrib( GL_ENABLE_BIT | GL_PIXEL_MODE_BIT | GL_COLOR_BUFFER_BIT);
    glPushClientAttrib( GL_CLIENT_PIXEL_STORE_BIT);

    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.3f);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // if we don't, draw the string character-by-character, using the
    // display lists we do have:
    std::wstring chars = string.toStdWString();
    for (size_t i = 0; i < string.getLength(); i++) {
        if (hasDisplayList(state, chars[i])) {
            list[chars[i]]->call(state);
        }
        else {
            drawCharacter(chars[i]);
        }
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
    if (bitmaps[c])
        return bitmaps[c];

    bitmaps[c] = new FLbitmap;
    bitmaps[c]->bitmap = NULL;

    if(FT_Load_Char(face, c, FT_LOAD_RENDER)) {
#ifdef DEBUG
        SoDebugError::post("SoBitmapFontCache::getBitmap",
                           "FT_Load_Char failed");
#endif
    }

    FT_GlyphSlot glyph = face->glyph;
    FT_Bitmap bitmap = glyph->bitmap;

    bitmaps[c]->width = bitmap.width;
    bitmaps[c]->height = bitmap.rows;

    bitmaps[c]->xorig = glyph->bitmap_left;
    bitmaps[c]->yorig = glyph->bitmap_top - glyph->bitmap.rows;
    bitmaps[c]->xmove = glyph->advance.x / 64.0f;
    bitmaps[c]->ymove = glyph->advance.y / 64.0f;

    if (bitmap.width && bitmap.rows) {
        //Allocate memory for the texture data.
        bitmaps[c]->bitmap = new unsigned char[2 * bitmaps[c]->width * bitmaps[c]->height];

        unsigned char* src = face->glyph->bitmap.buffer;
        unsigned char* dest = bitmaps[c]->bitmap;

        for(int y=0; y<bitmaps[c]->height; y++)
        {
            for(int x = 0; x < bitmaps[c]->width; x++)
            {
                dest[2*(x+y*bitmap.width)+0] = 255;
                dest[2*(x+y*bitmap.width)+1] = src[x+(bitmap.rows - 1 - y)*bitmap.width];
            }
        }
    }
    return bitmaps[c];
}
