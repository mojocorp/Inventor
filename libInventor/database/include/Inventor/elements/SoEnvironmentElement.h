#ifndef  _SO_ENVIRONMENT_ELEMENT_
#define  _SO_ENVIRONMENT_ELEMENT_

#include <Inventor/elements/SoReplacedElement.h>
#include <Inventor/SbColor.h>
#include <Inventor/SbVec3f.h>

//////////////////////////////////////////////////////////////////////////////
//
//  Class: SoEnvironmentElement
//
//  Element that stores the current environment.
//
//  This class allows read-only access to the top element in the state
//  to make accessing several values in it more efficient. Individual
//  values must be accessed through this instance.
//
//////////////////////////////////////////////////////////////////////////////

SoEXTENDER class INVENTOR_API SoEnvironmentElement : public SoReplacedElement
{
   SO_ELEMENT_HEADER(SoEnvironmentElement);

public:

      /// Type of fog:
      enum FogType {
          NONE,
          HAZE,
          FOG,
          SMOKE
      };

      /// Initializes element
      virtual void init(SoState * state);

      /// Sets the current environment
      static void set(SoState *state, SoNode *node,
                      float ambientIntensity,
                      const SbColor &ambientColor,
                      const SbVec3f &attenuation,
                      int32_t fogType,
                      const SbColor &fogColor,
                      float fogVisibility);

      /// Returns the environment from an element
      static void get(SoState *state,
                      float &ambientIntensity,
                      SbColor &ambientColor,
                      SbVec3f &attenuation,
                      int32_t &fogType,
                      SbColor &fogColor,
                      float &fogVisibility);
      
      /// Returns the default environment.
      static void getDefault(float &ambientIntensity,
                             SbColor &ambientColor,
                             SbVec3f &attenuation,
                             int32_t &fogType,
                             SbColor &fogColor,
                             float &fogVisibility);

protected:

      float 	ambientIntensity;
      SbColor 	ambientColor;
      SbVec3f 	attenuation;
      int32_t 	fogType;
      SbColor 	fogColor;
      float 	fogVisibility;

      // Destructor.
      virtual ~SoEnvironmentElement();

      virtual void setElt(float ambientIntensity,
                          const SbColor &ambientColor,
                          const SbVec3f &attenuation,
                          const int32_t fogType,
                          const SbColor &fogColor,
                          float fogVisibility);

      // Computes fog density based on visibility
      static float	computeDensity(float visibility, SbBool squared);

SoINTERNAL public:

      // Initializes the SoEnvironmentElement class.
      static void initClass();
};

#endif  // _SO_ENVIRONMENT_ELEMENT_
