#include "Paddle.hpp"

// constructor:
// X, Y - paddle centre coordinates
// speed - ball sclar speed in pixels / second
// moveLimits - rectangle within the paddle can move
// texture - texture used for displaying the ball
Paddle::Paddle(
    float x, float y, float speed, SDL_Rect moveLimits, const Texture& texture)
    : MovableObject(x, y, speed)
    , _velocity(gMath::Vector2d())
    , _move_limits(moveLimits)
    , _texture(texture)
    , _half_height(_texture.Height() / 2.0f)
    , _half_width(_texture.Width() / 2.0f)
{
}

// update paddle state
void Paddle::Update(float deltaTime)
{
  // update position
  _position += _velocity * deltaTime;
  // correct if position went out of the allowed moving range
  KeepInMovingLimits();
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
  _velocity._x = 0.0;
  _velocity._y = 0.0;
}

// enables moving up by proper setting of the paddle velocity vector
void Paddle::MoveUp()
{
  _velocity._x = 0;
  _velocity._y = -_speed / 2.0;
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

// corrects position if paddle tries to escape allowed moving area
void Paddle::KeepInMovingLimits()
{
  // move right if reached the left limit
  if (_position._x - _half_width < _move_limits.x) {
    _position._x = _move_limits.x + _half_width + 1;
    return;
  }
  // move left if reached the right limit
  if (_position._x + _half_width > _move_limits.x + _move_limits.w) {
    _position._x = _move_limits.x + _move_limits.w - _half_width - 1;
    return;
  }
  //  move down if reached the the top limit
  if (_position._y - _half_height < _move_limits.y) {
    _position._y = _move_limits.y + _half_height + 1;
    return;
  }
  // move up if reached the bottom limit
  if (_position._y + _half_height > _move_limits.y + _move_limits.h) {
    _position._y = _move_limits.y + _move_limits.h - _half_height - 1;
    return;
  }
}