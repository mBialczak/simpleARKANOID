#ifndef Ball_HPP
#define Ball_HPP
#include "Vector2d.hpp"

class Ball {
  public:
  // default constructor
  Ball(float X, float Y);
  
  private:
  //ball position
  Vector2d _position;
};

#endif // !Ball_HPP