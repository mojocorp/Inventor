#ifndef SO_PROJECTION_ELEMENT_H
#define SO_PROJECTION_ELEMENT_H

#include <Inventor/elements/SoReplacedElement.h>

class SoProjection;
class SbProjection;

class INVENTOR_API SoProjectionElement : public SoReplacedElement
{
    SO_ELEMENT_HEADER( SoProjectionElement );

public:
    /// Initializes element
    virtual void    init(SoState *state);

    static void set( SoState *state, SoProjection *prj );
    static void set( SoState *state, SoNode *, SoProjection *prj );

    static SoProjection *get( SoState *state );
SoINTERNAL public:
    static void     initClass();
protected:
    SoProjection *projection;

    virtual ~SoProjectionElement();
};

#endif
