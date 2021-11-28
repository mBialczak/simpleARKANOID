#include "Ball.hpp"

// constructor
Ball::Ball(
    float X, float Y, float DirectionAngle, float Speed, const Texture& texture)
    : _position(gMath::Vector2d(X, Y))
    , _direction(DirectionAngle)
    , _speed(Speed)
    // set velocity vector: create unit velocity vector and multiply by speed
    , _velocity(gMath::Vector2d(gMath::ToRadians(DirectionAngle)) * Speed)
    , _texture(texture)
{
}

// update ball state with given time difference from last update
void Ball::Update(float DeltaTime)
{
  // update position
  _position += _velocity * DeltaTime;
}

// render the ball
void Ball::Draw() const
{
  _texture.Render(
      static_cast<int>(_position._x), static_cast<float>(_position._y));
}
