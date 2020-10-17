#pragma once

#include <Inventor/caches/SoCache.h>

#include <vector>

class INVENTOR_API SoFontCache : public SoCache {
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

