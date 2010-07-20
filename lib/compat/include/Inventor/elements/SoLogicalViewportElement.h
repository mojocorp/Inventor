#ifndef  _SO_LOGICAL_VIEWPORT_ELEMENT
#define  _SO_LOGICAL_VIEWPORT_ELEMENT

#include <Inventor/SbViewportRegion.h>
#include <Inventor/elements/SoSubElement.h>

//////////////////////////////////////////////////////////////////////////////
//
//  Class: SoViewportRegionElement
//
//  Element that stores the current viewport region, which indicates
//  which part of the window is being rendered into. This information
//  is also needed during picking and bounding box computation to
//  determine sizes/locations of screen-space objects.
//
//////////////////////////////////////////////////////////////////////////////

SoEXTENDER class INVENTOR_API SoLogicalViewportElement : public SoElement {

    SO_ELEMENT_HEADER(SoLogicalViewportElement);

  public:
    /// Initializes element
    virtual void    init(SoState *state);

    /// Sets the viewport region
    static void     set(SoState *state, const SbViewportRegion &vpReg);

    /// Returns current viewport region from the state
    static const SbViewportRegion & get(SoState *state);

    /// Returns TRUE if the viewports match
    virtual SbBool  matches(const SoElement *elt) const;

    /// Create and return a copy of this element
    virtual SoElement   *copyMatchInfo() const;

    /// Prints element (for debugging)
    virtual void    print(FILE *fp) const;

  SoINTERNAL public:
    // Initializes the SoViewportRegionElement class
    static void     initClass();

  protected:
    SbViewportRegion    viewportRegion;

    virtual ~SoLogicalViewportElement();
  private:
    static SbViewportRegion defaultLogicalViewport;
};

#endif /* _SO_LOGICAL_VIEWPORT_ELEMENT */
