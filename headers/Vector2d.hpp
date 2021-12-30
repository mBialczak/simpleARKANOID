#ifndef Vector2d_hpp
#define Vector2d_hpp
#include <cmath>

// namespace for grouping objects and functions required for the game
// calculations
namespace gMath {

// class representing a two dimmensional vector
class Vector2d
{
  public:
  // default constructor
  Vector2d();
  // constructor taking X and Y coordinates
  Vector2d(float X, float Y);
  // constructor creating unit vector from the direction angle in radians
  explicit Vector2d(float angle);

  // Returns the x component of the vector
  float X() const { return x_; }
  // Returns the y component of the vector
  float Y() const { return y_; }

  // Sets the x component of the vector
  void SetX(float x) { x_ = x; }
  // Sets the y component of the vector
  void SetY(float y) { y_ = y; }

  // returns current vector multiplied by scalar
  Vector2d operator*(float scalar);
  // adds other vector2d replacing the current value
  Vector2d& operator+=(const Vector2d& other);

  private:
  // vector axis components
  float x_;
  float y_;
};

// mathematical constants used

constexpr float c_Pi = M_PI;

// Helper functions

// calculates vertical distance beetween two vectors
inline float VerticalDistance(const Vector2d& A, const Vector2d& B)
{
  return fabs(B.Y() - A.Y());
}

// calculates horizontal distance beetween two vectors
inline float HorizontalDistance(const Vector2d& A, const Vector2d& B)
{
  return fabs(B.X() - A.X());
}

// converts angle in degrees to angle in radians
inline float ToRadians(float degrees) { return degrees * c_Pi / 180.0f; }

} // end of gMath namespace
#endif // !Vector2d_hpp
