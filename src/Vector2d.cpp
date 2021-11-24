#include "Vector2d.hpp"

// default constructor
Vector2d::Vector2d()
    : _x(0.0f)
    , _y(0.0f)
{
}
// constructor taking X and Y coordinates
Vector2d::Vector2d(float X, float Y)
    : _x(X)
    , _y(Y)
{
}