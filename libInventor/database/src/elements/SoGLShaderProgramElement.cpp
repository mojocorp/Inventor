#include <Inventor/elements/SoGLShaderProgramElement.h>
#include "SbGLShaderProgram.h"

SO_ELEMENT_SOURCE(SoGLShaderProgramElement);

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Destructor.
//
// Use: private

SoGLShaderProgramElement::~SoGLShaderProgramElement()
//
////////////////////////////////////////////////////////////////////////
{
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Initializes SoGLShaderProgramElement class.
//
// Use: internal

void
SoGLShaderProgramElement::initClass()
{
    SO_ELEMENT_INIT_CLASS(SoGLShaderProgramElement, SoReplacedElement);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Initializes element.
//
// Use: public

void
SoGLShaderProgramElement::init(SoState *_state)
//
////////////////////////////////////////////////////////////////////////
{
    // Initialize base class stuff
    SoReplacedElement::init(_state);

    shaderProgram = NULL;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//
//
// Use: public

void
SoGLShaderProgramElement::set(SoState *const state, SoNode *const node, SbGLShaderProgram * program)
//
////////////////////////////////////////////////////////////////////////
{
    // Get an instance we can change (pushing if necessary)
    SoGLShaderProgramElement *elt = (SoGLShaderProgramElement *) getElement(state, classStackIndex, node);

    if (elt != NULL) {
        elt->shaderProgram = program;
        elt->shaderProgram->bind();
    }
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//
//
// Use: public

SbGLShaderProgram*
SoGLShaderProgramElement::get(SoState *state)
//
////////////////////////////////////////////////////////////////////////
{
    const SoGLShaderProgramElement *elt = (const SoGLShaderProgramElement *)getConstElement(state, classStackIndex);

    return elt->shaderProgram;
}
////////////////////////////////////////////////////////////////////////
//
// Description:
//    Pushes element, copying data from previous element
//
// Use: public

void
SoGLShaderProgramElement::push(SoState *)
//
////////////////////////////////////////////////////////////////////////
{
    const SoGLShaderProgramElement	*prevElt =
    (const SoGLShaderProgramElement *) getNextInStack();

    shaderProgram = prevElt->shaderProgram;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Pops element, causing side effects in GL.
//
// Use: public

void
SoGLShaderProgramElement::pop(SoState *state, const SoElement *childElt)
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
    const SoGLShaderProgramElement *child =
    (const SoGLShaderProgramElement *) childElt;

    // Restore previous program
    if (shaderProgram != child->shaderProgram) {
        if (shaderProgram)
            shaderProgram->bind();
        else
            child->shaderProgram->release();
    }
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Compares program id's
//
// Use: public

SbBool
SoGLShaderProgramElement::matches(const SoElement *elt) const
//
////////////////////////////////////////////////////////////////////////
{
    return (shaderProgram == ((SoGLShaderProgramElement*)elt)->shaderProgram);
}
