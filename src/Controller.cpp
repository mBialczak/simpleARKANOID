#include "Controller.hpp"
#include "SDL.h"

// function for handling all the input events
void Controller::HandleInput(bool& running, Paddle& paddle, Ball& ball) const
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
  HandleKeyPresses(evt, SDL_GetKeyboardState(NULL), paddle, ball);
}

// helper function for  handling keypresses
void Controller::HandleKeyPresses(
    SDL_Event& evt, const Uint8* keysArray, Paddle& paddle, Ball& ball) const
{
  // move the paddle accordingly to pressed keys
  if (keysArray[_up]) {
    // REVIEW: verify if needed
    // move up only if paddle has not outrun the ball with some reasonable
    // margin
    if (gMath::VerticalDistance(paddle.Position(), ball.Position())
        > paddle.HalfHeight() + 5 * ball.Radius()) {
      paddle.MoveUp();
    }
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
  else if (keysArray[_start_pause]) {
    // start the ball if it is in the starting position
    if (!ball.IsMoving()) {
      ball.Start();
    }
  }
  else {
    paddle.Stop();
  }
}