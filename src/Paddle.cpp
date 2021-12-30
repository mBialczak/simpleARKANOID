#include "Paddle.hpp"

// Constructor:
// X, Y - paddle centre coordinates
// speed - ball scalar speed in pixels / second
// moveLimits - invisible rectangle within the paddle can move
// texture - texture used for displaying the ball.
// Throws std::invalid_argument if passed values don't allow to create the
// Paddle
Paddle::Paddle(
    float x, float y, float speed, SDL_Rect moveLimits, const Texture& texture)
    : MovableObject(x, y, speed)
    , _velocity(gMath::Vector2d())
    , _move_limits(moveLimits)
    , _texture(texture)
    , _half_height(_texture.Height() / 2.0f)
    , _half_width(_texture.Width() / 2.0f)
{
  // check if the passed arguments allow to construct the paddle with reasonable
  // values. Throw exception if not, as continuation makes no sense.
  if (x < moveLimits.x || y < moveLimits.y || speed < 0.0f)
    throw std::invalid_argument(
        "Invalid arguments passed to the Paddle constructor");
}

// update paddle state
void Paddle::Update(float deltaTime)
{
  // update position
  position_ += _velocity * deltaTime;
  // correct if position went out of the allowed moving range
  KeepInMovingLimits();
};

// render the paddle
void Paddle::Draw() const
{
  _texture.Render(
      static_cast<int>(position_.X()), static_cast<int>(position_.Y()));
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
  if (position_.X() - _half_width < _move_limits.x) {
    position_.SetX(_move_limits.x + _half_width + 1);
    return;
  }
  // move left if reached the right limit
  if (position_.X() + _half_width > _move_limits.x + _move_limits.w) {
    position_.SetX(_move_limits.x + _move_limits.w - _half_width - 1);
    return;
  }
  // move down if reached the the top limit
  if (position_.Y() - _half_height < _move_limits.y) {
    position_.SetY(_move_limits.y + _half_height + 1);
    return;
  }
  // move up if reached the bottom limit
  if (position_.Y() + _half_height > _move_limits.y + _move_limits.h) {
    position_.SetY(_move_limits.y + _move_limits.h - _half_height - 1);
    return;
  }
}