#ifndef Vector2d_hpp
#define Vector2d_hpp
#include "SDL_image.h"
#include <cmath>

// namespace used for mathematical calculations in the game
namespace gMath {

// mathematical constants used
constexpr float c_Pi = M_PI;
// NOTE: comment
struct Vector2d
{
  // default constructor
  Vector2d();
  // constructor taking X and Y coordinates
  Vector2d(float X, float Y);
  // constructs unit vector from the direction angle
  explicit Vector2d(float angle);
  // returns current vector multiplied by scalar
  Vector2d operator*(float scalar);
  // add inplace other vector2d
  Vector2d& operator+=(const Vector2d& other);

  // axis components
  float _x;
  float _y;
};

// Helper functions
// NOTE: remove INU
// calculates vertical distance beetween two vectors
inline float VerticalDistance(const Vector2d& A, const Vector2d& B)
{
  return fabs(B._y - A._y);
}
// NOTE: remove INU
// calculates horizontal distance beetween two vectors
inline float HorizontalDistance(const Vector2d& A, const Vector2d& B)
{
  return fabs(B._x - A._x);
}

// converts angle in degrees to angle in radians
inline float ToRadians(float degrees) { return degrees * c_Pi / 180.0f; }

} // end of gMath namespace
#endif // !Vector2d_hpp
