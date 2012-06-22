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
SbPList *SoBitmapFontCache::fonts = NULL;
FT_Library SoBitmapFontCache::library;

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
    if (fonts == NULL) {
        // One-time font library initialization
        fonts = new SbPList;

        if (FT_Init_FreeType( &library )) {
#ifdef DEBUG
            SoDebugError::post("SoBitmapFontCache::getFont",
                               "FT_Init_FreeType failed");
#endif
            return NULL;
        }
    }

    SoBitmapFontCache *result = NULL;
    for (int i = 0; i < fonts->getLength() && result == NULL; i++) {
        SoBitmapFontCache *fc = (SoBitmapFontCache *)(*fonts)[i];
        if (forRender ? fc->isRenderValid(state) : fc->isValid(state)) {
            result = fc;
            result->ref();
        }
    }
    if (result == NULL) {
        result = new SoBitmapFontCache(state);

        // If error:
        if (result->fontId == 0) {
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

SoBitmapFontCache::SoBitmapFontCache(SoState *state) : SoCache(state)
//
////////////////////////////////////////////////////////////////////////
{
    ref();

    list = NULL;

    // Grab all the stuff we'll need to determine our validity from
    // the state.
    const SbViewportRegion &vpr = SoViewportRegionElement::get(state);
    addElement(state->getConstElement(SoViewportRegionElement::getClassStackIndex()));

    float fontSize = SoFontSizeElement::get(state) * vpr.getPixelsPerPoint();
    addElement(state->getConstElement(SoFontSizeElement::getClassStackIndex()));

    const SbName & fontName = SoFontNameElement::get(state);

    addElement(state->getConstElement(SoFontNameElement::getClassStackIndex()));
    if (fontName == SoFontNameElement::getDefault()) {
        if (FT_New_Memory_Face(library, binary_utopia_regular, BINARY_UTOPIA_REGULAR_SIZE, 0, &fontId)) {
#ifdef DEBUG
            SoDebugError::post("SoBitmapFontCache::getFont",
                               "Couldn't load font Utopia-Regular!");
#endif
            numChars = 0;
        }
    } else {

        // Initialize everything
        if (FT_New_Face( library, SoFont::getFontFileName(fontName.getString()).getString(), 0, &fontId )) {
#ifdef DEBUG
            SoDebugError::post("SoBitmapFontCache::getFont",
                               "Couldn't find font %s, replacing with Utopia-Regular", fontName.getString());
#endif
            if (FT_New_Memory_Face(library, binary_utopia_regular, BINARY_UTOPIA_REGULAR_SIZE, 0, &fontId)) {
#ifdef DEBUG
                SoDebugError::post("SoBitmapFontCache::getFont",
                                   "Couldn't load font Utopia-Regular!");
#endif
                numChars = 0;
            }
        }
    }

    FT_Set_Char_Size( fontId, fontSize * 64, fontSize * 64, 96, 96);

    numChars = 256;  // ??? JUST DO ASCII FOR NOW!
    listFlags.resize(numChars);
    bitmaps.resize(numChars);
    for (int i = 0; i < numChars; i++) {
        listFlags[i] = FALSE;
        bitmaps[i] = NULL;
    }

    fonts->append(this);
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
    if (fontId) {
        for (int i = 0; i < numChars; i++) {
            if (bitmaps[i] != NULL) {
                delete [] bitmaps[i]->bitmap;
                delete bitmaps[i];
                bitmaps[i] = NULL;
            }
        }
        // Only destroy the font library font if no other font caches
        // are using the same font identifier:
        SbBool otherUsing = FALSE;
        for (int i = 0; i < fonts->getLength(); i++) {
            SoBitmapFontCache *t = (SoBitmapFontCache *)(*fonts)[i];
            if (t != this && t->fontId == fontId) otherUsing = TRUE;
        }
        if (!otherUsing) {
            FT_Done_Face(fontId);
            fontId = NULL;
        }

        listFlags.clear();
        bitmaps.clear();

        fonts->remove(fonts->find(this));
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
    if (list) {
        list->unref(NULL);
        list = NULL;
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
    if (!list) return isValid(state);
    else
        return (list->getContext() == SoGLCacheContextElement::get(state)
                 && isValid(state));
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Sets up for GL rendering.
//
// Use: internal

void
SoBitmapFontCache::setupToRender(SoState *state)
//
////////////////////////////////////////////////////////////////////////
{

    otherOpen = SoCacheElement::anyOpen(state);

    if (!otherOpen && !list) {
        list = new SoGLDisplayList(state,
                                   SoGLDisplayList::DISPLAY_LIST,
                                   numChars);
        list->ref();
    }
    if (list) {
        // Set correct list base
        glListBase(list->getFirstIndex());
        list->addDependency(state);
    }
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Returns TRUE if a display lists exists for given character.
//    Tries to build a display list, if it can.
//
// Use: internal

SbBool
SoBitmapFontCache::hasDisplayList(const char c)
//
////////////////////////////////////////////////////////////////////////
{
    // If we have one, return TRUE
    if (listFlags[c] == TRUE)
        return TRUE;

    // If we don't and we can't build one, return FALSE.
    if (otherOpen)
        return FALSE;

    // Build one:
    glNewList(list->getFirstIndex()+c, GL_COMPILE);
    drawCharacter(c);
    glEndList();
    listFlags[c] = TRUE;

    return TRUE;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Assuming that there are display lists built for all the
//    characters in given string, render them using the GL's CallLists
//    routine.
//
// Use: internal

void
SoBitmapFontCache::callLists(const SbString &string)
//
////////////////////////////////////////////////////////////////////////
{
    for (size_t i=0; i<string.getLength(); i++)
    {
        if (!listFlags[string.getString()[i]])
            printf("no display list for %c\n", string.getString()[i]);

    }
    const char *str = string.getString();
    glCallLists(string.getLength(), GL_UNSIGNED_BYTE, str);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Returns the pixel-space bounding box of given character.
//
// Use: internal, public

void
SoBitmapFontCache::getCharBbox(char c, SbBox3f &box)
//
////////////////////////////////////////////////////////////////////////
{
    box.makeEmpty();

    const FLbitmap *bmap = getBitmap(c);
    if (bmap == NULL)
        return;

    box.extendBy(SbVec3f(-bmap->xorig, -bmap->yorig, 0));
    box.extendBy(SbVec3f(bmap->width - bmap->xorig,
                         bmap->height - bmap->yorig, 0));
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Returns the amount the current raster position will be advanced
//    after drawing the given character.
//
// Use: internal, public

SbVec3f
SoBitmapFontCache::getCharOffset(char c)
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

    const char *chars = str.getString();
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
SoBitmapFontCache::drawCharacter(char c)
//
////////////////////////////////////////////////////////////////////////
{
    const FLbitmap *bmap = getBitmap(c);

    if (bmap != NULL) {
        glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 2);

        glBitmap(0, 0, 0,0,bmap->xorig, bmap->yorig, NULL);

        glDrawPixels(bmap->width, bmap->height, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, bmap->bitmap);

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
SoBitmapFontCache::drawString(const SbString &string)
//
////////////////////////////////////////////////////////////////////////
{
    SbBool useCallLists = TRUE;

    const char *chars = string.getString();

    // If there aren't any other caches open, build display lists for
    // the characters we can:
    for (size_t i = 0; i < string.getLength(); i++) {
        // See if the font cache already has (or can build) a display
        // list for this character:
        if (!hasDisplayList(chars[i])) {
            useCallLists = FALSE;
            break;
        }
    }

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
    glDisable(GL_TEXTURE_2D);

    // if we have display lists for all of the characters, use
    // glCallLists:
    if (useCallLists) {
        callLists(string);
    } else {
        // if we don't, draw the string character-by-character, using the
        // display lists we do have:
        for (size_t i = 0; i < string.getLength(); i++) {
            if (!hasDisplayList(chars[i])) {
                drawCharacter(chars[i]);
            }
            else glCallList(list->getFirstIndex()+chars[i]);
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
SoBitmapFontCache::getBitmap(unsigned char c)
//
////////////////////////////////////////////////////////////////////////
{
    if (bitmaps[c])
        return bitmaps[c];

    bitmaps[c] = new FLbitmap;
    bitmaps[c]->bitmap = NULL;

    if(FT_Load_Char(fontId, c, FT_LOAD_RENDER)) {
#ifdef DEBUG
        SoDebugError::post("SoBitmapFontCache::getBitmap",
                           "FT_Load_Char failed");
#endif
    }

    FT_GlyphSlot glyph = fontId->glyph;
    FT_Bitmap bitmap = glyph->bitmap;

    bitmaps[c]->width = bitmap.width;
    bitmaps[c]->height = bitmap.rows;

    bitmaps[c]->xorig = glyph->bitmap_left;
    bitmaps[c]->yorig = glyph->bitmap_top - glyph->bitmap.rows;
    bitmaps[c]->xmove = glyph->advance.x / 64.0f;
    bitmaps[c]->ymove = glyph->advance.y / 64.0f;

    //Allocate memory for the texture data.
    bitmaps[c]->bitmap = new GLubyte[ 2 * bitmaps[c]->width * bitmaps[c]->height];

    unsigned char* dest = bitmaps[c]->bitmap + (( bitmaps[c]->height - 1) * bitmaps[c]->width * 2);
    unsigned char* src = bitmap.buffer;
    size_t destStep = bitmaps[c]->width * 2 * 2;

    for( unsigned int y = 0; y < bitmaps[c]->height; ++y)
    {
        for( unsigned int x = 0; x < bitmaps[c]->width; ++x)
        {
            *dest++ = 255;//*src;
            *dest++ = *src++;
        }
        dest -= destStep;
    }

    return bitmaps[c];
}
