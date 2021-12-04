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
  // move the paddle up when the key: _up is pressed
  if (keysArray[_up]) {
    // move up only if paddle has not outrun the ball with some reasonable
    // margin
    if (gMath::VerticalDistance(paddle.Position(), ball.Position())
        > paddle.HalfHeight() + 5 * ball.Radius()) {
      paddle.MoveUp();
    }
  }
  // move the paddle down when the key: _down is pressed
  else if (keysArray[_down]) {
    paddle.MoveDown();
  }
  // move the paddle down when the key: _left is pressed
  else if (keysArray[_left]) {
    paddle.MoveLeft();
  }
  // move the paddle right when the key: _right is pressed
  else if (keysArray[_right]) {
    paddle.MoveRight();
  }
  // apply left spin when the key: _spin_left is pressed
  else if (keysArray[_spin_left]) {
    ball.SetSpin(Spin::sLeft);
  }
  // apply right spin when the key: _spin_right is pressed
  else if (keysArray[_spin_right]) {
    ball.SetSpin(Spin::sRight);
  }
  // speed up by increasing the speed delta of the ball
  // note: the increase is applied every time the ball hits the paddle
  else if (keysArray[_speed_up]) {
    ball.SetSpeedDelta(25.0f);
  }
  // slow down by setting the speed delta of the ball to negative value
  // note: the increase is applied every time the ball hits the paddle
  else if (keysArray[_slow_down]) {
    ball.SetSpeedDelta(-25.0f);
  }
  // VERIFY: start the ball or pause / unpause the game
  else if (keysArray[_start_pause]) {
    // start the ball if it is in the starting position
    if (!ball.IsMoving()) {
      ball.Start();
    }
  }
  // actions to be executed whenever the above keys are not pressed
  else {
    paddle.Stop();
    ball.SetSpin(Spin::sNone);
    ball.SetSpeedDelta(0.0f);
  }
}