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
#include <machine.h>
#include <Inventor/SbViewportRegion.h>
#include <Inventor/caches/SoBitmapFontCache.h>
#include <Inventor/elements/SoCacheElement.h>
#include <Inventor/elements/SoGLCacheContextElement.h>
#include <Inventor/elements/SoFontNameElement.h>
#include <Inventor/elements/SoViewportRegionElement.h>
#include <Inventor/elements/SoFontSizeElement.h>

//////////////////////////////////////////////////////////////////////////////
/////////////////////    SoBitmapFontCache  //////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
// Static variables for SoBitmapFontCache
SbPList *SoBitmapFontCache::fonts = NULL;
FLcontext SoBitmapFontCache::flContext;

//The conversion code (from UTF-8 to UCS-2) is static, set only once.
iconv_t SoBitmapFontCache::conversionCode = NULL;


////////////////////////////////////////////////////////////////////////
//
// Description:
//    Convert MFString to UCS string, if necessary.
//
// Use: internal

SbBool
SoBitmapFontCache::convertToUCS(uint32_t nodeid,
                   const SoMFString& strings)
//
////////////////////////////////////////////////////////////////////////
{
    if (nodeid == currentNodeId) return TRUE;
    currentNodeId = nodeid;

    //delete previously converted UCS string
    int i;
    for (i = 0; i< UCSStrings.getLength(); i++){
    delete [] (char*)UCSStrings[i];
    }
    UCSStrings.truncate(0);
    UCSNumChars.truncate(0);

    //make sure conversion code already set:
    if (conversionCode == NULL){
    conversionCode = iconv_open("UCS-2", "UTF-8");
    }

    if ( conversionCode == (iconv_t)-1 ){
#ifdef DEBUG
    SoDebugError::post("SoBitmapFontCache::convertToUCS",
        "Invalid UTF-8 to UCS-2 conversion");
#endif /*DEBUG*/
    return FALSE;
    }

    //for each line of text, allocate a sufficiently large buffer:
    //An extra two bytes allocated, since glCallLists likes to trample it.
    for (i = 0; i< strings.getNum(); i++){
    UCSStrings[i] = new char[2*strings[i].getLength()+2];

    char* input = (char *)strings[i].getString();
    size_t inbytes = strings[i].getLength();
    size_t outbytes = 2*inbytes+2;
    char* output = (char*)UCSStrings[i];

    if ((iconv(conversionCode, &input, &inbytes, &output, &outbytes) == (size_t)-1)){
#ifdef DEBUG
        SoDebugError::post("SoBitmapFontCache::convertToUCS",
        "Error converting text to UCS-2");
#endif /*DEBUG*/

    }

    if (inbytes){
#ifdef DEBUG
        SoDebugError::post("SoBitmapFontCache::convertToUCS",
        "Incomplete conversion to UCS-2");
#endif /*DEBUG*/
        return FALSE;
    }

    UCSNumChars[i] = (void*)((2*strings[i].getLength()+2 - outbytes)>>1);

        int j;
        for (j = 0; j < getNumUCSChars(i); j++) {
            char* c = (char*)UCSStrings[i]+j*2;
            DGL_HTON_SHORT(SHORT(c), SHORT(c));
        }
    }

    return TRUE;

}

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
    if (!fc->fontNumList) continue;
    if (forRender ? fc->isRenderValid(state) : fc->isValid(state)) {
        result = fc;
        result->ref();

    }
    }
    if (result == NULL) {
    result = new SoBitmapFontCache(state);

    }
    return result;
}
////////////////////////////////////////////////////////////////////////
//
// Description:
//    Create a list of font numbers from a list of font names
//
// Use: private
GLubyte *
SoBitmapFontCache::createUniFontList(const char* fontNameList, float size)
//
////////////////////////////////////////////////////////////////////////
{
    char *s, *s1, *ends;
    FLfontNumber fn;
    float mat[2][2];

    mat[0][0] = mat[1][1] = size;
    mat[0][1] = mat[1][0] = 0.0;

    //Make a copy of fontNameList so we don't disturb the one we are passed.
    //Find \n at end of namelist:
    char * nameCopy = new char[strlen(fontNameList)+1];
    strcpy(nameCopy, fontNameList);

    //find the last null in nameCopy.
    s = ends = (char *)strrchr(nameCopy, '\0');
    *s = ';';  /* put a guard in the end of string */


    s = (char*)nameCopy;
    fontNums = new SbPList;

    while ((s1 = (char *)strchr(s, ';'))) {
       *s1 = (char)NULL;  /* font name is pointed to s */

       if ((fn = flCreateFont((const GLubyte*)s, mat, 0, NULL)) == (FLfontNumber)0) {
#ifdef DEBUG
        SoDebugError::post("SoBitmapFontCache::createUniFontList",
        "Cannot create font %s", s);
#endif
       }
       else {
     fontNums->append((void*)(unsigned long)fn);
       }
       if(s1 == ends) break;
       s = (s1 + 1);  /* move to next font name */
    }

    if (fontNums->getLength() == 0 ) return NULL;

    // create a comma-separated list of font numbers:
    char *fontList = new char[10*fontNums->getLength()];
    fontList[0] = '\0';
    for (int i = 0; i< fontNums->getLength(); i++ ){
    fn = (FLfontNumber)(long)(*fontNums)[i];
    sprintf(&fontList[strlen(fontList)], "%d,", fn);
    }
    fontList[strlen(fontList) - 1] = '\0'; // the last ',' is replaced with NULL

    delete [] nameCopy;

    return (GLubyte *)fontList;

}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Sees if this font is valid.  If it is valid, it also makes it
//    current.
//
// Use: public

SbBool
SoBitmapFontCache::isValid(const SoState *state) const
//
////////////////////////////////////////////////////////////////////////
{
    SbBool result = SoCache::isValid(state);

    if (result) {
    if (flGetCurrentContext() != flContext) {
        flMakeCurrentContext(flContext);
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

    // Initialize font list
    fontNumList = createUniFontList(fontName.getString(), fontSize);


    if (fontNumList == NULL) {
    // Try Utopia-Regular, unless we just did!
    if (fontName != SbName("Utopia-Regular")) {
#ifdef DEBUG
        SoDebugError::post("SoText2::getFont",
              "Couldn't find font %s, replacing with Utopia-Regular",
               fontName.getString());
#endif
        fontNumList = createUniFontList("Utopia-Regular", fontSize);

    }
    if (fontNumList == NULL) {
#ifdef DEBUG
        SoDebugError::post("SoText3::getFont",
                   "Couldn't find font Utopia-Regular!");
#endif
        numChars = 0;
    }
    }


    numChars = 65536;  // ??? Just do UCS-2
    currentNodeId = 0;
    displayListDict = new SbDict;
    bitmapDict = new SbDict;

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
    if (fontNumList) {
    if (flGetCurrentContext() != flContext) {
        flMakeCurrentContext(flContext);
    }

    //Must free every bitmap in dictionary:
    //this will just apply flFreeBitmap(entry->value)

    bitmapDict->applyToAll(freeBitmap);

    // Only destroy the font library font if no other font caches
    // are using the same font identifier:
    // Must go through fontlist and destroy every font that isn't used
    // by any other cache.

    SbBool otherFonts = (fonts->getLength() > 1);
    SbDict *otherFontDict;
    if (otherFonts){
        otherFontDict = new SbDict;
        //Enter all the other fontnums into the dictionary:
        for (int i = 0; i< fonts->getLength(); i++) {
        SoBitmapFontCache *t = (SoBitmapFontCache *)(*fonts)[i];
        if ( t == this) continue;
        for (int j = 0; j< (t->fontNums->getLength()); j++){
            unsigned long key = (unsigned long)(*(t->fontNums))[j];
            otherFontDict->enter(key, NULL);
        }
        }
    }
    // Now destroy any fonts that don't appear in otherFontDict
    for (int i = 0; i < fontNums->getLength(); i++){
        void *value;
        if ( !otherFonts ||
            !otherFontDict->find((unsigned long)(*fontNums)[i], value)){
        flDestroyFont((FLfontNumber)(long)(*fontNums)[i]);
        }
    }
    if (otherFonts) delete otherFontDict;
    delete displayListDict;
    delete bitmapDict;

    if (fontNumList)	delete [] fontNumList;
    if (fontNums)		delete fontNums;

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
SoBitmapFontCache::hasDisplayList(const char* c)
//
////////////////////////////////////////////////////////////////////////
{
    unsigned char *uc = (unsigned char*)c;
    unsigned long key = (uc[0]<<8)|uc[1];
    // If we have one, return TRUE
    void *value = NULL;
    if (displayListDict->find(key, value)) return TRUE;

    // If we don't and we can't build one, return FALSE.
    if (otherOpen) return FALSE;

    // Build one:
    glNewList(list->getFirstIndex()+key, GL_COMPILE);
    drawCharacter(c);
    glEndList();

    displayListDict->enter(key, value);

    return TRUE;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Assuming that there are display lists built for all the
//    UCS-2 characters in given string, render them.
//    string should have already been converted from UTF8 form.
//
// Use: internal

void
SoBitmapFontCache::callLists(const char *string, int len)
//
////////////////////////////////////////////////////////////////////////
{

    glCallLists(len, GL_2_BYTES, (unsigned char*)string);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Returns the pixel-space bounding box of given UCS-2 character.
//
// Use: internal, public

void
SoBitmapFontCache::getCharBbox(char* c, SbBox3f &box)
//
////////////////////////////////////////////////////////////////////////
{
    box.makeEmpty();

    const FLbitmap *bmap = getBitmap((unsigned char*)c);
    if (bmap == NULL) return;

    box.extendBy(SbVec3f(-bmap->xorig, -bmap->yorig, 0));
    box.extendBy(SbVec3f(bmap->width - bmap->xorig,
             bmap->height - bmap->yorig, 0));
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Returns the amount the current raster position will be advanced
//    after drawing the given UCS-2 character.
//
// Use: internal, public

SbVec3f
SoBitmapFontCache::getCharOffset(char* c)
//
////////////////////////////////////////////////////////////////////////
{
    const FLbitmap *bmap = getBitmap((unsigned char*)c);
    if (bmap != NULL)
    return SbVec3f(bmap->xmove, bmap->ymove, 0);
    else return SbVec3f(0,0,0);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Returns the width of specified UCS2 string.
//
// Use: internal, public

float
SoBitmapFontCache::getWidth(int line)
//
////////////////////////////////////////////////////////////////////////
{
    float result = 0.0;

    char *str = getUCSString(line);
    for (int i = 0; i < getNumUCSChars(line); i++) {
    const FLbitmap *bmap = getBitmap((unsigned char*)(str+2*i));
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
    //take height from UCS-2 code for "M"
    const FLbitmap *bmap = getBitmap((unsigned char*)"\000M");
    if (bmap != NULL)
    return bmap->height - bmap->yorig;
    else return 0;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Draws a bitmap, using UCS-2 character
//
// Use: internal public

void
SoBitmapFontCache::drawCharacter(const char* c)
//
////////////////////////////////////////////////////////////////////////
{
    unsigned char *uc = (unsigned char*)c;
    const FLbitmap *bmap = getBitmap(uc);

    if (bmap != NULL)
    glBitmap(bmap->width, bmap->height, bmap->xorig, bmap->yorig,
         bmap->xmove, bmap->ymove, bmap->bitmap);
#ifdef DEBUG
    else SoDebugError::post("SoBitmapFontCache::drawCharacter",
    "no bitmap for character %d ", uc[0]*256+uc[1]);
#endif
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Draws a whole string.  Tries to build display lists, if it can.
//    Assumes string is in UCS-2 format.
//
// Use: internal public

void
SoBitmapFontCache::drawString(int line)
//
////////////////////////////////////////////////////////////////////////
{
    SbBool useCallLists = TRUE;

    char *str = getUCSString(line);
    unsigned char *ustr = (unsigned char*)str;

    // If there aren't any other caches open, build display lists for
    // the characters we can:
    for (int i = 0; i < getNumUCSChars(line); i++) {
    // See if the font cache already has (or can build) a display
    // list for this character:
    if (!hasDisplayList(str + 2*i)) {
        useCallLists = FALSE;
        break;
    }
    }

    // if we have display lists for all of the characters, use
    // glCallLists:
    if (useCallLists) {
    callLists(str, getNumUCSChars(line));
    } else {
    // if we don't, draw the string character-by-character, using the
    // display lists we do have:
    for (int i = 0; i < getNumUCSChars(line); i++) {
        if (!hasDisplayList(str + 2*i)) {
        drawCharacter(str + 2*i);
        }
        else glCallList(list->getFirstIndex()+
        ((ustr[2*i]<<8) | ustr[2*i+1]));
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
SoBitmapFontCache::getBitmap(unsigned char* c)
//
////////////////////////////////////////////////////////////////////////
{
    if (!fontNumList) return NULL;

    unsigned long key = (unsigned long)(c[0]<<8 | c[1]);
    void* value;
    if(!bitmapDict->find(key, value)){
    value = (void*)flUniGetBitmap(fontNumList, c);

#ifdef DEBUG
    if(value == NULL){
        SoDebugError::post("SoBitmapFontCache::getBitmap",
        "Invalid Unicode bitmap for character %d", key);
    }
#endif /*DEBUG*/

    bitmapDict->enter(key, value);
    }

    return (FLbitmap*)value;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    free up a bitmap.
//
// Use: static, private
void
SoBitmapFontCache::freeBitmap(unsigned long, void* value)
////////////////////////////////////////////////////////////////////////
{
    flFreeBitmap((FLbitmap*)value);
}

/////////////////////////////////////////////////////////////////////////
