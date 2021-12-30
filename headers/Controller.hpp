#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP
#include "Ball.hpp"
#include "Game.hpp"
#include "Paddle.hpp"
#include "SDL.h"
#include <exception>

// class for controling input and steering from the keyboard
class Controller
{
  public:
  // Contructor taking the game object to control
  Controller(Game& game);

  // Handles all the input events
  // Takes the paddle and ball to synchronise game control
  void HandleInput(bool& running, Paddle& paddle, Ball& ball) const;

  private:
  // handles input events specific to the game paused state
  void HandlePausedEvents() const;
  // handles input events specific to the game routine running
  void HandleRoutineEvents(Paddle& paddle, Ball& ball) const;
  // handles input events specific to the game over or won
  void HandleGameOverEvents() const;

  // reference to the main game object being controlled
  Game& game_;

  // keys used for game control with  corresponding SDL codes:
  // paddle up key
  Uint8 up_ = SDL_SCANCODE_UP;
  // paddle down key
  Uint8 down_ = SDL_SCANCODE_DOWN;
  // paddle left key
  Uint8 left_ = SDL_SCANCODE_LEFT;
  // paddle right key
  Uint8 right_ = SDL_SCANCODE_RIGHT;
  // start the ball movement when on the paddle
  Uint8 start_ = SDL_SCANCODE_SPACE;
  // spin ball right key
  Uint8 spin_left_ = SDL_SCANCODE_A;
  // spin ball left key
  Uint8 spin_right_ = SDL_SCANCODE_F;
  // speed up key
  Uint8 speed_up_ = SDL_SCANCODE_E;
  // slow down key
  Uint8 slow_down_ = SDL_SCANCODE_D;
  // pause / unpause key
  Uint8 pause_key_ = SDL_SCANCODE_ESCAPE;
  // the key enabling quiting the game
  // works after game over only
  Uint8 quit_ = SDL_SCANCODE_ESCAPE;
  // the key for restarting the game
  // works after game over or won only
  Uint8 restart_ = SDL_SCANCODE_RETURN;
};
#endif // !CONTROLLER_HPP