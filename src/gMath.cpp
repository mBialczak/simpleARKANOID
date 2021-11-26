#include "gMath.hpp"

#include <exception>

namespace gMath {

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

// constructs unit vector (normalized vector) from the direction angle
// NOTE: add info about angle unit
Vector2d::Vector2d(float Angle)
    : _x(cos(Angle))
    // y coordinate needs to inverted due to SDL coordinate system
    , _y(-sin(Angle))
{
}

// returns current vector multiplied by scalar
Vector2d Vector2d::operator*(float Scalar)
{
  // make temporary copy to be returned
  Vector2d new_vector { *this };
  new_vector._x *= Scalar;
  new_vector._y *= Scalar;
  return new_vector;
}

// add inplace other vector2d
Vector2d& Vector2d::operator+=(const Vector2d& Other)
{
  // add member coordinates
  _x += Other._x;
  _y += Other._y;
  return *this;
}

} // end of gMath namespace