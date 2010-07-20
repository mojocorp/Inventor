#ifndef _SO_QT_HALF_SCREEN_STEREO_
#define _SO_QT_HALF_SCREEN_STEREO_

#include <Inventor/components/stereo/SoBaseStereo.h>

class INVENTOR_API SoQtHalfScreenStereo : public SoBaseStereo
{
public:
    enum HalfScreenMode {
        OVERUNDER_FILL = 0,
        OVERUNDER,
        SIDEBYSIDE_FILL,
        SIDEBYSIDE
    };

    SoQtHalfScreenStereo(SoQtViewer*);
    ~SoQtHalfScreenStereo();

    void setHalfScreenMode(HalfScreenMode newmode) { mode = newmode; }
    HalfScreenMode getHalfScreenMode() { return mode; }

    virtual StereoViewType getStereoViewType() { return ANAGLYPH_STEREO; }
    virtual void reverseStereoView(SbBool reverse);
    virtual SbBool isStereoViewReversed() { return FALSE; }
    virtual void renderStereoView() {}
private:
    HalfScreenMode mode;
};

#endif /* _SO_QT_HALF_SCREEN_STEREO_ */

