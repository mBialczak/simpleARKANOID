#ifndef Paddle_hpp
#define Paddle_hpp
#include "Texture.hpp"
#include "Vector2d.hpp"
// #include "gMath.hpp" // NOTE: remove INU

// Texture forward declaration
class Texture;

// the movable paddle
class Paddle
{
  public:
  // constructor:
  // X, Y - paddle centre coordinates
  // speed - ball sclar speed in pixels / second
  // moveLimits - rectangle within the paddle can move
  // texture - texture used for displaying the ball
  Paddle(float X, float Y, float speed, SDL_Rect moveLimits,
      const Texture& texture);
  // update paddle state
  void Update(float deltaTime);
  // render the paddle
  void Draw() const;

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
  // returns tcurrent y coordinate of the top paddle border
  float TopBorderY() const { return _position._y - _half_height; }
  // returns current y coordinate of the bottom paddle border
  float BottomBorderY() const { return _position._y + _half_height; }
  // returns current x coordinate of the left paddle border
  float LeftX() const { return _position._x - _half_width; }
  // returns current y coordinate of the right paddle border
  float RightX() const { return _position._x + _half_width; }

  private:
  // corrects position if paddle tries to escape allowed moving area
  void KeepInMovingLimits();

  // paddle position
  gMath::Vector2d _position;
  // paddle scalar speed
  float _speed;
  // paddle velocity vector
  gMath::Vector2d _velocity;
  // moving limits rectangle
  SDL_Rect _move_limits;
  // texture used for displaying the ball
  const Texture& _texture;
  // storing below values spares many repeated calculations while collision
  // detection and keeping paddle within allowed moving limits
  const float _height;
  const float _half_height;
  const float _half_width;
};
#endif // !Paddle_hpp
