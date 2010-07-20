#ifndef  _SO_TEXTURE_UNIT_ELEMENT_
#define  _SO_TEXTURE_UNIT_ELEMENT_

#include <Inventor/lists/SbIntList.h>
#include <Inventor/elements/SoAccumulatedElement.h>

SoEXTENDER class INVENTOR_API SoTextureUnitElement : public SoAccumulatedElement {

    SO_ELEMENT_HEADER(SoTextureUnitElement);

  public:

    /// Initializes element.
    virtual void	init(SoState *state);

    /// Add to the current texture unit list
    static void		add(SoState *state, int unit);

    /// Overrides push() method to copy values from next instance in the stack
    virtual void	push(SoState *state);

    /// Returns the current list of texture unit from the state
    static const SbIntList &	get(SoState *state);

    /// Prints element (for debugging)
    virtual void	print(FILE *fp) const;

  SoINTERNAL public:

    // Initializes the SoCoordinateElement class
    static void		initClass();

  protected:
    // list of texture units
    SbIntList	        units;

    virtual ~SoTextureUnitElement();
};

#endif /* _SO_TEXTURE_UNIT_ELEMENT_ */
