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
      static_cast<int>(_position.X()), static_cast<int>(_position.Y()));
}

// stops the paddle by setting velocity vector to {0,0}
void Paddle::Stop()
{
  _velocity.SetX(0.0f);
  _velocity.SetY(0.0f);
}

// enables moving up by proper setting of the paddle velocity vector
void Paddle::MoveUp()
{
  _velocity.SetX(0.0f);
  _velocity.SetY(-_speed / 2.0f);
}

// enables moving down by proper setting of the paddle velocity vector
void Paddle::MoveDown()
{
  _velocity.SetX(0.0f);
  _velocity.SetY(_speed);
}

// enables moving left by proper setting of the paddle velocity vector
void Paddle::MoveLeft()
{
  _velocity.SetX(-_speed);
  _velocity.SetY(0.0f);
}

// enables moving right by proper setting of the paddle velocity vector
void Paddle::MoveRight()
{
  _velocity.SetX(_speed);
  _velocity.SetY(0.0f);
}

// corrects position if paddle tries to escape allowed moving area
void Paddle::KeepInMovingLimits()
{
  // move right if reached the left limit
  if (_position.X() - _half_width < _move_limits.x) {
    _position.SetX(_move_limits.x + _half_width + 1);
    return;
  }
  // move left if reached the right limit
  if (_position.X() + _half_width > _move_limits.x + _move_limits.w) {
    _position.SetX(_move_limits.x + _move_limits.w - _half_width - 1);
    return;
  }
  //  move down if reached the the top limit
  if (_position.Y() - _half_height < _move_limits.y) {
    _position.SetY(_move_limits.y + _half_height + 1);
    return;
  }
  // move up if reached the bottom limit
  if (_position.Y() + _half_height > _move_limits.y + _move_limits.h) {
    _position.SetY(_move_limits.y + _move_limits.h - _half_height - 1);
    return;
  }
}