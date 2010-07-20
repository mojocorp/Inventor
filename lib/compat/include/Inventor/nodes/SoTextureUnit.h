#ifndef _SO_TEXTURE_UNIT_
#define _SO_TEXTURE_UNIT_

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/nodes/SoNode.h>
#include <Inventor/fields/SoSFUInt32.h>

class INVENTOR_API SoTextureUnit : public SoNode {
    SO_NODE_HEADER(SoTextureUnit);
public:
    SoTextureUnit();

    SoSFUInt32 unit;

SoINTERNAL public:
    static void initClass();
protected:
    virtual void  doAction(SoAction *action);
    virtual void  callback(SoCallbackAction *action);
    virtual void  GLRender(SoGLRenderAction *action);

    virtual ~SoTextureUnit();
};

#endif /* _SO_TEXTURE_UNIT_ */
