#include "StaticObject.hpp"

// default constructor
StaticObject::StaticObject()
    : position_(gMath::Vector2d())
{
}

// Constructor setting position. Takes x and y coordinates of the object
StaticObject::StaticObject(float x, float y)
    : position_(gMath::Vector2d(x, y))
{
}
