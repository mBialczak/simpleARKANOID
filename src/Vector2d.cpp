#include "Vector2d.hpp"

// namespace for grouping objects and functions required for the game
// calculations
namespace gMath {

// default constructor
Vector2d::Vector2d()
    : x_(0.0f)
    , y_(0.0f)
{
}
// constructor taking X and Y coordinates
Vector2d::Vector2d(float x, float Y)
    : x_(x)
    , y_(Y)
{
}

// constructor creating unit vector from the direction angle in radians
Vector2d::Vector2d(float angle)
    : x_(cos(angle))
    // y coordinate needs to inverted due to SDL coordinate system
    , y_(-sin(angle))
{
}

// returns current vector multiplied by scalar
Vector2d Vector2d::operator*(float scalar)
{
  // make temporary copy to be returned
  Vector2d new_vector { *this };
  // multiply each axis value by a scalar
  new_vector.x_ *= scalar;
  new_vector.y_ *= scalar;

  return new_vector;
}

// adds other vector2d replacing the current value
Vector2d& Vector2d::operator+=(const Vector2d& other)
{
  // add member coordinates
  x_ += other.x_;
  y_ += other.y_;

  return *this;
}

} // end of gMath namespace