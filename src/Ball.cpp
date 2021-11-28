#include "Ball.hpp"

// constructor:
// X, Y - ball centre coordinates
// directionAngle - direction in degrees in which the ball starts moving
// speed - ball sclar speed in pixels / second
// texture - texture used for displaying the ball
Ball::Ball(
    float X, float Y, float directionAngle, float speed, const Texture& texture)
    : _position(gMath::Vector2d(X, Y))
    , _direction(directionAngle)
    , _speed(speed)
    // set velocity vector: create unit velocity vector and multiply by scalar
    // speed
    , _velocity(gMath::Vector2d(gMath::ToRadians(directionAngle)) * speed)
    , _texture(texture)
{
}

// update ball state with given time difference from last update
void Ball::Update(float deltaTime)
{
  // update position
  _position += _velocity * deltaTime;
}

// render the ball
void Ball::Draw() const
{
  _texture.Render(
      static_cast<int>(_position._x), static_cast<float>(_position._y));
}
