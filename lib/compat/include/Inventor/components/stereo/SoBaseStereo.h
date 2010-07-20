#ifndef _SO_BASE_STEREO_
#define _SO_BASE_STEREO_

#include <Inventor/SbBasic.h>

class SoQtViewer;

class INVENTOR_API SoBaseStereo
{
public:
    enum StereoViewType {
        NO_STEREO_VIEW     = 0,
        INTERLACED_STEREO  = 1,
        HALF_SCREEN_STEREO = 2,
        RAW_STEREO         = 3,
        OPENGL_STEREO      = 3,
        ANAGLYPH_STEREO    = 4,
        ANAGLYPH           = 4,
        SSDI_STEREO        = 5,
        PASSIVE_STEREO     = 6
    };

    SoBaseStereo(SoQtViewer *stereoViewer = NULL);
    virtual ~SoBaseStereo();

    virtual StereoViewType getStereoViewType() = 0;
    virtual void reverseStereoView(SbBool reverse) = 0;
    virtual SbBool isStereoViewReversed() = 0;
    virtual void renderStereoView() = 0;
};

#endif /* _SO_BASE_STEREO_ */

