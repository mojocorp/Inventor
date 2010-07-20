#ifndef _SO_IMAGE_BACKGROUD_
#define _SO_IMAGE_BACKGROUD_

#include <Inventor/nodes/SoBackground.h>
#include <Inventor/fields/SoSFColor.h>
#include <Inventor/fields/SoSFBool.h>

#include <Inventor/fields/SoSFString.h>
#include <Inventor/fields/SoSFEnum.h>
#include <Inventor/fields/SoSFImage.h>

class SoFieldSensor;

class INVENTOR_API SoImageBackground : public SoBackground {
    SO_NODE_HEADER(SoImageBackground);
public:
    enum Style {
        NONE,
        CENTER,
        LOWER_LEFT,
        UPPER_LEFT,
        UPPER_RIGHT,
        LOWER_RIGHT,
        STRETCH,
        TILE
    };

    SoSFString       filename;
    SoSFImage        image;
    SoSFEnum         style;

    /// Constructor
    SoImageBackground();

SoINTERNAL public:
    static void initClass();
protected:
    virtual void GLRender(SoGLRenderAction * );

    virtual ~SoImageBackground();
private:
    static void filenameChangedCB( void *, SoSensor * );
    static void imageChangedCB( void *, SoSensor * );

    SoFieldSensor * filenamesensor;
    SoFieldSensor * imagesensor;
};

#endif /* _SO_IMAGE_BACKGROUD_ */

