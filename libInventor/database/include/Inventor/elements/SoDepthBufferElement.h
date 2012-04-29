#ifndef _SO_DEPTH_BUFFER_ELEMENT_
#define _SO_DEPTH_BUFFER_ELEMENT_

#include <Inventor/SbVec2f.h>
#include <Inventor/elements/SoSubElement.h>

class SoDepthBufferElement : public SoElement {

    SO_ELEMENT_HEADER(SoDepthBufferElement);

public:
    enum DepthWriteFunction {
        NEVER,
        ALWAYS,
        LESS,
        LEQUAL,
        EQUAL,
        GEQUAL,
        GREATER,
        NOTEQUAL
    };

    /// Initializes element
    virtual void	init(SoState *state);

    /// Comparison based on value of floats
    virtual SbBool matches(const SoElement *elt) const;

    /// Copy method, takes care copying the data.
    virtual SoElement *copyMatchInfo() const;

    /// Prints element (for debugging)
    virtual void print(FILE *fp) const;

    static void set(SoState * state, SbBool test, SbBool write,
                    DepthWriteFunction function, const SbVec2f & range);

    static void get(SoState * state, SbBool & test_out, SbBool & write_out,
                    DepthWriteFunction & function_out, SbVec2f & range_out);

    /// Returns each default value
    static bool                 getDefaultTest() { return true; }
    static bool                 getDefaultWrite(){ return true; }
    static DepthWriteFunction   getDefaultFunction() { return LESS; }
    static SbVec2f              getDefaultRange() { return SbVec2f(0.0f, 1.0f); }

    virtual void	push(SoState *state);

SoINTERNAL public:
      // Initializes the SoDepthBufferElement class
      static void		initClass();

protected:
    // Virtual set methods that subclasses can override.
    virtual void	setElt(SbBool test, SbBool write,
                               DepthWriteFunction function, const SbVec2f & range);

    virtual ~SoDepthBufferElement();

    SbBool    test;
    SbBool    write;
    DepthWriteFunction function;
    SbVec2f range;
};

#endif /* _SO_DEPTH_BUFFER_ELEMENT_ */
