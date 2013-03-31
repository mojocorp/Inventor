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

#include <Inventor/elements/SoViewportRegionElement.h>
#include <Inventor/elements/SoFontSizeElement.h>
#include <Inventor/elements/SoGLCacheContextElement.h>
#include <Inventor/elements/SoCacheElement.h>
#include <Inventor/elements/SoGLTextureImageElement.h>

#include <Inventor/errors/SoDebugError.h>

#include <algorithm>

//////////////////////////////////////////////////////////////////////////////
/////////////////////    SoBitmapFontCache  //////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
// Static variables for SoBitmapFontCache
std::vector<SoBitmapFontCache*> SoBitmapFontCache::fonts;

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
    : SoFontCache(state), context(-1), renderList(NULL), binpack(SbVec2s(512, 512))
//
////////////////////////////////////////////////////////////////////////
{
    data.setValue(binpack.getSize(), SbImage::Format_Luminance_Alpha, NULL);

    ref();

    // Grab all the stuff we'll need to determine our validity from
    // the state.
    const SbViewportRegion &vpr = SoViewportRegionElement::get(state);
    addElement(state->getConstElement(SoViewportRegionElement::getClassStackIndex()));

    float fontSize = SoFontSizeElement::get(state) * vpr.getPixelsPerPoint();

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
    if (renderList) {
        renderList->unref(NULL);
        renderList = NULL;
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
    if (!renderList) return isValid(state);
    else
        return (context == SoGLCacheContextElement::get(state)
                 && isValid(state));
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

    const FLbitmap & bmap = getBitmap(c);

    box.setBounds(0,0,0, bmap.width, bmap.height, 0);
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
    const FLbitmap & bmap = getBitmap(c);

    return SbVec3f(bmap.xmove, bmap.ymove, 0);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Returns the size of given string.
//
// Use: internal, public

SbVec2s
SoBitmapFontCache::getSize(const SbString &str)
//
////////////////////////////////////////////////////////////////////////
{
    SbVec2s result(0, 0);

    std::wstring chars = str.toStdWString();
    for (size_t i = 0; i < str.getLength(); i++) {
        const FLbitmap & bmap = getBitmap(chars[i]);
        result[0] += bmap.xmove;
        result[1] = std::max(result[1], (short)bmap.height);
    }
    return result;
}

void
SoBitmapFontCache::open(SoState *state, SoNode *node)
{
    SbColor blendColor(0,0,0);

    if (renderList && renderList->getContext() == context) {
        SoGLTextureImageElement::set(
                    state, node, data,
                    GL_CLAMP, GL_CLAMP,
                    GL_MODULATE, GL_NEAREST, GL_NEAREST, blendColor, renderList);
    }  // Not valid, try to build
    else {
        // Free up old list, if necessary:
        if (renderList) {
            renderList->unref(state);
            renderList = NULL;
        }
        context = SoGLCacheContextElement::get(state);
        renderList = SoGLTextureImageElement::set(
                    state, node, data,
                    GL_CLAMP, GL_CLAMP,
                    GL_MODULATE, GL_NEAREST, GL_NEAREST, blendColor, NULL);
        if (renderList)
            renderList->ref();
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
    std::wstring chars = string.toStdWString();
    for (size_t i = 0; i < string.getLength(); i++) {
        const FLbitmap & bmap = getBitmap(chars[i]);
        if (bmap.width && bmap.height) {
            int x1 = bmap.xorig;
            int y1 = bmap.yorig;
            int x2 = x1 + bmap.width;
            int y2 = y1 + bmap.height;

            glBegin(GL_QUADS);
            glTexCoord2f(bmap.uvmin[0], bmap.uvmin[1]);
            glVertex3i(x1, y1, 0);
            glTexCoord2f(bmap.uvmax[0], bmap.uvmin[1]);
            glVertex3i(x2, y1, 0);
            glTexCoord2f(bmap.uvmax[0], bmap.uvmax[1]);
            glVertex3i(x2, y2, 0);
            glTexCoord2f(bmap.uvmin[0], bmap.uvmax[1]);
            glVertex3i(x1, y2, 0);
            glEnd();
        }
        glTranslatef(bmap.xmove, bmap.ymove, 0);
    }
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Returns a bitmap.
//
// Use: private
const SoBitmapFontCache::FLbitmap &
SoBitmapFontCache::getBitmap(wchar_t c)
//
////////////////////////////////////////////////////////////////////////
{
    if (bitmaps.find(c) != bitmaps.end())
        return bitmaps[c];

    if(FT_Load_Char(face, c, FT_LOAD_RENDER)) {
#ifdef DEBUG
        SoDebugError::post("SoBitmapFontCache::getBitmap",
                           "FT_Load_Char failed");
#endif
    }

    FT_GlyphSlot glyph = face->glyph;
    FT_Bitmap bitmap = glyph->bitmap;

    bitmaps[c].width = bitmap.width;
    bitmaps[c].height = bitmap.rows;

    SbBox2s rect;
    binpack.insert(bitmaps[c].width, bitmaps[c].height, rect, false);
    int xmin = rect.getMin()[0];
    int ymin = rect.getMin()[1];

    bitmaps[c].uvmin[0] = xmin / (float)binpack.getSize()[0];
    bitmaps[c].uvmin[1] = ymin / (float)binpack.getSize()[1];
    bitmaps[c].uvmax[0] = rect.getMax()[0] / (float)binpack.getSize()[0];
    bitmaps[c].uvmax[1] = rect.getMax()[1] / (float)binpack.getSize()[1];

    bitmaps[c].xorig = glyph->bitmap_left;
    bitmaps[c].yorig = glyph->bitmap_top - glyph->bitmap.rows;
    bitmaps[c].xmove = glyph->advance.x >> 6;
    bitmaps[c].ymove = glyph->advance.y >> 6;

    if (bitmap.width && bitmap.rows) {
        unsigned char* src = face->glyph->bitmap.buffer;
        unsigned char* dest = data.getBytes();

        for(int y=0; y<bitmaps[c].height; y++) {
            for(int x = 0; x < bitmaps[c].width; x++) {
                dest[2*(x+xmin +(y+ymin)*binpack.getSize()[0])+0] = 255;
                dest[2*(x+xmin +(y+ymin)*binpack.getSize()[0])+1] = src[x+(bitmap.rows - 1 - y)*bitmap.width];
            }
        }
    }
    return bitmaps[c];
}
