#include "MovableObject.hpp"

// default constructor;
MovableObject::MovableObject()
    : _position(gMath::Vector2d())
    , _speed(0.0f)
{
}
// constructor taking position coordinates
// sets _position vector and sclar speed
MovableObject::MovableObject(float x, float y, float speed)
    : _position(gMath::Vector2d(x, y))
    , _speed(speed)
{
}

// sets object position
void MovableObject::SetPosition(float x, float y)
{
  _position = gMath::Vector2d(x, y);
}