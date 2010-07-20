#ifndef _SO_TO_VRML2_ACTION_
#define _SO_TO_VRML2_ACTION_

#include <Inventor/actions/SoSubAction.h>
#include <Inventor/actions/SoAction.h>

class SoVRMLGroup;
class INVENTOR_API SoToVRML2Action : public SoAction {
    SO_ACTION_HEADER(SoToVRML2Action);
public:
    SoToVRML2Action();

    SoVRMLGroup *getVRML2SceneGraph() const;
    void reuseAppearanceNodes(SbBool appearance);
    void reusePropertyNodes(SbBool property);
    void reuseGeometryNodes(SbBool geometry);

SoINTERNAL public:
    static void		initClass();
};

#endif /* _SO_TO_VRML2_ACTION_ */
