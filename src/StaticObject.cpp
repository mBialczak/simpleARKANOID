#include "StaticObject.hpp"

// default constructor
StaticObject::StaticObject()
    : _position(gMath::Vector2d())
{
}

// constructor setting position
StaticObject::StaticObject(float x, float y)
    : _position(gMath::Vector2d(x, y))
{
}
// pure virtual function for setting position
// most classes derived can use this function as it simply sets the vector position
void StaticObject::SetPosition(float x, float y)
{
  _position = gMath::Vector2d { x, y };
}