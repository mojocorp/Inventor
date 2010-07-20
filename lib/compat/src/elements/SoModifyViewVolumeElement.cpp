#include <Inventor/elements/SoModifyViewVolumeElement.h>

SO_ELEMENT_SOURCE(SoModifyViewVolumeElement);

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Destructor.
//
// Use: private

SoModifyViewVolumeElement::~SoModifyViewVolumeElement()
//
////////////////////////////////////////////////////////////////////////
{
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Initializes SoViewVolumeElement class.
//
// Use: internal

void
SoModifyViewVolumeElement::initClass()
{
    SO_ELEMENT_INIT_CLASS(SoModifyViewVolumeElement, SoElement);
}


////////////////////////////////////////////////////////////////////////
//
// Description:
//    Initializes element.
//
// Use: public

void
SoModifyViewVolumeElement::init(SoState *state)
//
////////////////////////////////////////////////////////////////////////
{
    SoElement::init(state);

    screenPortion.setValue(0., 0., 1., 1.);
    screenPortionBox.setBounds(0., 0., 0., 1., 1., 1.);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Overrides this method to return TRUE if the two
//    elements match.
//
// Use: public

SbBool
SoModifyViewVolumeElement::matches(const SoElement *elt) const
//
////////////////////////////////////////////////////////////////////////
{
    return (screenPortionBox == ((const SoModifyViewVolumeElement *) elt)->screenPortionBox) &&
           (screenPortion == ((const SoModifyViewVolumeElement *) elt)->screenPortion);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//     Create a copy of this instance suitable for calling matches()
//     on.
//
// Use: protected

SoElement *
SoModifyViewVolumeElement::copyMatchInfo() const
//
////////////////////////////////////////////////////////////////////////
{
    SoModifyViewVolumeElement *result =
	(SoModifyViewVolumeElement *)getTypeId().createInstance();

    result->screenPortion = screenPortion;
    result->screenPortionBox = screenPortionBox;

    return result;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Sets the ray view volume in element accessed from state.
//
// Use: public, static

void
SoModifyViewVolumeElement::set(SoState *state, const SbVec4f &portion)
//
////////////////////////////////////////////////////////////////////////
{
    SoModifyViewVolumeElement *elt;

    // Get an instance we can change (pushing if necessary)
    elt = (SoModifyViewVolumeElement *) getElement(state, classStackIndex);

    if (elt != NULL)
        elt->screenPortion = portion;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Sets the ray view volume in element accessed from state.
//
// Use: public, static

void
SoModifyViewVolumeElement::set(SoState *state, const SbBox3f &portion)
//
////////////////////////////////////////////////////////////////////////
{
    SoModifyViewVolumeElement *elt;

    // Get an instance we can change (pushing if necessary)
    elt = (SoModifyViewVolumeElement *) getElement(state, classStackIndex);

    if (elt != NULL)
        elt->screenPortionBox = portion;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Returns the ray view volume from the state
//
// Use: public, static

const SbVec4f &
SoModifyViewVolumeElement::get(SoState *state)
//
////////////////////////////////////////////////////////////////////////
{
    const SoModifyViewVolumeElement *elt;

    elt = (const SoModifyViewVolumeElement *)getConstElement(state, classStackIndex);

    return elt->screenPortion;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Returns the ray view volume from the state
//
// Use: public, static

const SbBox3f &
SoModifyViewVolumeElement::getBox(SoState *state)
//
////////////////////////////////////////////////////////////////////////
{
    const SoModifyViewVolumeElement *elt;

    elt = (const SoModifyViewVolumeElement *)getConstElement(state, classStackIndex);

    return elt->screenPortionBox;
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
SoModifyViewVolumeElement::print(FILE *fp) const
{
    SoElement::print(fp);
}
#else  /* DEBUG */
void
SoModifyViewVolumeElement::print(FILE *) const
{
}
#endif /* DEBUG */

const SoModifyViewVolumeElement *
SoModifyViewVolumeElement::getInstance(SoState *state, SbBool &elemIsSet)
{
    elemIsSet = FALSE;

    return (const SoModifyViewVolumeElement *)
        getConstElement(state, classStackIndex);
}
