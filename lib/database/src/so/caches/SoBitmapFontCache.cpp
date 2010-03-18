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

#include <Inventor/caches/SoBitmapFontCache.h>

#include <Inventor/elements/SoFontNameElement.h>
#include <Inventor/elements/SoViewportRegionElement.h>
#include <Inventor/elements/SoFontSizeElement.h>
#include <Inventor/elements/SoGLCacheContextElement.h>
#include <Inventor/elements/SoGLDisplayList.h>
#include <Inventor/elements/SoCacheElement.h>

#include <Inventor/errors/SoDebugError.h>

//////////////////////////////////////////////////////////////////////////////
/////////////////////    SoBitmapFontCache  //////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
// Static variables for SoBitmapFontCache
SbPList *SoBitmapFontCache::fonts = NULL;
FLcontext SoBitmapFontCache::flContext;

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
        flContext = flCreateContext(NULL, FL_FONTNAME, NULL,
                                  1.0, 1.0);
        if (flContext == NULL) {
#ifdef DEBUG
            SoDebugError::post("SoText2::getFont",
                               "flCreateContext returned NULL");
#endif
            return NULL;
        }
        if (flGetCurrentContext() != flContext)
            flMakeCurrentContext(flContext);
        flSetHint(FL_HINT_MINOUTLINESIZE, 24.0);
    }
    else if (flContext == NULL) return NULL;
    else {
        if (flGetCurrentContext() != flContext)
            flMakeCurrentContext(flContext);
    }

    SoBitmapFontCache *result = NULL;
    for (int i = 0; i < fonts->getLength() && result == NULL; i++) {
        SoBitmapFontCache *fc = (SoBitmapFontCache *)(*fonts)[i];
        //ML if (!fc->fontNumList) continue;
        if (forRender ? fc->isRenderValid(state) : fc->isValid(state)) {
            result = fc;
            result->ref();
            if (flGetCurrentFont() != result->fontId) {
                flMakeCurrentFont(result->fontId);
            }
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
//    Sees if this font is valid.  If it is valid, it also makes it
//    current.
//
// Use: public

SbBool
SoBitmapFontCache::isValid(SoState *state) const
//
////////////////////////////////////////////////////////////////////////
{
    SbBool result = SoCache::isValid(state);

    if (result) {
        if (flGetCurrentContext() != flContext) {
            flMakeCurrentContext(flContext);
            flMakeCurrentFont(fontId);
        }
        else if (flGetCurrentFont() != fontId)
            flMakeCurrentFont(fontId);
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
    SbName fontName = SoFontNameElement::get(state);
    addElement(state->getConstElement(
        SoFontNameElement::getClassStackIndex()));
    if (fontName == SoFontNameElement::getDefault()) {
        fontName = SbName("Utopia-Regular");
    }
    const SbViewportRegion &vpr = SoViewportRegionElement::get(state);
    addElement(state->getConstElement(
        SoViewportRegionElement::getClassStackIndex()));
    float fontSize = SoFontSizeElement::get(state) * vpr.getPixelsPerPoint();
    addElement(state->getConstElement(
        SoFontSizeElement::getClassStackIndex()));

    // Initialize everything
    GLfloat m[2][2];
    m[0][0] = m[1][1] = fontSize;
    m[0][1] = m[1][0] = 0.0;
    fontId = flCreateFont((const GLubyte *)fontName.getString(), m, 0, NULL);

    if (fontId == 0) {
        // Try Utopia-Regular, unless we just did!
        if (fontName != SbName("Utopia-Regular")) {
#ifdef DEBUG
            SoDebugError::post("SoText2::getFont",
                      "Couldn't find font %s, replacing with Utopia-Regular",
                       fontName.getString());
#endif
            fontId = flCreateFont((const GLubyte *)"Utopia-Regular",
                                  m, 0, NULL);
        }
        if (fontId == 0) {
#ifdef DEBUG
            SoDebugError::post("SoText3::getFont",
                               "Couldn't find font Utopia-Regular!");
#endif
            numChars = 0;
        }
    }
    flMakeCurrentFont(fontId);

    numChars = 256;  // ??? JUST DO ASCII FOR NOW!
    listFlags = new SbBool[numChars];
    bitmaps = new FLbitmap*[numChars];
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
        if (flGetCurrentContext() != flContext) {
            flMakeCurrentContext(flContext);
            flMakeCurrentFont(fontId);
        }
        else if (flGetCurrentFont() != fontId)
            flMakeCurrentFont(fontId);

        int i;
        for (i = 0; i < numChars; i++) {
            if (bitmaps[i] != NULL) flFreeBitmap(bitmaps[i]);
        }

        // Only destroy the font library font if no other font caches
        // are using the same font identifier:
        SbBool otherUsing = FALSE;
        for (i = 0; i < fonts->getLength(); i++) {
            SoBitmapFontCache *t = (SoBitmapFontCache *)(*fonts)[i];
            if (t != this && t->fontId == fontId) otherUsing = TRUE;
        }
        if (!otherUsing) {
            flDestroyFont(fontId);
        }

        delete [] listFlags;
        delete [] bitmaps;

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
    if (listFlags[c] == TRUE) return TRUE;

    // If we don't and we can't build one, return FALSE.
    if (otherOpen) return FALSE;

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
    printf("call list: %s\n", str);
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
    if (bmap == NULL) return;

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
    else return SbVec3f(0,0,0);
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
        glBitmap(bmap->width, bmap->height, bmap->xorig, bmap->yorig,
             bmap->xmove, bmap->ymove, bmap->bitmap);
        printf("draw char %c - w: %d h: %d ox: %f oy: %f xmove %f ymove %f\n", c, bmap->width, bmap->height, bmap->xorig, bmap->yorig, bmap->xmove, bmap->ymove);
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
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Returns a bitmap.
//
// Use: private

const FLbitmap *
SoBitmapFontCache::getBitmap(unsigned char c)
//
////////////////////////////////////////////////////////////////////////
{
//ML    if (!fontNumList) return NULL;

    if (bitmaps[c] == NULL) {
        bitmaps[c] = flGetBitmap(fontId, c);
    }
    return bitmaps[c];
}
