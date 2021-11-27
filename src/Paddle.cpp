#include "Paddle.hpp"

// constructor
Paddle::Paddle(float X, float Y, float Speed, const Texture& texture)
    : _position(gMath::Vector2d(X, Y))
    , _speed(Speed)
    , _velocity(gMath::Vector2d())
    , _texture(texture)
{
}

// render the paddle
void Paddle::Draw() const
{
  _texture.Render(
      static_cast<int>(_position._x), static_cast<float>(_position._y));
}