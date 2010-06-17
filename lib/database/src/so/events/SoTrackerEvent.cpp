#include <Inventor/events/SoTrackerEvent.h>

SO_EVENT_SOURCE(SoTrackerEvent);

SoTrackerEvent::SoTrackerEvent()
{
}

SoTrackerEvent::~SoTrackerEvent()
{
}

void
SoTrackerEvent::initClass(void)
{
  SO_EVENT_INIT_CLASS(SoTrackerEvent, SoEvent);
}

void
SoTrackerEvent::setPosition3(const SbVec3f& p)
{
  this->trackerInfo.setPosition3(p);
}

const SbVec3f
SoTrackerEvent::getPosition3() const
{
  return this->trackerInfo.getPosition3();
}

void
SoTrackerEvent::setOrientation(const float& a, const float& e, const float& h)
{
  this->trackerInfo.setOrientation(a, e, h);
}

void
SoTrackerEvent::setOrientation(const SbRotation& r)
{
  this->trackerInfo.setOrientation(r);
}

void
SoTrackerEvent::setOrientation(const SbVec3f& axis, const float& angle)
{
  this->trackerInfo.setOrientation(axis, angle);
}

const SbRotation
SoTrackerEvent::getOrientation() const
{
  return this->trackerInfo.getOrientation();
}

const SbLine
SoTrackerEvent::getRay() const
{
  return this->trackerInfo.getRay();
}

const SbTrackerInfo*
SoTrackerEvent::getTrackerInfo() const
{
  return &this->trackerInfo;
}

SbTrackerInfo*
SoTrackerEvent::getTrackerInfo()
{
  return &this->trackerInfo;
}

const SbTrackerInfo&
SoTrackerEvent::getTrackerInfoRef() const
{
  return this->trackerInfo;
}
