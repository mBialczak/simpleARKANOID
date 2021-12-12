#include "StaticObject.hpp"

// default constructor
StaticObject::StaticObject()
    : _position(gMath::Vector2d())
{
}

// Constructor setting position. Takes x and y coordinates of the object
StaticObject::StaticObject(float x, float y)
    : _position(gMath::Vector2d(x, y))
{
}
