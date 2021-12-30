#ifndef Paddle_hpp
#define Paddle_hpp

#include "MovableObject.hpp"
#include "Texture.hpp"
#include "Vector2d.hpp"

// Texture forward declaration
class Texture;

// Constructor:
// X, Y - paddle centre coordinates
// speed - ball scalar speed in pixels / second
// moveLimits - invisible rectangle within the paddle can move
// texture - texture used for displaying the ball.
// Throws std::invalid_argument if passed values don't allow to create the
// Paddle
class Paddle : public virtual MovableObject
{
  public:
  // Constructor:
  // X, Y - paddle centre coordinates
  // speed - ball scalar speed in pixels / second
  // moveLimits - invisible rectangle within the paddle can move
  // texture - texture used for displaying the ball
  Paddle(float x, float y, float speed, SDL_Rect moveLimits,
      const Texture& texture);

  // default virtual destructor
  ~Paddle() override = default;

  // update paddle state
  void Update(float deltaTime) override;
  // render the paddle
  void Draw() const override;
  // returns half of the paddle height
  float HalfHeight() const { return half_height_; }
  // returns half of the paddle width
  float HalfWidth() const { return half_width_; }
  // stops the paddle by setting velocity vector to {0,0}
  void Stop();
  // enables moving up by proper setting of the paddle velocity vector
  void MoveUp();
  // enables moving down by proper setting of the paddle velocity vector
  void MoveDown();
  // enables moving right by proper setting of the paddle velocity vector
  void MoveRight();
  // enables moving left by proper setting of the paddle velocity vector
  void MoveLeft();

  private:
  // corrects position if paddle tries to escape allowed moving area
  void KeepInMovingLimits();

  // paddle velocity vector
  gMath::Vector2d velocity_;
  // invisible moving limits rectangle
  SDL_Rect move_limits_;
  // texture used for displaying the ball
  const Texture& texture_;
  // storing below values spares many repeated calculations while collision
  // detection and process of keeping the paddle within allowed moving limits
  const float half_height_;
  const float half_width_;
};
#endif // !Paddle_hpp
