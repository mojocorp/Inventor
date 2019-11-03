#ifndef _SO_STEREO_ELEMENT_
#define _SO_STEREO_ELEMENT_

//////////////////////////////////////////////////////////////////////////////
//
//  Class: SoStereoElement
//
//  Element that stores the current stereo parameters of the camera.
//
//////////////////////////////////////////////////////////////////////////////

#include <Inventor/SbVec.h>
#include <Inventor/elements/SoSubElement.h>

SoEXTENDER class SoStereoElement : public SoElement {

    SO_ELEMENT_HEADER(SoStereoElement);

public:
    enum StereoMode {
      MONOSCOPIC,
      LEFT_VIEW,
      RIGHT_VIEW
    };

    /// Initializes element
    virtual void	init(SoState *state);

    /// Comparison based on value of floats
    virtual SbBool matches(const SoElement *elt) const;

    /// Copy method, takes care copying the data.
    virtual SoElement *copyMatchInfo() const;

    /// Prints element (for debugging)
    virtual void print(FILE *fp) const;

    /// Sets the current stereo parameters in the state
    static void set(SoState * state, StereoMode mode, float offset = 3.0f, float balance = 1.0f);

    /// Returns current stereo parameters from the state
    static void get(SoState * state, StereoMode &mode, float &offset, float &balance);

    /// Returns each default value
    static StereoMode   getDefaultMode() { return MONOSCOPIC; }
    static float        getDefaultOffset() { return 3.0f; }
    static float        getDefaultBalace() { return 1.0f; }

    virtual void	push(SoState *state);

SoINTERNAL public:
      // Initializes the SoStereoElement class
      static void		initClass();

protected:
    virtual ~SoStereoElement();

    StereoMode      mode;
    float           offset;
    float           balance;
};

#endif /* _SO_STEREO_ELEMENT_ */
