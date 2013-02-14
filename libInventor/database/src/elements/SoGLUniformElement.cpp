#include <Inventor/elements/SoGLUniformElement.h>
#include <Inventor/errors/SoDebugError.h>

SO_ELEMENT_SOURCE(SoGLUniformElement);

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Destructor.
//
// Use: private

SoGLUniformElement::~SoGLUniformElement()
//
////////////////////////////////////////////////////////////////////////
{
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Initializes SoGLUniformElement class.
//
// Use: internal

void
SoGLUniformElement::initClass()
{
    SO_ELEMENT_INIT_CLASS(SoGLUniformElement, SoAccumulatedElement);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Initializes element.
//
// Use: public

void
SoGLUniformElement::init(SoState *)
//
////////////////////////////////////////////////////////////////////////
{
    planes.truncate(0);
    startIndex = 0;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Adds a clip plane to the current set in the state.
//
// Use: public, static
/*
void
SoGLUniformElement::add(SoState *state, SoNode *node, const SbPlane &plane)
//
////////////////////////////////////////////////////////////////////////
{
    // Get an instance we can change (pushing if necessary)
    SoGLUniformElement *elt = (SoGLUniformElement *) getElement(state, classStackIndex);

    if (elt != NULL) {
        //elt->addToElt(plane, SoModelMatrixElement::get(state));

        // Update node id list in element
        elt->addNodeId(node);
    }
}
*/
////////////////////////////////////////////////////////////////////////
//
// Description:
//    Override push to copy the existing planes from the previous
//    element.
//
// Use: public

void
SoGLUniformElement::push(SoState *)
//
////////////////////////////////////////////////////////////////////////
{
    const SoGLUniformElement *elt = (const SoGLUniformElement *) getNextInStack();

    // Use SbPList::operator = to copy the pointers to the existing
    // planes. Since the previous element can't be destroyed before
    // this one is, there is no problem with using pointers to
    // existing plane structures. However, any new ones we add have to
    // be freed up when this instance goes away, so we save the
    // starting index to allow us to fix this in pop().
    planes = elt->planes;
    startIndex = planes.getLength();
    nodeIds = elt->nodeIds;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Override pop to free up plane structures that were created in
//    the instance that was removed from the state.
//
// Use: public

void
SoGLUniformElement::pop(SoState *, const SoElement *prevTopElement)
//
////////////////////////////////////////////////////////////////////////
{
    const SoGLUniformElement *prevElt = (const SoGLUniformElement *) prevTopElement;

    // Free up any plane structures that were created by prevElt
    //for (int i = prevElt->startIndex; i < prevElt->planes.getLength(); i++)
    //    delete (So_ClipPlane *) prevElt->planes[i];
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Returns the first (top) instance of the element in the state.
//
// Use: public, static

const SoGLUniformElement *
SoGLUniformElement::getInstance(SoState *state)
//
////////////////////////////////////////////////////////////////////////
{
    return (const SoGLUniformElement *)getConstElement(state, classStackIndex);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Returns the number of planes in the element
//
// Use: public

int
SoGLUniformElement::getNum() const
//
////////////////////////////////////////////////////////////////////////
{
    return planes.getLength();
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Returns the nth plane stored in an instance in object or world
//    space.
//
// Use: public
#if 0
const SbPlane &
SoGLUniformElement::get(int index, SbBool inWorldSpace) const
//
////////////////////////////////////////////////////////////////////////
{
#ifdef DEBUG
    if (index < 0 || index >= planes.getLength())
    SoDebugError::post("SoGLUniformElement::get",
			   "Index (%d) is out of range 0 - %d",
			   index, planes.getLength() - 1);
#endif /* DEBUG */

    So_ClipPlane *plane = (So_ClipPlane *) planes[index];

    if (! inWorldSpace)
	return plane->objPlane;

    // Transform plane into world space if necessary
    if (! plane->worldPlaneValid) {
	plane->worldPlane = plane->objPlane;
	plane->worldPlane.transform(plane->objToWorld);
    }
    return plane->worldPlane;
}
#endif
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
SoGLUniformElement::print(FILE *fp) const
{
    SoAccumulatedElement::print(fp);
}
#else  /* DEBUG */
void
SoGLUniformElement::print(FILE *) const
{
}
#endif /* DEBUG */
