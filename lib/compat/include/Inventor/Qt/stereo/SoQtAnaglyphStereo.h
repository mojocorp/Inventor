#ifndef _SO_QT_ANAGLYPH_STEREO_
#define _SO_QT_ANAGLYPH_STEREO_

#include <Inventor/components/stereo/SoBaseStereo.h>

class INVENTOR_API SoQtAnaglyphStereo : public SoBaseStereo
{
public:
    enum ColorFilter {
        RED_CYAN,
        GREEN_MAGENTA,
        BLUE_YELLOW
    };

    SoQtAnaglyphStereo(SoQtViewer*);
    ~SoQtAnaglyphStereo();

    virtual StereoViewType getStereoViewType() { return HALF_SCREEN_STEREO; }
    virtual void reverseStereoView(SbBool reverse);
    virtual SbBool isStereoViewReversed() { return FALSE; }
    virtual void renderStereoView() {}

    void setColorFilter(ColorFilter newfilter) { filter = newfilter; }
    ColorFilter getColorFilter() { return filter; }
private:
    ColorFilter filter;
};

#endif /* _SO_QT_ANAGLYPH_STEREO_ */

