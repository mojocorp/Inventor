#ifndef _SO_GET_PRIMITIVE_COUNT_ACTION_
#define _SO_GET_PRIMITIVE_COUNT_ACTION_

#include <Inventor/actions/SoSubAction.h>
#include <Inventor/actions/SoAction.h>

class INVENTOR_API SoGetPrimitiveCountAction : public SoAction {
    SO_ACTION_HEADER(SoGetPrimitiveCountAction);
public:
    SoGetPrimitiveCountAction();
    virtual ~SoGetPrimitiveCountAction();

SoINTERNAL public:
    static void		initClass();
};

#endif /* _SO_GET_PRIMITIVE_COUNT_ACTION_ */
