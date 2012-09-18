#include <Inventor/misc/SoGL.h>
#include <Inventor/elements/SoGLTextureUnitElement.h>
#include <Inventor/actions/SoGLRenderAction.h>

SO_ELEMENT_SOURCE(SoGLTextureUnitElement);

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Initializes SoGLTextureUnitElement class.
//
// Use: internal

void
SoGLTextureUnitElement::initClass()
{
    SO_ELEMENT_INIT_CLASS(SoGLTextureUnitElement, SoTextureUnitElement);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Destructor.
//
// Use: private

SoGLTextureUnitElement::~SoGLTextureUnitElement()
//
////////////////////////////////////////////////////////////////////////
{
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Initializes element
//
// Use: public

void
SoGLTextureUnitElement::init(SoState *state)
//
////////////////////////////////////////////////////////////////////////
{
    SoTextureUnitElement::init(state);

    copiedFromParent = NULL;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Pushes element, copying data from previous element
//
// Use: public

void
SoGLTextureUnitElement::push(SoState * state)
//
////////////////////////////////////////////////////////////////////////
{
    const SoGLTextureUnitElement  *prevElt = (const SoGLTextureUnitElement *) getNextInStack();

    data = prevElt->data;

    copiedFromParent = state;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Pops element, causing side effects in GL.
//
// Use: public

void
SoGLTextureUnitElement::pop(SoState *state, const SoElement *childElt)
//
////////////////////////////////////////////////////////////////////////
{
    // Since popping this element has GL side effects, make sure any
    // open caches capture it.  We may not send any GL commands, but
    // the cache dependency must exist even if we don't send any GL
    // commands, because if the element changes, the _lack_ of GL
    // commands here is a bug (remember, GL commands issued here are
    // put inside the cache).
    capture(state);
    copiedFromParent = NULL;

    // If the previous element didn't have the same value...
    const SoGLTextureUnitElement *child = (const SoGLTextureUnitElement *) childElt;
        
    // Restore previous texture unit
    if (data != child->data)
        send();
}

///////////////////////////////////////////////////////////////////////
//
// Description:
//    Sets point size in element.
//
// Use: protected, virtual

void
SoGLTextureUnitElement::setElt(int32_t unit)
//
////////////////////////////////////////////////////////////////////////
{
    // Optimization:  on push, we copy the value from the previous
    // element.  If the element is set to the same value, we don't
    // bother sending it, but do add a cache dependency on the
    // previous element.

    if (data != unit) {
        data = unit;
        send();
        copiedFromParent = NULL;
    }
    else if (copiedFromParent) {
        SoGLTextureUnitElement *parent =
            (SoGLTextureUnitElement *) getNextInStack();
        parent->capture(copiedFromParent);
    }
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Sends texture unit in element to GL.
//
// Use: private

void
SoGLTextureUnitElement::send()
//
////////////////////////////////////////////////////////////////////////
{
    glActiveTextureARB((unsigned int) GL_TEXTURE0 + data);
}
