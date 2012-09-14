#ifndef _SO_GL_DEPTH_BUFFER_ELEMENT_
#define _SO_GL_DEPTH_BUFFER_ELEMENT_

#include <Inventor/elements/SoDepthBufferElement.h>

class SoGLDepthBufferElement : public SoDepthBufferElement {

    SO_ELEMENT_HEADER(SoGLDepthBufferElement);

public:
    /// Initializes element
    virtual void	init(SoState *state);

    virtual void	push(SoState *state);

    /// Override pop() method so side effects can occur in GL
    virtual void	pop(SoState *state, const SoElement *prevTopElement);

SoINTERNAL public:
    // Initializes the SoGLDepthBufferElement class
    static void		initClass();

protected:
    virtual void	setElt(SbBool test, SbBool write, DepthWriteFunction function, const SbVec2f & range);

    virtual ~SoGLDepthBufferElement();
private:
    enum masks{
        TEST_MASK  = (1<<0),
        WRITE_MASK = (1<<1),
        FUNCT_MASK = (1<<2),
        RANGE_MASK = (1<<3)
    };

    uint32_t whatChanged;

    void		send();
};

#endif /* _SO_GL_DEPTH_BUFFER_ELEMENT_ */
