#ifndef _SO_GL_ENVIRONMENT_ELEMENT
#define _SO_GL_ENVIRONMENT_ELEMENT

#include <Inventor/elements/SoEnvironmentElement.h>

//////////////////////////////////////////////////////////////////////////////
//
//  Class: SoGLEnvironmentElement
//
//  Element that stores the current environment in GL. Overrides the
//  virtual methods on SoEnvironmentElement to send the environment
//  to GL when necessary.
//
//  Note that this class relies on SoEnvironmentElement to store the
//  values in the instance.
//
//////////////////////////////////////////////////////////////////////////////

SoEXTENDER class INVENTOR_API SoGLEnvironmentElement : public SoEnvironmentElement
{
    SO_ELEMENT_HEADER(SoGLEnvironmentElement);

public:

    /// Initializes element
    virtual void init(SoState *state);

    /// Override push() so we can remember what values the
    /// previous element set, and not send it again if it is the same.
    virtual void push(SoState *state);

    /// Override pop() method so side effects can occur in GL
    virtual void pop(SoState *state, const SoElement *childElt);

SoINTERNAL public:

    // Initializes the SoGLEnvironmentElement class.
    static void initClass();

protected:

    // Sets the environment in an instance. Has GL side effects.
    virtual void setElt(float ambientIntensity,
                        const SbColor &ambientColor,
                        const SbVec3f &attenuation,
                        const int32_t fogType,
                        const SbColor &fogColor,
                        float fogVisibility);

    // Destructor.
    virtual ~SoGLEnvironmentElement();

private:

    // Sends environment in element to GL
    void send();
};

#endif  /* _SO_GL_ENVIRONMENT_ELEMENT */
