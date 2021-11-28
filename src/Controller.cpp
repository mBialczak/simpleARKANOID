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
    SDL_Event& evt, const Uint8* KeysArray, Paddle& paddle) const
{
  // move the paddle accordingly to pressed keys
  if (KeysArray[_up]) {
    paddle.MoveUp();
  }
  else if (KeysArray[_down]) {
    paddle.MoveDown();
  }
  else if (KeysArray[_left]) {
    paddle.MoveLeft();
  }
  else if (KeysArray[_right]) {
    paddle.MoveRight();
  }
  else {
    paddle.Stop();
  }
}