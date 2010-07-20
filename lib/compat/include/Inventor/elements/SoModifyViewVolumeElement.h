#ifndef  _SO_MODIFY_VIEW_VOLUME_ELEMENT
#define  _SO_MODIFY_VIEW_VOLUME_ELEMENT

#include <Inventor/SbVec4f.h>
#include <Inventor/SbBox3f.h>
#include <Inventor/elements/SoSubElement.h>

//////////////////////////////////////////////////////////////////////////////
//
//  Class: SoModifyViewVolumeElement
//
//  
//
//////////////////////////////////////////////////////////////////////////////

SoEXTENDER class INVENTOR_API SoModifyViewVolumeElement : public SoElement {

    SO_ELEMENT_HEADER(SoModifyViewVolumeElement);

public:
    virtual SbBool matches(const SoElement *elt) const;

    /// Copy method, takes care copying the data.
    virtual SoElement *copyMatchInfo() const;

    /// Initializes element
    virtual void    init(SoState *state);

    /// Sets the view volume portion in the state
    static void     set(SoState *state, const SbVec4f &portion);
    static void     set(SoState *state, const SbBox3f &portion);

    /// Returns the current view volume portion from the state
    static const SbVec4f & get(SoState *state);
    static const SbBox3f & getBox(SoState *state);

    /// Prints element (for debugging)
    virtual void    print(FILE *fp) const;

    /// Returns the top (current) instance of the element in the state
    static const SoModifyViewVolumeElement * getInstance(SoState *state, SbBool &elemIsSet);

SoINTERNAL public:
    static void     initClass();

protected:
    SbBox3f screenPortionBox;
    SbVec4f screenPortion;

    virtual ~SoModifyViewVolumeElement();
};

#endif /* _SO_MODIFY_VIEW_VOLUME_ELEMENT */
