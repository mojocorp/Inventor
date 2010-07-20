#ifndef _SO_QT_INTERLACED_STEREO_
#define _SO_QT_INTERLACED_STEREO_

#include <Inventor/components/stereo/SoBaseStereo.h>

class INVENTOR_API SoQtInterlacedStereo : public SoBaseStereo
{
public:
    enum InterlacedMode {
        HORIZONTAL = 0, // default
        VERTICAL
    };

    SoQtInterlacedStereo(SoQtViewer*);
    ~SoQtInterlacedStereo();

    void setInterlacedMode(InterlacedMode newmode) { mode = newmode; }
    InterlacedMode getInterlacedMode() { return mode; }

    virtual StereoViewType getStereoViewType() { return INTERLACED_STEREO; }
    virtual void reverseStereoView(SbBool reverse);
    virtual SbBool isStereoViewReversed() { return FALSE; }
    virtual void renderStereoView() {}
private:
   InterlacedMode mode;
};

#endif /* _SO_QT_INTERLACED_STEREO_ */

