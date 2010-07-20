#include <Inventor/elements/SoLogicalViewportElement.h>

SO_ELEMENT_SOURCE(SoLogicalViewportElement);

SbViewportRegion SoLogicalViewportElement::defaultLogicalViewport;

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Destructor.
//
// Use: private

SoLogicalViewportElement::~SoLogicalViewportElement()
//
////////////////////////////////////////////////////////////////////////
{
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Initializes SoLogicalViewportElement class.
//
// Use: internal

void
SoLogicalViewportElement::initClass()
{
    SO_ELEMENT_INIT_CLASS(SoLogicalViewportElement, SoElement);
    defaultLogicalViewport.setWindowSize(0, 0);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Initializes element
//
// Use: public

void
SoLogicalViewportElement::init(SoState *)
//
////////////////////////////////////////////////////////////////////////
{
    // Choose a reasonable default window size
    viewportRegion.setWindowSize(100, 100);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Sets viewport region to given matrix in element accessed from state.
//
// Use: public, static

void
SoLogicalViewportElement::set(SoState *state, const SbViewportRegion &vpReg)
//
////////////////////////////////////////////////////////////////////////
{
    SoLogicalViewportElement	*elt;

    // Get an instance we can change (pushing if necessary)
    elt = (SoLogicalViewportElement *) getElement(state, classStackIndex);

    if (elt != NULL)
        elt->viewportRegion = vpReg;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Returns viewport region from state.
//
// Use: public, static

const SbViewportRegion &
SoLogicalViewportElement::get(SoState *state)
//
////////////////////////////////////////////////////////////////////////
{
    const SoLogicalViewportElement *elt;

    elt = (const SoLogicalViewportElement *)getConstElement(state, classStackIndex);
    
    if (elt != NULL )
        return elt->viewportRegion;
    else
        return defaultLogicalViewport;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Returns TRUE if viewport regions are the same -- if they have
//    the same size and origin.
//
// Use: public

SbBool
SoLogicalViewportElement::matches(const SoElement *elt) const
//
////////////////////////////////////////////////////////////////////////
{
    return (viewportRegion == ((const SoLogicalViewportElement *) elt)->viewportRegion);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//     Create a copy of this instance suitable for calling matches()
//     on.
//
// Use: protected

SoElement *
SoLogicalViewportElement::copyMatchInfo() const
//
////////////////////////////////////////////////////////////////////////
{
    SoLogicalViewportElement *result =
	(SoLogicalViewportElement *)getTypeId().createInstance();

    result->viewportRegion = viewportRegion;
    
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
SoLogicalViewportElement::print(FILE *fp) const
{
    SoElement::print(fp);

    fprintf(fp, "\tLogical Viewport Region:\n");

    const SbVec2s	&winSize = viewportRegion.getWindowSize();
    const SbVec2f	&vpOrig  = viewportRegion.getViewportOrigin();
    const SbVec2f	&vpSize  = viewportRegion.getViewportSize();

    fprintf(fp, "\t\tWindow   size   = %d, %d\n", winSize[0], winSize[1]);
    fprintf(fp, "\t\tViewport origin = %g, %g\n", vpOrig[0], vpOrig[1]);
    fprintf(fp, "\t\tViewport size   = %g, %g\n", vpSize[0], vpSize[1]);
}
#else  /* DEBUG */
void
SoLogicalViewportElement::print(FILE *) const
{
}
#endif /* DEBUG */
