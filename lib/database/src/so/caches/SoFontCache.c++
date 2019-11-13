#include <Inventor/caches/SoFontCache.h>

#include <Inventor/SbFile.h>
#include <Inventor/nodes/SoFont.h>
#include <Inventor/elements/SoFontNameElement.h>
#include <Inventor/elements/SoFontSizeElement.h>
#include <Inventor/errors/SoDebugError.h>

#include "utopia-regular.c++"

#include <ft2build.h>
#include FT_FREETYPE_H

// Static variables for SoFontCache
FT_Library SoFontCache::library = NULL;

void
SoFontCache::init() {
    // One-time font library initialization
    if (FT_Init_FreeType(&library)) {
#ifdef DEBUG
        SoDebugError::post("SoFontCache::SoFontCache",
                           "FT_Init_FreeType failed");
#endif
    }
}

void
SoFontCache::finish() {
    FT_Done_FreeType(library);
}

SoFontCache::SoFontCache(SoState *state) : SoCache(state), face(NULL) {
    const SbName &fontName = SoFontNameElement::get(state);

    // Grab all the stuff we'll need to determine our validity from
    // the state.
    addElement(state->getConstElement(SoFontNameElement::getClassStackIndex()));
    addElement(state->getConstElement(SoFontSizeElement::getClassStackIndex()));

    if (fontName == SoFontNameElement::getDefault() ||
        fontName == "Utopia-Regular") {
        if (FT_New_Memory_Face(library, binary_utopia_regular,
                               BINARY_UTOPIA_REGULAR_SIZE, 0, &face)) {
#ifdef DEBUG
            SoDebugError::post("SoBitmapFontCache::getFont",
                               "Couldn't load embeded font Utopia-Regular!");
#endif
        }
    } else {
        SbFile file;
        if (file.open(SoFont::getFontFileName(fontName), "rb")) {
            face_buffer.resize(SbFile::size(SoFont::getFontFileName(fontName)));
            file.read(&face_buffer[0], 1, face_buffer.size());

            if (FT_New_Memory_Face(library, &face_buffer[0],
                                   (FT_Long)face_buffer.size(), 0, &face)) {
#ifdef DEBUG
                SoDebugError::post(
                    "SoBitmapFontCache::getFont",
                    "Couldn't find font %s, replacing with Utopia-Regular",
                    fontName.getString());
#endif
            }
        }
    }

    if (!face && FT_New_Memory_Face(library, binary_utopia_regular,
                                    BINARY_UTOPIA_REGULAR_SIZE, 0, &face)) {
#ifdef DEBUG
        SoDebugError::post("SoText2::getFont",
                           "Couldn't find font Utopia-Regular!");
#endif
    }
}
