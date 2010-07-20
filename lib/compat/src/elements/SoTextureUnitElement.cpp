#include <Inventor/elements/SoTextureUnitElement.h>
#include <Inventor/errors/SoDebugError.h>

SO_ELEMENT_SOURCE(SoTextureUnitElement);

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Destructor.
//
// Use: public

SoTextureUnitElement::~SoTextureUnitElement()
//
////////////////////////////////////////////////////////////////////////
{
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Initializes SoTextureUnitElement class.
//
// Use: internal

void
SoTextureUnitElement::initClass()
{
    SO_ELEMENT_INIT_CLASS(SoTextureUnitElement, SoAccumulatedElement);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Initializes element.
//
// Use: public

void
SoTextureUnitElement::init(SoState *)
//
////////////////////////////////////////////////////////////////////////
{
    units.truncate(0);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Adds the given profile to the list of profiles
//
// Use: public

void
SoTextureUnitElement::add(SoState *state, int unit)
//
////////////////////////////////////////////////////////////////////////
{
    SoTextureUnitElement	*elt;

    elt = (SoTextureUnitElement *) getElement(state, classStackIndex);

    if (elt != NULL && unit >= 0) {
        elt->units.append(unit);
    }
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Returns the current texture unit list
//
// Use: public

const SbIntList &
SoTextureUnitElement::get(SoState *state)
//
////////////////////////////////////////////////////////////////////////
{
    const SoTextureUnitElement *elt;
    elt = (const SoTextureUnitElement *)
        getConstElement(state, classStackIndex);

    return elt->units;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Override push to copy the existing texture units from the previous
//    set.
//
// Use: public

void
SoTextureUnitElement::push(SoState *)
//
////////////////////////////////////////////////////////////////////////
{
    SoTextureUnitElement *elt;
    elt = (SoTextureUnitElement *) getNextInStack();

    units = elt->units;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Prints contents of element for debugging.
//
// Use: public
//
////////////////////////////////////////////////////////////////////////

#ifdef DEBUG
void
SoTextureUnitElement::print(FILE *fp) const
{
    SoAccumulatedElement::print(fp);
}
#else  /* DEBUG */
void
SoTextureUnitElement::print(FILE *) const
{
}
#endif /* DEBUG */
