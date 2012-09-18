#ifndef  _SO_GL_TEXTURE_UNIT_ELEMENT_
#define  _SO_GL_TEXTURE_UNIT_ELEMENT_

#include <Inventor/elements/SoTextureUnitElement.h>

SoEXTENDER class INVENTOR_API SoGLTextureUnitElement : public SoTextureUnitElement
{
    SO_ELEMENT_HEADER(SoGLTextureUnitElement);

public:
    /// Initializes element
    virtual void        init(SoState *state);

    /// Override push() method to copy state pointer and value of
    /// previous element, to avoid sending GL commands if the elements
    /// are the same.
    virtual void        push(SoState *state);

    /// Override pop() method so side effects can occur in GL
    virtual void        pop(SoState *state, const SoElement *prevTopElement);
SoINTERNAL public:

    //! Initializes the SoTextureUnitElement class
    static void         initClass();

protected:
    /// Sets the texture unit in an instance. Has GL side effects.
    virtual void        setElt(int32_t unit);

    virtual ~SoGLTextureUnitElement();

    SoState             *copiedFromParent;

    // Sends texture unit in element to GL
    void		send();
};

#endif /* _SO_GL_TEXTURE_UNIT_ELEMENT_ */
