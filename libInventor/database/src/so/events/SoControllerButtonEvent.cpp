#include <Inventor/events/SoControllerButtonEvent.h>

SO_EVENT_SOURCE(SoControllerButtonEvent);

SoControllerButtonEvent ::SoControllerButtonEvent()
  : button(ANY)
{
}

SoControllerButtonEvent::~SoControllerButtonEvent()
{
}

void
SoControllerButtonEvent::initClass(void)
{
  SO_EVENT_INIT_CLASS(SoControllerButtonEvent, SoButtonEvent);
}

void
SoControllerButtonEvent::setButton(const SoControllerButtonEvent::Button& b)
{
  this->button = b;
}

SoControllerButtonEvent::Button
SoControllerButtonEvent::getButton() const
{
  return this->button;
}

void
SoControllerButtonEvent::setPosition3(const SbVec3f& p)
{
  this->trackerInfo.setPosition3(p);
}

const SbVec3f
SoControllerButtonEvent::getPosition3() const
{
  return this->trackerInfo.getPosition3();
}

void
SoControllerButtonEvent::setOrientation(const float& a, const float& e, const float& h)
{
  this->trackerInfo.setOrientation(a, e, h);
}

void
SoControllerButtonEvent::setOrientation(const SbRotation& r)
{
  this->trackerInfo.setOrientation(r);
}

void
SoControllerButtonEvent::setOrientation(const SbVec3f& axis, const float& angle)
{
  this->trackerInfo.setOrientation(axis, angle);
}

const SbRotation
SoControllerButtonEvent::getOrientation() const
{
  return this->trackerInfo.getOrientation();
}

const SbLine
SoControllerButtonEvent::getRay() const
{
  return this->trackerInfo.getRay();
}

const SbTrackerInfo*
SoControllerButtonEvent::getTrackerInfo() const
{
  return &this->trackerInfo;
}

SbTrackerInfo*
SoControllerButtonEvent::getTrackerInfo()
{
  return &this->trackerInfo;
}

const SbTrackerInfo&
SoControllerButtonEvent::getTrackerInfoRef() const
{
  return this->trackerInfo;
}

SbBool
SoControllerButtonEvent::isButtonPressEvent(const SoEvent* e,
                                            SoControllerButtonEvent::Button whichButton)
{
  if (e->isOfType(SoControllerButtonEvent::getClassTypeId())) {
    const SoControllerButtonEvent* bev = (const SoControllerButtonEvent*)e;
    if ((bev->getState() == SoButtonEvent::DOWN) &&
        ((whichButton == -1) ||
         (whichButton == bev->getButton())))
      return TRUE;
  }
  return FALSE;
}

SbBool
SoControllerButtonEvent::isButtonReleaseEvent(const SoEvent* e,
                                              SoControllerButtonEvent::Button whichButton)
{
  if (e->isOfType(SoControllerButtonEvent::getClassTypeId())) {
    const SoControllerButtonEvent* bev = (const SoControllerButtonEvent*)e;
    if ((bev->getState() == SoButtonEvent::UP) &&
        ((whichButton == -1) ||
         (whichButton == bev->getButton())))
      return TRUE;
  }
  return FALSE;
}
