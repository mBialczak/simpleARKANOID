#ifndef Vector2d_HPP
#define Vector2d_HPP
#include "SDL_image.h"
#include <cmath>

// namespace used for mathematical calculations in the game
namespace gMath {

// mathematical constants used
constexpr float c_Pi = M_PI;
// NOTE: comment
struct Vector2d {
  // default constructor
  Vector2d();
  // constructor taking X and Y coordinates
  Vector2d(float X, float Y);
  // constructs unit vector from the direction angle
  explicit Vector2d(float Angle);
  // returns current vector multiplied by scalar
  Vector2d operator*(float Scalar);
  // add inplace other vector2d
  Vector2d& operator+=(const Vector2d& Other);

  // axis components
  float _x;
  float _y;
};

// converts angle in degrees to angle in radians
inline float ToRadians(float Degrees)
{
  {
    return Degrees * c_Pi / 180.0f;
  };
}

} // end of gMath namespace
#endif // !Vector2d_HPP
