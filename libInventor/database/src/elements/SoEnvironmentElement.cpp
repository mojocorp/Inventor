#include <Inventor/elements/SoEnvironmentElement.h>
#include <Inventor/elements/SoLightAttenuationElement.h>

SO_ELEMENT_SOURCE(SoEnvironmentElement);

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Destructor.
//
// Use: private

SoEnvironmentElement::~SoEnvironmentElement()
//
////////////////////////////////////////////////////////////////////////
{
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Initializes SoEnvironmentElement class.
//
// Use: internal

void
SoEnvironmentElement::initClass()
//
////////////////////////////////////////////////////////////////////////
{
   SO_ELEMENT_INIT_CLASS(SoEnvironmentElement, SoReplacedElement);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Initializes element
//
// Use: public

void
SoEnvironmentElement::init(SoState * /*state*/)
//
////////////////////////////////////////////////////////////////////////
{
   SoEnvironmentElement::getDefault(ambientIntensity, ambientColor, attenuation,
                                    fogType, fogColor, fogVisibility);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Set element's value.
//
// Use: public, static

void
SoEnvironmentElement::set(SoState *state, SoNode *node,
                          float _ambientIntensity,
                          const SbColor &_ambientColor,
                          const SbVec3f &_attenuation,
                          int32_t _fogType,
                          const SbColor &_fogColor,
                          float _fogVisibility)
//
////////////////////////////////////////////////////////////////////////
{
   SoEnvironmentElement *elem = (SoEnvironmentElement *)SoReplacedElement::getElement(state, classStackIndex, node);
   
   if(elem != NULL)
      elem->setElt(_ambientIntensity, _ambientColor, _attenuation,
                   _fogType, _fogColor, _fogVisibility);

   //////////////////////
   //
   // Set up light attenuation. This is stored in the
   // SoLightAttenuationElement, which is then accessed by subsequent
   // light sources.
   //
   SoLightAttenuationElement::set(state, node, _attenuation);
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Returns element values from state
//
// Use: public

void
SoEnvironmentElement::get(SoState *state,
                          float &_ambientIntensity,
                          SbColor &_ambientColor,
                          SbVec3f &_attenuation,
                          int32_t &_fogType,
                          SbColor &_fogColor,
                          float &_fogVisibility)
//
////////////////////////////////////////////////////////////////////////
{
   const SoEnvironmentElement *elem= (const SoEnvironmentElement *)SoElement::getConstElement(state, classStackIndex);

   _ambientIntensity = elem->ambientIntensity;
   _ambientColor = elem->ambientColor;
   _attenuation = elem->attenuation;
   _fogType = elem->fogType;
   _fogColor = elem->fogColor;
   _fogVisibility = elem->fogVisibility;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Sets environment in element to given values.
//
// Use: protected, virtual

void
SoEnvironmentElement::setElt(float _ambientIntensity,
                             const SbColor &_ambientColor,
                             const SbVec3f &_attenuation,
                             int32_t _fogType,
                             const SbColor &_fogColor,
                             float _fogVisibility)
//
////////////////////////////////////////////////////////////////////////
{
    ambientIntensity = _ambientIntensity;
    ambientColor = _ambientColor;
    attenuation = _attenuation;
    fogType = _fogType;
    fogColor = _fogColor;
    fogVisibility = _fogVisibility;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Returns element default values
//
// Use: public

void
SoEnvironmentElement::getDefault(float &_ambientIntensity,
                                 SbColor &_ambientColor,
                                 SbVec3f &_attenuation,
                                 int32_t &_fogType,
                                 SbColor &_fogColor,
                                 float &_fogVisibility)
//
////////////////////////////////////////////////////////////////////////
{
    _ambientIntensity = 0.2f;
    _ambientColor = SbColor(1.0f, 1.0f, 1.0f);
    _attenuation = SbVec3f(0.0f, 0.0f, 1.0f);
    _fogType = NONE;
    _fogColor = SbColor(1.0f, 1.0f, 1.0f);
    _fogVisibility = 0.0f;
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//    Computes fog density based on visibility.
//
// Use: private

float
SoEnvironmentElement::computeDensity(float visibility, SbBool squared)
//
////////////////////////////////////////////////////////////////////////
{
    //
    // We want nearly total opacity at a distance of "visibility" from
    // the eye. The exponential GL fog function is:
    //
    //		f = e ** (-density * distance)
    //
    // (the exponent is squared in the SMOKE case)
    //
    // Since this function approaches 0 asymptotically, we have to
    // choose a reasonable cutoff point that approximates total
    // opacity. e ** -4 is about 0.018, so all we have to do is make
    // the exponent equal to -4 at a distance of "visibility".
    //

    if (squared) {
        return 2.0f / visibility;
    } else {
        return 4.0f / visibility;
    }
}
