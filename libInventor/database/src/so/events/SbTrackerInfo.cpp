#include <Inventor/events/SbTrackerInfo.h>

#include <math.h>

SbTrackerInfo::SbTrackerInfo()
  : position3(0.0f, 0.0f, 0.0f),
    orientation(SbVec3f(1.0f, 0.0f, 0.0f), 0.0f),
    userData(NULL)
{
}

SbTrackerInfo::~SbTrackerInfo()
{
}

void
SbTrackerInfo::setPosition3(const SbVec3f& p)
{
  this->position3 = p;
}

const SbVec3f
SbTrackerInfo::getPosition3() const
{
  return this->position3;
}

const SbRotation&
SbTrackerInfo::getOrientation() const
{
  return this->orientation;
}

void
SbTrackerInfo::getOrientation(SbVec3f& axis, float& angle) const
{
  this->orientation.getValue(axis, angle);
}

void
SbTrackerInfo::setOrientation(const float& q0, const float& q1,
                              const float& q2, const float& q3)
{
  this->orientation.setValue(q0, q1, q2, q3);
}

void
SbTrackerInfo::setOrientation(const float q[4])
{
  this->orientation.setValue(q);
}

void
SbTrackerInfo::setOrientation(const SbVec3f& axis, const float& angle)
{
  this->orientation.setValue(axis, angle);
}

void
SbTrackerInfo::setOrientation(const SbRotation& orientation)
{
  this->orientation = orientation;
}

void
SbTrackerInfo::setOrientation(const float& xangle, const float& yangle,
                              const float& zangle)
{
  // Calculate the rotation from Euler angles.
  // Qx = sin(a/2), 0, 0, cos(a/2)
  // Qy = 0, sin(b/2), 0, cos(b/2)
  // Qz = 0, 0, sin(c,2), cos(c/2)
  // Q = Qx * Qy * Qz

  SbRotation qx(sinf(xangle/2), 0, 0, cosf(xangle/2));
  SbRotation qy(0, sinf(yangle/2), 0, cosf(yangle/2));
  SbRotation qz(0, 0, sinf(zangle/2), cosf(zangle/2));
  this->orientation = SbRotation::identity();
  this->orientation *= qz * qx * qy;
}

const SbLine
SbTrackerInfo::getRay() const
{
  SbVec3f ray(0.0f, 0.0f, -1.0f);
  this->orientation.multVec(ray, ray);
  return SbLine(this->position3, this->position3 + ray);
}

void
SbTrackerInfo::setData(void* data)
{
  this->userData = data;
}

const void*
SbTrackerInfo::getData() const
{
  return this->userData;
}
