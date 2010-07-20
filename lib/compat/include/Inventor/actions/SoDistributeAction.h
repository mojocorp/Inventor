#ifndef _SO_DISTRIBUTE_ACTION_
#define _SO_DISTRIBUTE_ACTION_

#include <Inventor/actions/SoSubAction.h>
#include <Inventor/actions/SoAction.h>

class INVENTOR_API SoDistributeAction : public SoAction {
    SO_ACTION_HEADER(SoDistributeAction);
public:
    SoDistributeAction();
    virtual ~SoDistributeAction();

SoINTERNAL public:
    static void		initClass();
};

#endif /* _SO_DISTRIBUTE_ACTION_ */
