#ifndef  _SO_FONT_CACHE_
#define  _SO_FONT_CACHE_

#include <Inventor/caches/SoCache.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <vector>

class SoFontCache : public SoCache {
public:
    //  Init function
    static void init();
    static void finish();
protected:
    /// Constructor
    SoFontCache(SoState *state);
public:
    // Font library identifier for this font
    FT_Face face;

private:
    static FT_Library library;

    std::vector<unsigned char> buffer;
};

#endif /* _SO_FONT_CACHE_ */
