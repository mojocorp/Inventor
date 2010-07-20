#ifndef _SO_BACKGROUND_
#define _SO_BACKGROUND_

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/nodes/SoNode.h>

class INVENTOR_API SoBackground : public SoNode {
    SO_NODE_ABSTRACT_HEADER(SoBackground);
public:

SoINTERNAL public:
    static void		initClass();

protected:
    SoBackground();
    virtual ~SoBackground();
};

#endif /* _SO_BACKGROUND_ */
