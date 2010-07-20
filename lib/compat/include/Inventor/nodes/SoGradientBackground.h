#ifndef _SO_GRADIENT_BACKGROUD_
#define _SO_GRADIENT_BACKGROUD_

#include <Inventor/nodes/SoBackground.h>
#include <Inventor/fields/SoSFColor.h>
#include <Inventor/fields/SoSFBool.h>
#include <Inventor/fields/SoSFVec2f.h>

class INVENTOR_API SoGradientBackground : public SoBackground {

    SO_NODE_HEADER(SoGradientBackground);

public:
    /// Constructor
    SoGradientBackground();

    /// First color in gradient (0.7 0.7 0.8 by default).
    SoSFColor color0;
    /// Second color in gradient (0.0 0.1 0.3 by default).
    SoSFColor color1;
    /// Reverse colors (default is false).
    SoSFBool swapColors;
    /// gradient direction (default is vertical).
    SoSFVec2f direction;

SoINTERNAL public:
    static void initClass();
protected:
    virtual void GLRender(SoGLRenderAction * );

    virtual ~SoGradientBackground();
};

#endif /* _SO_GRADIENT_BACKGROUD_ */
