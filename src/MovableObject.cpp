#include "MovableObject.hpp"

// default constructor;
MovableObject::MovableObject()
    : position_(gMath::Vector2d())
    , _speed(0.0f)
{
}
// constructor taking position coordinates
// sets position_ vector and sclar speed
MovableObject::MovableObject(float x, float y, float speed)
    : position_(gMath::Vector2d(x, y))
    , _speed(speed)
{
}

// sets object position
void MovableObject::SetPosition(float x, float y)
{
  position_ = gMath::Vector2d(x, y);
}