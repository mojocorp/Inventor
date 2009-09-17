#include <Inventor/elements/SoGLPolygonOffsetElement.h>
#include <Inventor/misc/SoGL.h>

SO_ELEMENT_SOURCE(SoGLPolygonOffsetElement);

SoGLPolygonOffsetElement::~SoGLPolygonOffsetElement()
{
}

void
SoGLPolygonOffsetElement::initClass()
{
   SO_ELEMENT_INIT_CLASS(SoGLPolygonOffsetElement, SoPolygonOffsetElement);
}

void
SoGLPolygonOffsetElement::init(SoState *state)
{
   SoPolygonOffsetElement::init(state);
   
   copiedFromParent= FALSE;
   
   // Save the state.
   state = state;

   currentstyles       = (Style)0;
   currentoffsetfactor = 1.0f;
   currentoffsetunits  = 1.0f;
}

void
SoGLPolygonOffsetElement::push(SoState *state)
{
   const SoGLPolygonOffsetElement *prevElt = (const SoGLPolygonOffsetElement*)getNextInStack();

   offsetfactor = prevElt->offsetfactor;
   offsetunits  = prevElt->offsetunits;
   style        = prevElt->style;
   active       = prevElt->active;
   
   currentstyles= prevElt->currentstyles;
   currentoffsetfactor= prevElt->currentoffsetfactor;
   currentoffsetunits= prevElt->currentoffsetunits;
   
   copiedFromParent= TRUE;
   state= state;
}

void
SoGLPolygonOffsetElement::pop(SoState *state, const SoElement *childElt)
{
   // Since popping this element has GL side effects, make sure any
   // open caches capture it.  We may not send any GL commands, but
   // the cache dependency must exist even if we don't send any GL
   // commands, because if the element changes, the _lack_ of GL
   // commands here is a bug (remember, GL commands issued here are
   // put inside the cache).
   capture(state);
   copiedFromParent = FALSE;
   
   // If the previous element didn't have the same value...
   const SoGLPolygonOffsetElement *child = (const SoGLPolygonOffsetElement*)childElt;

   currentstyles       = child->currentstyles;
   currentoffsetfactor = child->currentoffsetfactor;
   currentoffsetunits  = child->currentoffsetunits;

   if(*this != *child)
      send();
}

void
SoGLPolygonOffsetElement::setElt(float _factor, float _units, Style _styles, SbBool _on)
{
   // Optimization:  on push, we copy the value from the previous
   // element.  If the element is set to the same value, we don't
   // bother sending it, but do add a cache dependency on the
   // previous element.

   if(   style        != _styles
      || active       != _on
      || offsetfactor != _factor
      || offsetunits  != _units
   ) {
       SoPolygonOffsetElement::setElt(_factor, _units, _styles, _on);
       send();
       copiedFromParent = FALSE;
   }
   else if(copiedFromParent) {
       SoGLPolygonOffsetElement *parent = (SoGLPolygonOffsetElement *)getNextInStack();
       parent->capture(state);
   }
}

void
SoGLPolygonOffsetElement::send()
{
   if(active) {
      if(style & FILLED) {
         if(!(currentstyles & FILLED))
            glEnable(GL_POLYGON_OFFSET_FILL);
      }
      else {
         if(currentstyles & FILLED)
            glDisable(GL_POLYGON_OFFSET_FILL);
      }
      if(style & LINES) {
         if(!(currentstyles & LINES))
            glEnable(GL_POLYGON_OFFSET_LINE);
      }
      else {
         if(currentstyles & LINES)
            glDisable(GL_POLYGON_OFFSET_LINE);
      }
      if(style & POINTS) {
         if(!(currentstyles & POINTS))
            glEnable(GL_POLYGON_OFFSET_POINT);
      }
      else {
         if(currentstyles & POINTS)
            glDisable(GL_POLYGON_OFFSET_POINT);
      }

      glPolygonOffset(offsetfactor, offsetunits);

      currentstyles = style;
   }
   else { // ! active
      if(currentstyles & FILLED)
         glDisable(GL_POLYGON_OFFSET_FILL);
      if(currentstyles & LINES)
         glDisable(GL_POLYGON_OFFSET_LINE);
      if(currentstyles & POINTS)
         glDisable(GL_POLYGON_OFFSET_POINT);

      currentstyles = (Style)0;
   }  

   // update current offset values before returning
   currentoffsetfactor = offsetfactor;
   currentoffsetunits  = offsetunits;
}

int
operator ==(const SoGLPolygonOffsetElement &e1, const SoGLPolygonOffsetElement &e2)
{
   return(e1.style               == e2.style
       && e1.active              == e2.active
       && e1.offsetfactor        == e2.offsetfactor
       && e1.offsetunits         == e2.offsetunits

       && e1.currentstyles       == e2.currentstyles
       && e1.currentoffsetfactor == e2.currentoffsetfactor
       && e1.currentoffsetunits  == e2.currentoffsetunits);
}
