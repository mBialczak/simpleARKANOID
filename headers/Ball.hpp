#ifndef Ball_HPP
#define Ball_HPP
#include "Texture.hpp"
#include "gMath.hpp"

// Texture forward declaration
class Texture;

class Ball
{
  public:
  // constructor:
  // X, Y - ball centre coordinates
  // directionAngle - direction in degrees in which the ball starts moving
  // speed - ball sclar speed in pixels / second
  // texture - texture used for displaying the ball
  Ball(float X, float Y, float directionAngle, float speed,
      const Texture& texture);
  // update ball state with given time difference from last update
  void Update(float deltaTime);
  // render the ball
  void Draw() const;

  private:
  // ball position
  gMath::Vector2d _position;
  // ball direction (angle in degrees)//  REVIEW: check units
  float _direction;
  // ball scalar speed
  float _speed;
  // ball vector of velocity
  gMath::Vector2d _velocity;
  // texture used for displaying the ball
  const Texture& _texture;
};

#endif // !Ball_HPP