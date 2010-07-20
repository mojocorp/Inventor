#include <Inventor/elements/SoProjectionElement.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>

SO_ELEMENT_SOURCE(SoProjectionElement);

SoProjectionElement::~SoProjectionElement()
{

}

void
SoProjectionElement::initClass()
{
    SO_ELEMENT_INIT_CLASS(SoProjectionElement, SoReplacedElement);

    SO_ENABLE(SoGLRenderAction, SoProjectionElement);
    SO_ENABLE(SoGetBoundingBoxAction, SoProjectionElement);
}

void
SoProjectionElement::init(SoState *state)
{
    SoReplacedElement::init(state);

    projection = NULL;
}

void
SoProjectionElement::set( SoState *state, SoProjection *prj )
{
    SoProjectionElement::set(state, NULL, prj);
}

void
SoProjectionElement::set( SoState *state, SoNode * node, SoProjection *prj )
{
    SoProjectionElement *elt = (SoProjectionElement *) getElement(state, classStackIndex, node);

    if (elt != NULL)
        elt->projection = prj;
}

SoProjection *
SoProjectionElement::get( SoState *state )
{
    const SoProjectionElement *elt;

    elt = (const SoProjectionElement *)getConstElement(state, classStackIndex);

    return elt->projection;
}
