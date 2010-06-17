#ifndef _SO_CONTROLLER_BUTTON_EVENT_
#define _SO_CONTROLLER_BUTTON_EVENT_

#include <Inventor/events/SoButtonEvent.h>
#include <Inventor/events/SbTrackerInfo.h>

#define SO_CONTROLLER_PRESS_EVENT(EVENT,BUTTON) \
    (SoControllerButtonEvent::isButtonPressEvent(EVENT,SoControllerButtonEvent::BUTTON))

#define SO_CONTROLLER_RELEASE_EVENT(EVENT,BUTTON) \
    (SoControllerButtonEvent::isButtonReleaseEvent(EVENT,SoControllerButtonEvent::BUTTON))

class INVENTOR_API SoControllerButtonEvent : public SoButtonEvent
{
    SO_EVENT_HEADER();
public:

    enum Button {
        ANY     = 0,
        BUTTON1 = 1,
        BUTTON2 = 2,
        BUTTON3 = 3
    };

    /// Constructor
    SoControllerButtonEvent();

    /// Destructor
    virtual ~SoControllerButtonEvent();

    void setButton(const SoControllerButtonEvent::Button& b);
    SoControllerButtonEvent::Button getButton() const;

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

    static SbBool isButtonPressEvent(const SoEvent* e,
                                     SoControllerButtonEvent::Button b);

    static SbBool isButtonReleaseEvent(const SoEvent* e,
                                       SoControllerButtonEvent::Button b);
SoINTERNAL public:
    static void         initClass();
private:
    Button button;
    SbTrackerInfo trackerInfo;
};

#endif /* _SO_CONTROLLER_BUTTON_EVENT_ */
