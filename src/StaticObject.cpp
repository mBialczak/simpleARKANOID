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
