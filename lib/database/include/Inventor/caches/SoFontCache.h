#ifndef _SO_FONT_CACHE_
#define _SO_FONT_CACHE_

#include <Inventor/caches/SoCache.h>

#include <vector>

class SoFontCache : public SoCache {
    // Forward declarations for freetype:
    typedef struct FT_FaceRec_ *   FT_Face;
    typedef struct FT_LibraryRec_ *FT_Library;

  public:
    //  Init function
    static void init();
    static void finish();

  protected:
    // Constructor
    SoFontCache(SoState *state);

  public:
    // Font library identifier for this font
    FT_Face face;

  private:
    static FT_Library library;

    std::vector<unsigned char> face_buffer;
};

#endif /* _SO_FONT_CACHE_ */
