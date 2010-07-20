#ifndef _SO_QT_RAW_STEREO_
#define _SO_QT_RAW_STEREO_

#include <Inventor/components/stereo/SoBaseStereo.h>

#define SoQtStereoViewType SoBaseStereo

class INVENTOR_API SoQtRawStereo : public SoBaseStereo
{
public:
    SoQtRawStereo(SoQtViewer*);
    ~SoQtRawStereo();

    virtual StereoViewType getStereoViewType() { return RAW_STEREO; }
    virtual void reverseStereoView(SbBool reverse);
    virtual SbBool isStereoViewReversed() { return FALSE; }
    virtual void renderStereoView() {}
};

#endif /* _SO_QT_RAW_STEREO_ */

