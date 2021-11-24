#ifndef Vector2d_HPP
#define Vector2d_HPP
#include "SDL_image.h"

struct Vector2d {
  // default constructor
  Vector2d();
  // constructor taking X and Y coordinates
  Vector2d(float X, float Y);
  // axis components
  float _x;
  float _y;
};

#endif // !Vector2d_HPP
