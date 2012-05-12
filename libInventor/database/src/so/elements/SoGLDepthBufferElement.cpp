#include <Inventor/misc/SoGL.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/elements/SoGLDepthBufferElement.h>
#include <Inventor/misc/SoState.h>

SO_ELEMENT_SOURCE(SoGLDepthBufferElement);

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Destructor.
//
// Use: private

SoGLDepthBufferElement::~SoGLDepthBufferElement()
//
////////////////////////////////////////////////////////////////////////
{
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Initializes SoGLDepthBufferElement class.
//
// Use: internal

void
SoGLDepthBufferElement::initClass()
{
    SO_ELEMENT_INIT_CLASS(SoGLDepthBufferElement, SoDepthBufferElement);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Initializes element.
//
// Use: public

void
SoGLDepthBufferElement::init(SoState *_state)
//
////////////////////////////////////////////////////////////////////////
{
    // Initialize base class stuff
    SoDepthBufferElement::init(_state);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Pushes the element, copying values from previous element in
//    state:
//
// Use: public, virtual

void
SoGLDepthBufferElement::push(SoState *)
//
////////////////////////////////////////////////////////////////////////
{
    const SoGLDepthBufferElement	*prevElt =
        (const SoGLDepthBufferElement *)getNextInStack();

    test        = prevElt->test;
    write       = prevElt->write;
    function    = prevElt->function;
    range       = prevElt->range;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Pops element, causing side effects in GL.
//
// Use: public

void
SoGLDepthBufferElement::pop(SoState *state, const SoElement *childElt)
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

    // If the previous element didn't have the same value...
    const SoGLDepthBufferElement *child =
            (const SoGLDepthBufferElement *) childElt;

    // Restore previous point size
    if ((test != child->test) ||
        (write != child->write) ||
        (function != child->function) ||
        (range != child->range)) {
        send();
    }
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Sets depth params in element.
//
// Use: protected, virtual

void
SoGLDepthBufferElement::setElt(SbBool _test, SbBool _write, DepthWriteFunction _function, const SbVec2f & _range)
//
////////////////////////////////////////////////////////////////////////
{
    SoDepthBufferElement::setElt(test, write, function, range);

    if ((test != _test) ||
        (write != _write) ||
        (function != _function) ||
        (range != _range)) {

        send();
    }
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Sends depth params in element to GL.
//
// Use: private

void
SoGLDepthBufferElement::send()
//
////////////////////////////////////////////////////////////////////////
{
    if (test) {
        glEnable(GL_DEPTH_TEST);
    } else {
        glDisable(GL_DEPTH_TEST);
    }

    if (write) {
        glDepthMask(GL_TRUE);
    } else {
        glDepthMask(GL_FALSE);
    }

    switch (function) {
    case NEVER:     glDepthFunc(GL_NEVER);     break;
    case ALWAYS:    glDepthFunc(GL_ALWAYS);    break;
    case LESS:      glDepthFunc(GL_LESS);      break;
    case LEQUAL:    glDepthFunc(GL_LEQUAL);    break;
    case EQUAL:     glDepthFunc(GL_EQUAL);     break;
    case GEQUAL:    glDepthFunc(GL_GEQUAL);    break;
    case GREATER:   glDepthFunc(GL_GREATER);   break;
    case NOTEQUAL:  glDepthFunc(GL_NOTEQUAL);  break;
    default:	// Just to shut CC up
        break;
    }

    glDepthRange(range[0], range[1]);
}
