#include "Controller.hpp"
#include "SDL.h"
#include "gMath.hpp"

// function for handling all the input events
void Controller::HandleInput(bool& running, Paddle& paddle) const
{
  SDL_Event evt;
  // get all SDL events
  while (SDL_PollEvent(&evt)) {
    // handle quit evt
    if (evt.type == SDL_QUIT) {
      running = false;
    }
  }
  // get all the pressed keyboard keys and handle them
  HandleKeyPresses(evt, SDL_GetKeyboardState(NULL), paddle);
}

// helper function for  handling keypresses
void Controller::HandleKeyPresses(
    SDL_Event& evt, const Uint8* keysArray, Paddle& paddle) const
{
  // move the paddle accordingly to pressed keys
  if (keysArray[_up]) {
    paddle.MoveUp();
  }
  else if (keysArray[_down]) {
    paddle.MoveDown();
  }
  else if (keysArray[_left]) {
    paddle.MoveLeft();
  }
  else if (keysArray[_right]) {
    paddle.MoveRight();
  }
  else {
    paddle.Stop();
  }
}