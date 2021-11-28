#ifndef Paddle_hpp
#define Paddle_hpp
#include "Texture.hpp"
#include "gMath.hpp"
// Texture forward declaration
class Texture;

// the movable paddle
class Paddle
{
  public:
  // constructor
  Paddle(float X, float Y, float Speed, const Texture& texture);
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
