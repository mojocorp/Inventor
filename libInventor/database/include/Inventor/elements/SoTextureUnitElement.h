#ifndef  _SO_TEXTURE_UNIT_ELEMENT_
#define  _SO_TEXTURE_UNIT_ELEMENT_

#include <Inventor/elements/SoInt32Element.h>

//////////////////////////////////////////////////////////////////////////////
//
//  Class: SoTextureUnitElement
//
//  Element that stores the current texture unit.
//
//////////////////////////////////////////////////////////////////////////////

SoEXTENDER class INVENTOR_API SoTextureUnitElement : public SoInt32Element
{
    SO_ELEMENT_HEADER(SoTextureUnitElement);

public:
    /// Initializes element
    virtual void        init(SoState *state);

    /// Sets the current texture units in the state
    static void         set(SoState *state, int32_t unit)
        { SoInt32Element::set(classStackIndex, state, unit); }

    /// Returns current texture unit from the state
    static int32_t      get(SoState *state)
        { return SoInt32Element::get(classStackIndex, state); }

    /// Return the number of texture units avaible for the current context.
    static uint32_t getMaxTextureUnit(SoState *state);

SoINTERNAL public:

    /// Initializes the SoTextureUnitElement class
    static void         initClass();

protected:
    virtual ~SoTextureUnitElement();
};

#endif /* _SO_TEXTURE_UNIT_ELEMENT_ */
