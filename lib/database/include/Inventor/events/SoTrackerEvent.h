#ifndef _SO_TRACKER_EVENT_
#define _SO_TRACKER_EVENT_

#include <Inventor/events/SoSubEvent.h>
#include <Inventor/events/SbTrackerInfo.h>

class INVENTOR_API SoTrackerEvent : public SoEvent
{
    SO_EVENT_HEADER();

public:

    SoTrackerEvent(void);
    virtual ~SoTrackerEvent();

    void setPosition3(const SbVec3f& p);
    const SbVec3f getPosition3() const;

    void setOrientation(const float& a, const float& e, const float& h);
    void setOrientation(const SbRotation& r);
    void setOrientation(const SbVec3f& axis, const float& angle);
    const SbRotation getOrientation() const;

    const SbLine getRay() const;

    virtual const SbTrackerInfo* getTrackerInfo() const;
    virtual SbTrackerInfo* getTrackerInfo();
    virtual const SbTrackerInfo & getTrackerInfoRef() const;

SoINTERNAL public:
    static void initClass(void);
private:

    SbTrackerInfo trackerInfo;
};

#endif /* _SO_TRACKER_EVENT_ */

