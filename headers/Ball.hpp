#ifndef Ball_HPP
#define Ball_HPP
#include "Texture.hpp"
#include "Vector2d.hpp"

// Texture forward declaration
class Texture;

class Ball {
  public:
  // constructor
  Ball(float X, float Y, const Texture& texture);

  // render the ball
  void Render() const;

  private:
  // ball position
  Vector2d _position;
  // texture used for displaying the ball
  const Texture& _texture;
};

#endif // !Ball_HPP