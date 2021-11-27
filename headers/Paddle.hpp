#ifndef Paddle_hpp
#define Paddle_hpp
#include "Texture.hpp"
#include "gMath.hpp"
// Texture forward declaration
class Texture;

// the movable paddle
class Paddle {
  public:
  // constructor
  Paddle(float X, float Y, float Speed, const Texture& texture);

  // render the paddle
  void Draw() const;

  private:
  // paddle position
  gMath::Vector2d _position;
  // paddle scalar speed
  float _speed;
  // paddle velocity vector
  gMath::Vector2d _velocity;
  // texture used for displaying the ball
  const Texture& _texture;
};
#endif // !Paddle_hpp
