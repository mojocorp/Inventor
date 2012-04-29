#include <Inventor/elements/SoTransparencyTypeElement.h>
#include <Inventor/elements/SoShapeStyleElement.h>
#include <Inventor/elements/SoTransparencyTypeElement.h>

SO_ELEMENT_SOURCE(SoTransparencyTypeElement);

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Destructor.
//
// Use: private

SoTransparencyTypeElement::~SoTransparencyTypeElement()
//
////////////////////////////////////////////////////////////////////////
{
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Initializes element
//
// Use: public

void
SoTransparencyTypeElement::init(SoState *)
//
////////////////////////////////////////////////////////////////////////
{
    data = getDefault();
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Initializes SoTransparencyTypeElement class.
//
// Use: internal
void
SoTransparencyTypeElement::initClass()
//
////////////////////////////////////////////////////////////////////////
{
    SO_ELEMENT_INIT_CLASS(SoTransparencyTypeElement, SoInt32Element);
}

///////////////////////////////////////////////////////////////////////
//
// Description
// set the current transparency in the state
//
void
SoTransparencyTypeElement::set(SoState *state, TransparencyType type)
{
    SoInt32Element::set(classStackIndex, state, (int32_t)type);

    // Set the transparency bit in the ShapeStyle element
    // and the lazy element.
    SoShapeStyleElement::setTransparencyType(state,type);
    SoLazyElement::setTransparencyType(state, type);
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
SoTransparencyTypeElement::print(FILE *fp) const
{
    SoElement::print(fp);

    switch(data)
    {
    case SCREEN_DOOR:         fprintf(fp, "\tTransparency type = SCREEN_DOOR\n"); break;
    case ADD:                 fprintf(fp, "\tTransparency type = ADD\n"); break;
    case DELAYED_ADD:         fprintf(fp, "\tTransparency type = DELAYED_ADD\n"); break;
    case SORTED_OBJECT_ADD:   fprintf(fp, "\tTransparency type = SORTED_OBJECT_ADD\n"); break;
    case BLEND:               fprintf(fp, "\tTransparency type = BLEND\n"); break;
    case DELAYED_BLEND:       fprintf(fp, "\tTransparency type = DELAYED_BLEND\n"); break;
    case SORTED_OBJECT_BLEND: fprintf(fp, "\tTransparency type = SORTED_OBJECT_BLEND\n"); break;
    default:                  fprintf(fp, "\tTransparency type = UNKNOWN\n"); break;
    }
}
#else  /* DEBUG */
void
SoTransparencyTypeElement::print(FILE *) const
{
}
#endif /* DEBUG */
