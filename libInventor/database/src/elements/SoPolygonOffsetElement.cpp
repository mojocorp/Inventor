#include <Inventor/elements/SoPolygonOffsetElement.h>

SO_ELEMENT_SOURCE(SoPolygonOffsetElement);

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Destructor.
//
// Use: private

SoPolygonOffsetElement::~SoPolygonOffsetElement()
//
////////////////////////////////////////////////////////////////////////
{
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Initializes SoPolygonOffsetElement class.
//
// Use: internal

void
SoPolygonOffsetElement::initClass()
//
////////////////////////////////////////////////////////////////////////
{
   SO_ELEMENT_INIT_CLASS(SoPolygonOffsetElement, SoReplacedElement);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Initializes element
//
// Use: public

void
SoPolygonOffsetElement::init(SoState * /*state*/)
//
////////////////////////////////////////////////////////////////////////
{
   SoPolygonOffsetElement::getDefault(offsetfactor, offsetunits, style, active);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Set element's value.
//
// Use: public, static

void
SoPolygonOffsetElement::set(SoState *state, SoNode *node, float factor, float units, Style styles, SbBool on)
//
////////////////////////////////////////////////////////////////////////
{
   SoPolygonOffsetElement *elem = (SoPolygonOffsetElement *)SoReplacedElement::getElement(state, classStackIndex, node);
   
   if(elem != NULL)
      elem->setElt(factor, units, styles, on);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Returns element values from state
//
// Use: public

void
SoPolygonOffsetElement::get(SoState *state, float &factor, float &units, Style &styles, SbBool &on)
//
////////////////////////////////////////////////////////////////////////
{
   const SoPolygonOffsetElement *elem= (const SoPolygonOffsetElement *)SoElement::getConstElement(state, classStackIndex);

   factor = elem->offsetfactor;
   units  = elem->offsetunits;
   styles = elem->style;
   on     = elem->active;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Sets polygon offset in element to given values.
//
// Use: protected, virtual

void
SoPolygonOffsetElement::setElt(float _factor, float _units, Style _styles, SbBool _on)
//
////////////////////////////////////////////////////////////////////////
{
   offsetfactor = _factor;
   offsetunits  = _units;
   style        = _styles;
   active       = _on;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Returns element default values
//
// Use: public

void
SoPolygonOffsetElement::getDefault(float &_factor, float &_units, Style &_styles, SbBool &_on)
//
////////////////////////////////////////////////////////////////////////
{
   _factor = 0.0f;
   _units  = 0.0f;
   _styles = SoPolygonOffsetElement::FILLED;
   _on     = FALSE;
}
