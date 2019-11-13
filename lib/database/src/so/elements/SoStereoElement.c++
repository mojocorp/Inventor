#include <Inventor/elements/SoStereoElement.h>

SO_ELEMENT_SOURCE(SoStereoElement);

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Destructor.
//
// Use: private

SoStereoElement::~SoStereoElement()
//
////////////////////////////////////////////////////////////////////////
{}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Initializes SoStereoElement class.
//
// Use: internal

void
SoStereoElement::initClass()
//
////////////////////////////////////////////////////////////////////////
{
    SO_ELEMENT_INIT_CLASS(SoStereoElement, SoElement);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Initializes element.
//
// Use: public

void
SoStereoElement::init(SoState *)
//
////////////////////////////////////////////////////////////////////////
{
    mode = getDefaultMode();
    offset = getDefaultOffset();
    balance = getDefaultBalace();
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//
//
// Use: public, static

void
SoStereoElement::set(SoState *state, StereoMode mode, float offset,
                     float balance)
//
////////////////////////////////////////////////////////////////////////
{
    // Get an instance we can change (pushing if necessary)
    SoStereoElement *elt =
        (SoStereoElement *)getElement(state, classStackIndex);

    if (elt != NULL) {
        elt->mode = mode;
        elt->offset = offset;
        elt->balance = balance;
    }
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Pushes the element, copying values from previous element in
//    state:
//
// Use: public, virtual

void
SoStereoElement::push(SoState *)
//
////////////////////////////////////////////////////////////////////////
{
    const SoStereoElement *prevElt = (const SoStereoElement *)getNextInStack();

    mode = prevElt->mode;
    offset = prevElt->offset;
    balance = prevElt->balance;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Returns shape hints from state.
//
// Use: public, static

void
SoStereoElement::get(SoState *state, StereoMode &mode_out, float &offset_out,
                     float &balance_out)
//
////////////////////////////////////////////////////////////////////////
{
    const SoStereoElement *elt;

    elt = (const SoStereoElement *)getConstElement(state, classStackIndex);

    mode_out = elt->mode;
    offset_out = elt->offset;
    balance_out = elt->balance;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Overrides this method to compare shape hints.
//
// Use: public

SbBool
SoStereoElement::matches(const SoElement *elt) const
//
////////////////////////////////////////////////////////////////////////
{
    const SoStereoElement *shElt = (const SoStereoElement *)elt;

    return (mode == shElt->mode && offset == shElt->offset &&
            balance == shElt->balance);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//     Create a copy of this instance suitable for calling matches()
//     on.
//
// Use: protected

SoElement *
SoStereoElement::copyMatchInfo() const
//
////////////////////////////////////////////////////////////////////////
{
    SoStereoElement *result = (SoStereoElement *)getTypeId().createInstance();

    result->mode = mode;
    result->offset = offset;
    result->balance = balance;

    return result;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Prints element for debugging.
//
// Use: public
//
////////////////////////////////////////////////////////////////////////

#ifdef DEBUG
void
SoStereoElement::print(FILE *fp) const {
    SoElement::print(fp);

    const char *modeName = NULL;
    switch (mode) {
    case MONOSCOPIC:
        modeName = "MONOSCOPIC";
        break;
    case LEFT_VIEW:
        modeName = "LEFT_VIEW";
        break;
    case RIGHT_VIEW:
        modeName = "RIGHT_VIEW";
        break;
    default:
        modeName = "UNKNOWN";
        break;
    }

    fprintf(fp, "\tStereo: mode = %s, offset = %f, balance = %f\n", modeName,
            offset, balance);
}
#else  /* DEBUG */
void
SoStereoElement::print(FILE *) const {}
#endif /* DEBUG */
