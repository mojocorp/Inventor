#include <Inventor/misc/SoGL.h>
#include <Inventor/elements/SoGLTextureCoordinate3Element.h>
#include <Inventor/errors/SoDebugError.h>

SO_ELEMENT_SOURCE(SoGLTextureCoordinate3Element);

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Destructor.
//
// Use: private

SoGLTextureCoordinate3Element::~SoGLTextureCoordinate3Element()
//
////////////////////////////////////////////////////////////////////////
{
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Initializes SoGLTextureCoordinate3Element class.
//
// Use: internal

void
SoGLTextureCoordinate3Element::initClass()
{
    SO_ELEMENT_INIT_CLASS(SoGLTextureCoordinate3Element,
                          SoTextureCoordinate3Element);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Initializes element.
//
// Use: public

void
SoGLTextureCoordinate3Element::init(SoState *s)
//
////////////////////////////////////////////////////////////////////////
{
    SoTextureCoordinate3Element::init(s);

    texgenCB = NULL;
    texgenCBData = NULL;
    copiedFromParent = NULL;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    TexGen is being used
//
// Use: public

void
SoGLTextureCoordinate3Element::setTexGen(SoState *state, SoNode *node,
                                         SoTexCoordTexgenCB *texGenFunc, void *texGenData,
                                         SoTextureCoordinateFunctionCB *func, void *funcData)
//
////////////////////////////////////////////////////////////////////////
{
    // Do base-class stuff
    SoTextureCoordinate3Element::setFunction(state, node, func,
                                             funcData);

    // Get an element we can modify:
    SoGLTextureCoordinate3Element	*elt;
    elt = (SoGLTextureCoordinate3Element *)
            getElement(state, classStackIndex, node);

    if (elt != NULL) {

        elt->setElt(texGenFunc, texGenData);

    }
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    TexGen is being used
//
// Use: protected

void
SoGLTextureCoordinate3Element::setElt(SoTexCoordTexgenCB *func,
                                      void *userData)
//
////////////////////////////////////////////////////////////////////////
{
    // Enable or disable texgen as appropriate
    if (func != NULL) {
        // Only call Enable if not already enabled:
        if (texgenCB == NULL) {
            glEnable(GL_TEXTURE_GEN_S);
            glEnable(GL_TEXTURE_GEN_T);
            glEnable(GL_TEXTURE_GEN_R);
            copiedFromParent = NULL;
        }
        else if (copiedFromParent) {
            SoGLTextureCoordinate3Element *parent =
                    (SoGLTextureCoordinate3Element *) getNextInStack();
            parent->capture(copiedFromParent);
        }

        // Call function to set up texgen parameters
        (*func)(userData);

        whatKind     = FUNCTION;
    }
    else {
        // Only call Disable if parent element was enabled:
        if (texgenCB != NULL) {
            glDisable(GL_TEXTURE_GEN_S);
            glDisable(GL_TEXTURE_GEN_T);
            glDisable(GL_TEXTURE_GEN_R);
            copiedFromParent = NULL;
        }
        else if (copiedFromParent) {
            SoGLTextureCoordinate3Element *parent =
                    (SoGLTextureCoordinate3Element *) getNextInStack();
            parent->capture(copiedFromParent);
        }
    }

    texgenCB     = func;
    texgenCBData = userData;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Returns code depending on which set routine was called.
//
// Use: public, virtual

SoGLTextureCoordinate3Element::CoordType
SoGLTextureCoordinate3Element::getType() const
//
////////////////////////////////////////////////////////////////////////
{
    return whatKind;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Returns the top (current) instance of the element in the state.
//
// Use: public

const SoGLTextureCoordinate3Element *
SoGLTextureCoordinate3Element::getInstance(SoState *state)
//
////////////////////////////////////////////////////////////////////////
{
    return (const SoGLTextureCoordinate3Element *)
            getConstElement(state, classStackIndex);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Given an index, send the appropriate stuff to GL.
//
// Use: public

void
SoGLTextureCoordinate3Element::send(int index) const
//
////////////////////////////////////////////////////////////////////////
{
#ifdef DEBUG
    if (whatKind != EXPLICIT)
        SoDebugError::post("SoGLTextureCoordinate3Element::send",
                           "explicit texture coordinates were not set!");

    if (index < 0 || index >= numCoords)
        SoDebugError::post("SoGLTextureCoordinate3Element::send",
                           "Index (%d) out of range 0 - %d",
                           index, numCoords - 1);
#endif /* DEBUG */

    if (coordsAre3D)
        glTexCoord3fv(coords3[index].getValue());
    else
        glTexCoord4fv(coords4[index].getValue());
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Pushes element, initializing new instance.
//
// Use: public

void
SoGLTextureCoordinate3Element::push(SoState *state)
//
////////////////////////////////////////////////////////////////////////
{
    // Copy texgen function and data from previous instance
    const SoGLTextureCoordinate3Element *prevElt =
            (const SoGLTextureCoordinate3Element *) getNextInStack();
    texgenCB     = prevElt->texgenCB;
    texgenCBData = prevElt->texgenCBData;

    copiedFromParent = state;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Pops element, causing side effects in GL if necessary.  This
//    code is a little convoluted-- we're trying pretty hard not to
//    send GL commands if we don't have to.
//
// Use: public

void
SoGLTextureCoordinate3Element::pop(SoState *state, const SoElement *childElt)
//
////////////////////////////////////////////////////////////////////////
{
    // If the previous element didn't have the same value...
    const SoGLTextureCoordinate3Element *child =
            (const SoGLTextureCoordinate3Element *)childElt;

    // Since popping this element may have GL side effects, we must
    // capture the state.  We may not send any GL commands, but
    // the cache dependency must exist even if we don't send any GL
    // commands, because if the element changes, the _lack_ of GL
    // commands here is a bug (remember, GL commands issued here are
    // put inside the cache).
    capture(state);
    copiedFromParent = NULL;

    // Different callbacks, must either disable texgen or resend:
    if (texgenCB != NULL) {

        // Enable if it wasn't enabled before:
        if (child->texgenCB == NULL) {
            glEnable(GL_TEXTURE_GEN_S);
            glEnable(GL_TEXTURE_GEN_T);
            glEnable(GL_TEXTURE_GEN_R);
        }

        (*texgenCB)(texgenCBData);
    }
    else if (child->texgenCB != NULL) {
        glDisable(GL_TEXTURE_GEN_S);
        glDisable(GL_TEXTURE_GEN_T);
        glDisable(GL_TEXTURE_GEN_R);
    }
}
