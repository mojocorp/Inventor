#ifndef _SO_TEXTURE_UNIT_H_
#define _SO_TEXTURE_UNIT_H_

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/fields/SoSFInt32.h>

class SoState;

class INVENTOR_API SoTextureUnit : public SoNode {

    SO_NODE_HEADER(SoTextureUnit);

public:
    // Fields:
    //
    SoSFInt32 unit;

    /**
     * Creates a texture unit node with default settings.
     */
    SoTextureUnit();

    /**
     * Returns the maximum supported texture image units for the current context.
     * Units can be used to access texture maps from the fragment shader. 
     * The value must be at least 2.
     */
    static uint32_t getMaxTextureUnit(SoState *state);

SoINTERNAL public:
    // Initializes this class for use in scene graphs. This
    // should be called after database initialization and before
    // any instance of this node is constructed.
    static void    initClass();

SoEXTENDER public:
    // Implement render action
    virtual void        doAction(SoAction *action);
    virtual void        callback(SoCallbackAction *action);
    virtual void        GLRender(SoGLRenderAction *action);
    virtual void        pick(SoPickAction *action);

protected:
    // Destructor. Protected to keep people from trying to delete
    // nodes, rather than using the reference count mechanism.
    virtual ~SoTextureUnit();

private:
};

#endif /* _SO_TEXTURE_UNIT_H_ */
