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
    , velocity_(gMath::Vector2d())
    , move_limits_(moveLimits)
    , texture_(texture)
    , half_height_(texture_.Height() / 2.0f)
    , half_width_(texture_.Width() / 2.0f)
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
  position_ += velocity_ * deltaTime;
  // correct if position went out of the allowed moving range
  KeepInMovingLimits();
};

// render the paddle
void Paddle::Draw() const
{
  texture_.Render(
      static_cast<int>(position_.X()), static_cast<int>(position_.Y()));
}

// stops the paddle by setting velocity vector to {0,0}
void Paddle::Stop()
{
  velocity_.SetX(0.0f);
  velocity_.SetY(0.0f);
}

// enables moving up by proper setting of the paddle velocity vector
void Paddle::MoveUp()
{
  velocity_.SetX(0.0f);
  velocity_.SetY(-_speed / 2.0f);
}

// enables moving down by proper setting of the paddle velocity vector
void Paddle::MoveDown()
{
  velocity_.SetX(0.0f);
  velocity_.SetY(_speed);
}

// enables moving left by proper setting of the paddle velocity vector
void Paddle::MoveLeft()
{
  velocity_.SetX(-_speed);
  velocity_.SetY(0.0f);
}

// enables moving right by proper setting of the paddle velocity vector
void Paddle::MoveRight()
{
  velocity_.SetX(_speed);
  velocity_.SetY(0.0f);
}

// corrects position if paddle tries to escape allowed moving area
void Paddle::KeepInMovingLimits()
{
  // move right if reached the left limit
  if (position_.X() - half_width_ < move_limits_.x) {
    position_.SetX(move_limits_.x + half_width_ + 1);
    return;
  }
  // move left if reached the right limit
  if (position_.X() + half_width_ > move_limits_.x + move_limits_.w) {
    position_.SetX(move_limits_.x + move_limits_.w - half_width_ - 1);
    return;
  }
  // move down if reached the the top limit
  if (position_.Y() - half_height_ < move_limits_.y) {
    position_.SetY(move_limits_.y + half_height_ + 1);
    return;
  }
  // move up if reached the bottom limit
  if (position_.Y() + half_height_ > move_limits_.y + move_limits_.h) {
    position_.SetY(move_limits_.y + move_limits_.h - half_height_ - 1);
    return;
  }
}