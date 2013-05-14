#include <Inventor/elements/SoPolygonOffsetElement.h>

SO_ELEMENT_SOURCE(SoPolygonOffsetElement);

SoPolygonOffsetElement::~SoPolygonOffsetElement()
{
}

void
SoPolygonOffsetElement::initClass()
{
   SO_ELEMENT_INIT_CLASS(SoPolygonOffsetElement, SoReplacedElement);
}

void
SoPolygonOffsetElement::init(SoState * /*state*/)
{
   SoPolygonOffsetElement::getDefault(offsetfactor, offsetunits, style, active);
}

void
SoPolygonOffsetElement::set(SoState *state, SoNode *node, float factor, float units, Style styles, SbBool on)
{
   SoPolygonOffsetElement *elem = (SoPolygonOffsetElement *)SoReplacedElement::getElement(state, classStackIndex, node);
   
   if(elem != NULL)
      elem->setElt(factor, units, styles, on);
}

void
SoPolygonOffsetElement::get(SoState *state, float &factor, float &units, Style &styles, SbBool &on)
{
   const SoPolygonOffsetElement *elem= (const SoPolygonOffsetElement *)SoElement::getConstElement(state, classStackIndex);

   factor = elem->offsetfactor;
   units  = elem->offsetunits;
   styles = elem->style;
   on     = elem->active;
}

void
SoPolygonOffsetElement::setElt(float _factor, float _units, Style _styles, SbBool _on)
{
   offsetfactor = _factor;
   offsetunits  = _units;
   style        = _styles;
   active       = _on;
}

void
SoPolygonOffsetElement::getDefault(float &_factor, float &_units, Style &_styles, SbBool &_on)
{
   _factor = 0.0f;
   _units  = 0.0f;
   _styles = SoPolygonOffsetElement::FILLED;
   _on     = FALSE;
}
