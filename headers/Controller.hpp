#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP
#include "Paddle.hpp"
#include "SDL.h"

// class for controling input and steering
class Controller
{
  public:
  // function for handling all the input events
  // REVIEW: verify
  void HandleInput(bool& running, Paddle& paddle, Ball& ball) const;

  private:
  // helper function for  handling keypresses
  void HandleKeyPresses(
      SDL_Event& evt, const Uint8* keysArray, Paddle& paddle, Ball& ball) const;

  // key scancodes used for game control
  // paddle up
  Uint8 _up = SDL_SCANCODE_UP;
  // paddle down
  Uint8 _down = SDL_SCANCODE_DOWN;
  // paddle left
  Uint8 _left = SDL_SCANCODE_LEFT;
  // paddle right
  Uint8 _right = SDL_SCANCODE_RIGHT;
  // REVIEW: verify if the comment is right
  // start game, pause or unpause
  Uint8 _start_pause = SDL_SCANCODE_SPACE;
};
#endif // !CONTROLLER_HPP