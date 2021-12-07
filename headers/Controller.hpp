#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP
#include "Game.hpp"
#include "Paddle.hpp"
#include "SDL.h"

// class for controling input and steering
class Controller
{
  public:
  // COMMENT; consider refactoring
  // function for handling all the input events
  void HandleInput(bool& running, Paddle& paddle, Ball& ball, Game& game) const;

  private:
  // helper function for  handling keypresses
  void HandleKeyPresses(SDL_Event& evt, const Uint8* keysArray, Paddle& paddle,
      Ball& ball, Game& game) const;

  // key scancodes (keys) used for game control
  // paddle up key
  Uint8 _up = SDL_SCANCODE_UP;
  // paddle down key
  Uint8 _down = SDL_SCANCODE_DOWN;
  // paddle left key
  Uint8 _left = SDL_SCANCODE_LEFT;
  // paddle right key
  Uint8 _right = SDL_SCANCODE_RIGHT;
  // VERIFY if the comment is right
  // start game, pause or unpause key
  Uint8 _start = SDL_SCANCODE_SPACE;
  // spin ball right key
  Uint8 _spin_left = SDL_SCANCODE_A;
  // spin ball left key
  Uint8 _spin_right = SDL_SCANCODE_F;
  // speed up key
  Uint8 _speed_up = SDL_SCANCODE_E;
  // slow down key
  Uint8 _slow_down = SDL_SCANCODE_D;
  // pause / unpause key // VERIFY
  Uint8 _pause_key = SDLK_ESCAPE;
};
#endif // !CONTROLLER_HPP