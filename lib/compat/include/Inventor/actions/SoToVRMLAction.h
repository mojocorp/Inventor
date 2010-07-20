#ifndef _SO_TO_VRML_ACTION_
#define _SO_TO_VRML_ACTION_

#include <Inventor/actions/SoSubAction.h>
#include <Inventor/actions/SoAction.h>

class SoNode;
class INVENTOR_API SoToVRMLAction : public SoAction {
    SO_ACTION_HEADER(SoToVRMLAction);
public:
    SoToVRMLAction();

    SoNode *getVRMLSceneGraph() const;

SoINTERNAL public:
    static void		initClass();
};

#endif /* _SO_TO_VRML_ACTION_ */

