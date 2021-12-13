#include "Vector2d.hpp"

// namespace for grouping objects and functions required for the game
// calculations
namespace gMath {

// default constructor
Vector2d::Vector2d()
    : _x(0.0f)
    , _y(0.0f)
{
}
// constructor taking X and Y coordinates
Vector2d::Vector2d(float x, float Y)
    : _x(x)
    , _y(Y)
{
}

// constructor creating unit vector from the direction angle in radians
Vector2d::Vector2d(float angle)
    : _x(cos(angle))
    // y coordinate needs to inverted due to SDL coordinate system
    , _y(-sin(angle))
{
}

// returns current vector multiplied by scalar
Vector2d Vector2d::operator*(float scalar)
{
  // make temporary copy to be returned
  Vector2d new_vector { *this };
  // multiply each axis value by a scalar
  new_vector._x *= scalar;
  new_vector._y *= scalar;

  return new_vector;
}

// adds other vector2d replacing the current value
Vector2d& Vector2d::operator+=(const Vector2d& other)
{
  // add member coordinates
  _x += other._x;
  _y += other._y;

  return *this;
}

} // end of gMath namespace