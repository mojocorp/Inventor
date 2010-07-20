#ifndef _SO_NURBS_BREP_ACTION_
#define _SO_NURBS_BREP_ACTION_

#include <Inventor/actions/SoSubAction.h>
#include <Inventor/actions/SoAction.h>

class INVENTOR_API SoNurbsBREPAction : public SoAction {
    SO_ACTION_HEADER(SoNurbsBREPAction);
public:
    SoNurbsBREPAction();
    virtual ~SoNurbsBREPAction();

SoINTERNAL public:
    static void		initClass();
};

#endif /* _SO_NURBS_BREP_ACTION_ */

