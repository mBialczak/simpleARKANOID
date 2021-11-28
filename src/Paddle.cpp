#include "Paddle.hpp"

// constructor
Paddle::Paddle(float X, float Y, float Speed, const Texture& texture)
    : _position(gMath::Vector2d(X, Y))
    , _speed(Speed)
    , _velocity(gMath::Vector2d())
    , _texture(texture)
{
}

// update paddle state
void Paddle::Update(float deltaTime)
{ // update position
  _position += _velocity * deltaTime;
};

// render the paddle
void Paddle::Draw() const
{
  _texture.Render(
      static_cast<int>(_position._x), static_cast<float>(_position._y));
}

// stops the paddle by setting velocity vector to {0,0}
void Paddle::Stop()
{
  _velocity._x = 0;
  _velocity._y = 0;
}

// enables moving up by proper setting of the paddle velocity vector
void Paddle::MoveUp()
{
  _velocity._x = 0;
  _velocity._y = -_speed;
}

// enables moving down by proper setting of the paddle velocity vector
void Paddle::MoveDown()
{
  _velocity._x = 0;
  _velocity._y = _speed;
}

// enables moving left by proper setting of the paddle velocity vector
void Paddle::MoveLeft()
{
  _velocity._x = -_speed;
  _velocity._y = 0;
}

// enables moving right by proper setting of the paddle velocity vector
void Paddle::MoveRight()
{
  _velocity._x = _speed;
  _velocity._y = 0;
}