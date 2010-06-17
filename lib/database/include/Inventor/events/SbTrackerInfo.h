#ifndef _SB_TRACKER_INFO_
#define _SB_TRACKER_INFO_

#include <Inventor/SbVec3f.h>
#include <Inventor/SbRotation.h>
#include <Inventor/SbLine.h>

class INVENTOR_API SbTrackerInfo
{
public:

    SbTrackerInfo();
    virtual ~SbTrackerInfo();

    void setPosition3(const SbVec3f& p);
    const SbVec3f getPosition3() const;

    const SbRotation& getOrientation() const;
    void getOrientation(SbVec3f& axis, float& angle) const;

    void setOrientation(const float& q0, const float& q1,
                        const float& q2, const float& q3);
    void setOrientation(const float q[4]);
    void setOrientation(const SbVec3f& axis, const float& angle);
    void setOrientation(const SbRotation& orientation);
    void setOrientation(const float& zangle, const float& xangle,
                        const float& yangle);

    const SbLine getRay() const;

    void setData(void *data);

    const void* getData() const;

private:

    SbVec3f position3;
    SbRotation orientation;

    void* userData;
};

#endif /* _SB_TRACKER_INFO_ */

