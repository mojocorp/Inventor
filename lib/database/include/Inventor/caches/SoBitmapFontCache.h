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

#include <Inventor/SbBox.h>
#include <Inventor/caches/SoCache.h>
#include <Inventor/fields/SoMFString.h>

#include <flclient.h>
#include <iconv.h>

class SoGLDisplayList;


// An internal class that makes life easier:

// This very specialized cache class is used to cache bitmaps and GL
// display lists containing bitmaps.  It is strange because it doesn't
// use the normal list of elements used to determine validity, etc,
// and knows exactly which elements it depends on.

class SoBitmapFontCache : public SoCache
{
  public:
    // Return a font (either a new one or an old one) that is valid
    // for the given state.
    static SoBitmapFontCache *	getFont(SoState *state, SbBool forRender);

    // Checks to see if this font is valid
    SbBool		isValid(const SoState *state) const;

    // Use this when rendering to decide if this cache is valid (it
    // checks the GL cache context in addition to other elements)
    SbBool		isRenderValid(SoState *state) const;

    // Set up for GL rendering:
    void	setupToRender(SoState *state);

    // Returns the amount the current raster position will be advanced
    // after drawing the given unicode character.
    SbVec3f		getCharOffset(char* c);

    // Get the pixel-space bounding box of a given unicode character.
    void		getCharBbox(char* c, SbBox3f &box);

    // Gets the width (in pixels) of specified UCS-2 string
    float		getWidth(int line);

    // Gets the height of the font, in pixels
    float		getHeight();

    // Draws the current UCS-2 string
    void		drawString(int line);

    // Draws the given unicode character (using GL)
    void		drawCharacter(const char* c);

    //Convert string to UCS-2 format, keep a copy in this cache.
    //Use nodeid to know when to reconvert.
    SbBool	convertToUCS(uint32_t nodeid, const SoMFString& string);

    //Returns line of UCS-2 text
    char *	getUCSString(int line)
    { return (char*)UCSStrings[line];}

    int		getNumUCSChars(int line)
    { return (int)(long)UCSNumChars[line];}

  protected:
    // Free up display lists before being deleted
    virtual void	destroy(SoState *state);

  private:
    // Constructor.
    SoBitmapFontCache(SoState *state);

    // Destructor
    virtual ~SoBitmapFontCache();

    // Returns TRUE if this font cache has a display list for the
    // given unicode UCS-2 character.  It will try to build a display list, if it
    // can.
    SbBool	hasDisplayList(const char* c);

    // Renders an entire UCS-2 string by using the GL callList() function.
    void	callLists(const char *string, int length);

    const FLbitmap *getBitmap(unsigned char* c);

    // Static list of all fonts.  OPTIMIZATION:  If there turn out to
    // be applications that use lots of fonts, we could change this
    // list into a dictionary keyed off the font name.
    static SbPList	*fonts;

    int		numChars;  // Number of characters in this font



    SoGLDisplayList *list;

    // Dictionary to point to unicode-character display lists;
    // Keyed by unicode value
    SbDict	*displayListDict;

    // Dictionary to point to bitmap; keyed by unicode value.
    SbDict	*bitmapDict;

    // This flag will be true if there is another cache open (if
    // building GL display lists for render caching, that means we
    // can't also build display lists).
    SbBool	otherOpen;

    // This indicates the nodeId of the last created cache, so we can
    // know when a new UCS-2 translation is required
    uint32_t	currentNodeId;

    // And some font library stuff:
    static FLcontext	flContext;

//  instead of font number, we use a comma-separated list of font numbers.
    GLubyte* createUniFontList(const char* fontNameList, float fontsize);
    GLubyte* fontNumList;
    SbPList* fontNums;

    // char* pointers of UCS-2 strings:
    SbPList	UCSStrings;
    // size of these strings, in UCS-2 characters:
    SbPList	UCSNumChars;

    static iconv_t conversionCode;  // Used for UCS conversion
    //Routine to release storage of an FLBitmap
    static void freeBitmap(unsigned long, void* value);

};
#endif /* _SO_BITMAP_FONT_CACHE_ */
