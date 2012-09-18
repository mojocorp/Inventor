#include <Inventor/elements/SoTextureCoordinate3Element.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/elements/SoShapeStyleElement.h>

SO_ELEMENT_SOURCE(SoTextureCoordinate3Element);

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Destructor.
//
// Use: private

SoTextureCoordinate3Element::~SoTextureCoordinate3Element()
//
////////////////////////////////////////////////////////////////////////
{
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Initializes SoTextureCoordinate3Element class.
//
// Use: internal

void
SoTextureCoordinate3Element::initClass()
{
    SO_ELEMENT_INIT_CLASS(SoTextureCoordinate3Element, SoReplacedElement);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Initializes element.
//
// Use: public

void
SoTextureCoordinate3Element::init(SoState *state)
//
////////////////////////////////////////////////////////////////////////
{
    SoReplacedElement::init(state);

    whatKind  = EXPLICIT;
    funcCB    = NULL;
    numCoords = 0;
    coords3   = NULL;
    coords4   = NULL;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Tell shapes to generate their own texture coordinates.  This is
//    the default.
//
// Use: public

void
SoTextureCoordinate3Element::setDefault(SoState *state, SoNode *node)
//
////////////////////////////////////////////////////////////////////////
{
    SoTextureCoordinate3Element	*elt;

    elt = (SoTextureCoordinate3Element *)
            getElement(state, classStackIndex, node);

    if (elt != NULL) {
        elt->whatKind = EXPLICIT;
        elt->numCoords = 0;
        elt->coords3 = NULL;
        elt->coords4 = NULL;
    }
    //The shapeStyle element will track this value:
    SoShapeStyleElement::setTextureFunction(state, FALSE);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Sets the current coords, given a function to compute them.
//
// Use: public

void
SoTextureCoordinate3Element::setFunction(SoState *state, SoNode *node,
                                         SoTextureCoordinateFunctionCB *func,
                                         void *userData)
//
////////////////////////////////////////////////////////////////////////
{
    SoTextureCoordinate3Element	*elt;

    elt = (SoTextureCoordinate3Element *) getElement(state,
                                                     classStackIndex, node);
    if (elt != NULL) {
        elt->whatKind = FUNCTION;
        elt->funcCB = func;
        elt->funcCBData = userData;
    }
    //The shapeStyle element will track this value:
    SoShapeStyleElement::setTextureFunction(state, TRUE);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Sets the current coords, given a set of 2D texture coordinates.
//
// Use: public

void
SoTextureCoordinate3Element::set3(SoState *state, SoNode *node,
                                  int32_t numCoords, const SbVec3f *coords)
//
////////////////////////////////////////////////////////////////////////
{
    SoTextureCoordinate3Element	*elt;

    elt = (SoTextureCoordinate3Element *)
            getElement(state, classStackIndex, node);

    if (elt != NULL) {
        elt->whatKind    = EXPLICIT;
        elt->numCoords   = numCoords;
        elt->coords3     = coords;
        elt->coordsAre3D = TRUE;
    }
    SoShapeStyleElement::setTextureFunction(state, FALSE);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Sets the current coords, given a set of 4D texture coordinates.
//
// Use: public

void
SoTextureCoordinate3Element::set4(SoState *state, SoNode *node,
                                  int32_t numCoords, const SbVec4f *coords)
//
////////////////////////////////////////////////////////////////////////
{
    SoTextureCoordinate3Element	*elt;

    elt = (SoTextureCoordinate3Element *)
            getElement(state, classStackIndex, node);

    if (elt != NULL) {
        elt->whatKind    = EXPLICIT;
        elt->numCoords   = numCoords;
        elt->coords4     = coords;
        elt->coordsAre3D = FALSE;
    }
    SoShapeStyleElement::setTextureFunction(state, FALSE);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Returns code depending on which set routine was called.
//
// Use: public, static

SoTextureCoordinate3Element::CoordType
SoTextureCoordinate3Element::getType(SoState *state)
//
////////////////////////////////////////////////////////////////////////
{
    return getInstance(state)->getType();
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Non-static (virtual) version of above method.
//
// Use: public

SoTextureCoordinate3Element::CoordType
SoTextureCoordinate3Element::getType() const
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

const SoTextureCoordinate3Element *
SoTextureCoordinate3Element::getInstance(SoState *state)
//
////////////////////////////////////////////////////////////////////////
{
    return (const SoTextureCoordinate3Element *)
            getConstElement(state, classStackIndex);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Given a point and a normal, call the registered callback to get
//    corresponding texture coordinates:
//
// Use: public

const SbVec4f &
SoTextureCoordinate3Element::get(const SbVec3f &point,
                                 const SbVec3f &normal) const
//
////////////////////////////////////////////////////////////////////////
{
#ifdef DEBUG
    if (whatKind != FUNCTION)
        SoDebugError::post("SoTextureCoordinate3Element::get",
                           "Functional texture coordinates were not set!");
#endif /* DEBUG */
    return (*funcCB)(funcCBData, point, normal);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Returns the indexed coordinate from an element as an SbVec2f.
//
// Use: public

const SbVec3f &
SoTextureCoordinate3Element::get3(int index) const
//
////////////////////////////////////////////////////////////////////////
{
#ifdef DEBUG
    if (whatKind != EXPLICIT)
        SoDebugError::post("SoTextureCoordinate3Element::get3",
                           "Explicit texture coordinates were not set!");

    if (index < 0 || index >= numCoords)
        SoDebugError::post("SoTextureCoordinate3Element::get3",
                           "Index (%d) is out of range 0 - %d",
                           index, numCoords - 1);
#endif /* DEBUG */

    if (coordsAre3D)
        return coords3[index];

    // Convert from 4-D if necessary
    else {
        // Cast the const away...
        SoTextureCoordinate3Element *elt = (SoTextureCoordinate3Element *) this;
        const SbVec4f		   &c4  = coords4[index];

        // If we can't do the projection, or we don't have to
        if (c4[3] == 0.0 || c4[3] == 1.0) {
            elt->convert3[0] = c4[0];
            elt->convert3[1] = c4[1];
            elt->convert3[2] = c4[2];
        }

        else {
            elt->convert3[0] = c4[0] / c4[3];
            elt->convert3[1] = c4[1] / c4[3];
            elt->convert3[2] = c4[2] / c4[3];
        }

        return convert3;
    }
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Returns the indexed coordinate from an element as an SbVec4f.
//
// Use: public

const SbVec4f &
SoTextureCoordinate3Element::get4(int index) const
//
////////////////////////////////////////////////////////////////////////
{
#ifdef DEBUG
    if (whatKind != EXPLICIT)
        SoDebugError::post("SoTextureCoordinate3Element::get4",
                           "Explicit texture coordinates were not set!");

    if (index < 0 || index >= numCoords)
        SoDebugError::post("SoTextureCoordinate3Element::get4",
                           "Index (%d) is out of range 0 - %d",
                           index, numCoords - 1);
#endif /* DEBUG */

    // Convert from 2-D if necessary
    if (coordsAre3D) {
        // Cast the const away...
        SoTextureCoordinate3Element *elt = (SoTextureCoordinate3Element *) this;
        const SbVec3f		   &c3  = coords3[index];

        elt->convert4[0] = c3[0];
        elt->convert4[1] = c3[1];
        elt->convert4[2] = c3[2];
        elt->convert4[3] = 1.0;

        return convert4;
    }

    else
        return coords4[index];
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
SoTextureCoordinate3Element::print(FILE *fp) const
{
    SoReplacedElement::print(fp);
}
#else  /* DEBUG */
void
SoTextureCoordinate3Element::print(FILE *) const
{
}
#endif /* DEBUG */

