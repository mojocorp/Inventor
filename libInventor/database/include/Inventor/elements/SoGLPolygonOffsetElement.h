#ifndef _SO_GL_POLYGON_OFFSET_ELEMENT
#define _SO_GL_POLYGON_OFFSET_ELEMENT

#include <Inventor/elements/SoPolygonOffsetElement.h>

//////////////////////////////////////////////////////////////////////////////
//
//  Class: SoGLPolygonOffsetElement
//
//  Element that stores the current polygon offset in GL. Overrides the
//  virtual methods on SoPolygonOffsetElement to send the polygon offset
//  to GL when necessary.
//
//  Note that this class relies on SoPolygonOffsetElement to store the
//  values in the instance.
//
//////////////////////////////////////////////////////////////////////////////

SoEXTENDER class INVENTOR_API SoGLPolygonOffsetElement : public SoPolygonOffsetElement
{
    SO_ELEMENT_HEADER(SoGLPolygonOffsetElement);

public:

    /// Initializes element
    virtual void init(SoState *state);

    /// Override push() so we can remember what values the
    /// previous element set, and not send it again if it is the same.
    virtual void push(SoState *state);

    /// Override pop() method so side effects can occur in GL
    virtual void pop(SoState *state, const SoElement *childElt);

SoINTERNAL public:

    // Initializes the SoGLPolygonOffsetElement class.
    static void initClass();

protected:

    // Sets the polygon offset in an instance. Has GL side effects.
    virtual void setElt(float factor, float units, Style styles, SbBool active);

    // Destructor.
    virtual ~SoGLPolygonOffsetElement();

private:

    // We save the state to figure out if the lastPattern variable was
    // copied from a parent element; if it was, then caches will have
    // to depend on that element because if it changes we have to have
    // a chance to change our decision about what GL calls to make.
    // If this is NULL, then there are no cache dependencies.
    SoState * copiedFromParent;

    enum masks{
        VALUE_MASK	= (1<<0),
        STYLE_MASK	= (1<<1)
    };

    uint32_t whatChanged;

    // Sends polygon offset in element to GL
    void send();
};

#endif  /* _SO_GL_POLYGON_OFFSET_ELEMENT */
