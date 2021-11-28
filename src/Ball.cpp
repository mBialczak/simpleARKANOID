#include "Ball.hpp"

// constructor
Ball::Ball(
    float X, float Y, float directionAngle, float speed, const Texture& texture)
    : _position(gMath::Vector2d(X, Y))
    , _direction(directionAngle)
    , _speed(speed)
    // set velocity vector: create unit velocity vector and multiply by speed
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
