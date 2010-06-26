#include <Inventor/elements/SoDepthBufferElement.h>

SO_ELEMENT_SOURCE(SoDepthBufferElement);

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Destructor.
//
// Use: private

SoDepthBufferElement::~SoDepthBufferElement()
//
////////////////////////////////////////////////////////////////////////
{
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Initializes SoDepthBufferElement class.
//
// Use: internal

void
SoDepthBufferElement::initClass()
//
////////////////////////////////////////////////////////////////////////
{
    SO_ELEMENT_INIT_CLASS(SoDepthBufferElement, SoElement);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Initializes element.
//
// Use: public

void
SoDepthBufferElement::init(SoState *)
//
////////////////////////////////////////////////////////////////////////
{
    test	= getDefaultTest();
    write	= getDefaultWrite();
    function    = getDefaultFunction();
    range       = getDefaultRange();
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//
//
// Use: public, static

void
SoDepthBufferElement::set(SoState * state, SbBool test, SbBool write,
                          DepthWriteFunction function, const SbVec2f & range)
//
////////////////////////////////////////////////////////////////////////
{
    SoDepthBufferElement	*elt;

    // Get an instance we can change (pushing if necessary)
    elt = (SoDepthBufferElement *) getElement(state, classStackIndex);

    if (elt != NULL)
        elt->setElt(test, write, function, range);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Pushes the element, copying values from previous element in
//    state:
//
// Use: public, virtual

void
SoDepthBufferElement::push(SoState *)
//
////////////////////////////////////////////////////////////////////////
{
    const SoDepthBufferElement	*prevElt =
        (const SoDepthBufferElement *)getNextInStack();

    test        = prevElt->test;
    write       = prevElt->write;
    function    = prevElt->function;
    range       = prevElt->range;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Virtual set method; the GL version of this element overrides
//    this to send stuff to GL, too...
//
// Use: protected

void
SoDepthBufferElement::setElt(SbBool _test, SbBool _write,
                             DepthWriteFunction _function, const SbVec2f & _range)
//
////////////////////////////////////////////////////////////////////////
{
    test        = _test;
    write       = _write;
    function    = _function;
    range       = _range;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Returns shape hints from state.
//
// Use: public, static

void
SoDepthBufferElement::get(SoState * state, SbBool & test_out, SbBool & write_out,
                          DepthWriteFunction & function_out, SbVec2f & range_out)
//
////////////////////////////////////////////////////////////////////////
{
    const SoDepthBufferElement *elt;

    elt = (const SoDepthBufferElement *)
        getConstElement(state, classStackIndex);

    test_out        = elt->test;
    write_out       = elt->write;
    function_out    = elt->function;
    range_out       = elt->range;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Overrides this method to compare shape hints.
//
// Use: public

SbBool
SoDepthBufferElement::matches(const SoElement *elt) const
//
////////////////////////////////////////////////////////////////////////
{
    const SoDepthBufferElement *shElt = (const SoDepthBufferElement *) elt;

    return (test        == shElt->test          	&&
            write       == shElt->write 		&&
            function    == shElt->function              &&
            range       == shElt->range);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//     Create a copy of this instance suitable for calling matches()
//     on.
//
// Use: protected

SoElement *
SoDepthBufferElement::copyMatchInfo() const
//
////////////////////////////////////////////////////////////////////////
{
    SoDepthBufferElement *result =
        (SoDepthBufferElement *)getTypeId().createInstance();

    result->test	= test;
    result->write	= write;
    result->function    = function;
    result->range	= range;

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
SoDepthBufferElement::print(FILE *fp) const
{
    const char *functionName;

    SoElement::print(fp);

    switch (function) {
    case NEVER:     functionName = "NEVER"; break;
    case ALWAYS:    functionName = "ALWAYS"; break;
    case LESS:      functionName = "LESS"; break;
    case LEQUAL:    functionName = "LEQUAL"; break;
    case EQUAL:     functionName = "EQUAL"; break;
    case GEQUAL:    functionName = "GEQUAL"; break;
    case GREATER:   functionName = "GREATER"; break;
    case NOTEQUAL:  functionName = "NOTEQUAL"; break;
    default:	// Just to shut CC up
        break;
    }



    fprintf(fp,
            "\tDepth Buffer: test = %s, write = %s,\n"
            "\t\t     function = %s, range = [%f-%f]\n",
            test ? "TRUE" : "FALSE", write ? "TRUE" : "FALSE", functionName, range[0], range[1]);
}
#else  /* DEBUG */
void
SoDepthBufferElement::print(FILE *) const
{
}
#endif /* DEBUG */
