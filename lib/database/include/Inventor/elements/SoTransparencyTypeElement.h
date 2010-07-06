#ifndef  _SO_TRANSPARENCY_TYPE_ELEMENT
#define  _SO_TRANSPARENCY_TYPE_ELEMENT

//////////////////////////////////////////////////////////////////////////////
//
//  Class: SoTransparencyTypeElement
//
//  Element that stores the current transparency type.
//
//////////////////////////////////////////////////////////////////////////////

#include <Inventor/elements/SoInt32Element.h>

/// Element that stores the current transparency type.
SoEXTENDER class INVENTOR_API SoTransparencyTypeElement : public SoInt32Element {

    SO_ELEMENT_HEADER(SoTransparencyTypeElement);

  public:
    /// Various levels of transparency rendering quality
    enum TransparencyType {
        SCREEN_DOOR,                    ///< Use stipple patterns for screen-door transparency
        ADD,                            ///< Use additive alpha blending
        DELAYED_ADD,                    ///< Uses additive blending, rendering all transparent objects after opaque ones
        BLEND,                          ///< Uses multiplicative alpha blending
        DELAYED_BLEND,                  ///< Uses multiplicative alpha blending, rendering all transparent objects after opaque ones
        SORTED_OBJECT_ADD,              ///< Same as DELAYED_ADD, but sorts transparent objects by distances of bounding boxes from camera
        SORTED_OBJECT_BLEND,            ///< Same as DELAYED_BLEND, but sorts transparent objects by distances of bounding boxes from camera
        SORTED_LAYERS_BLEND,            ///TODO
        DELAYED_SORTED_LAYERS_BLEND,    ///TODO
        SORTED_TRIANGLES_ADD,           ///TODO
        SORTED_TRIANGLES_BLEND,         ///TODO
        SORTED_OBJECT_TRIANGLES_ADD,    ///TODO
        SORTED_OBJECT_TRIANGLES_BLEND   ///TODO
    };

    /// Initializes element
    virtual void	init(SoState *state);

    /// Sets the current transparency type in the state
    static void		set(SoState *state, TransparencyType type)
    { SoInt32Element::set(classStackIndex, state, (int32_t)type); }

    /// Returns current transparency type from the state
    static TransparencyType	get(SoState *state)
    { return (TransparencyType)SoInt32Element::get(classStackIndex, state); }

    /// Returns the default transparency type
    static TransparencyType	getDefault()			{ return SCREEN_DOOR; }

    /// Prints element (for debugging)
    virtual void	print(FILE *fp) const;

  SoINTERNAL public:
        // Initializes the SoTransparencyTypeElement class
        static void		initClass();

  protected:
    virtual ~SoTransparencyTypeElement();
};

#endif /* _SO_TRANSPARENCY_TYPE_ELEMENT */
