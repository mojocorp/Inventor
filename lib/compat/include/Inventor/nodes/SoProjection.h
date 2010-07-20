#ifndef _SO_PROJECTION_
#define _SO_PROJECTION_

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/nodes/SoNode.h>
#include <Inventor/projection/SbProjection.h>
#include <Inventor/fields/SoSFInt32.h>
#include <Inventor/fields/SoSFMatrix.h>
#include <Inventor/fields/SoMFFloat.h> 
#include <Inventor/fields/SoMFVec3f.h> 

class SoState;

class INVENTOR_API SoProjection : public SoNode
{
    SO_NODE_HEADER( SoProjection );
public:
    SoProjection();

    SoMFFloat floatParameters;
    SoSFMatrix matrix;
    SoSFInt32 projTypeId;

    SbProjection* getProjection() const { return (SbProjection*)&projection; }

    static SbVec3f * checkProjection( SoState *state, SoNode *node, int32_t numCoords, const SbVec3f *coords);
    static void checkProjection( SoState* state, SbVec3f &point, SbBool inverse= FALSE);
    
    bool checkProjection( SoNode *node, SoAction *action, SoMFVec3f &coords );
    void resetProjection( SoAction *action, SoMFVec3f &coords);
SoINTERNAL public:
    static void initClass();
protected:
    virtual ~SoProjection();
private:
    SbProjection projection;
};

#endif /* _SO_PROJECTION_ */

