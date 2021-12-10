#include "Controller.hpp"
#include "SDL.h"

// Contructor taking the game object to control
Controller::Controller(Game& game)
    : _game(game)
{
}

// Handles all the input events
// Takes the paddle and ball to synchronise game control
void Controller::HandleInput(bool& running, Paddle& paddle, Ball& ball) const
{
  // get all SDL events
  SDL_Event evt;

  while (SDL_PollEvent(&evt)) {
    // handle quit event (like "close window" button)
    if (evt.type == SDL_QUIT) {
      running = false;
    }
    // dispatch the rest event handling to helpers depending on the game state
    switch (_game.State()) {
      case GameState::Routine:
        HandleRoutineEvents(paddle, ball);
        break;
      case GameState::Paused:
        HandlePausedEvents();
        break;
      case GameState::Over:
        HandleGameOverEvents();
        break;
      default:
        // report error if unknown game state encountered
        throw std::runtime_error(
            "Unknown game state passed to game input controller!");
    }
  }
}

// handles input events specific to the game paused state
void Controller::HandlePausedEvents() const
{
  // get the current keyboard state
  const Uint8* keysArray { SDL_GetKeyboardState(NULL) };
  // handle pause key presses
  if (keysArray[_pause_key]) {
    _game.TogglePause();
  }
}

// handles input events specific to the game routine running
void Controller::HandleRoutineEvents(Paddle& paddle, Ball& ball) const
{
  // get the current keyboard state
  const Uint8* keysArray { SDL_GetKeyboardState(NULL) };
  // move the paddle up when the key: _up is pressed
  if (keysArray[_up]) {
    // move paddle up only if it has not outrun the ball
    // (with some reasonable margin)
    // helps avoid visual apearance of ball overlaping the paddle
    // when ball hits the paddle but the paddle is still moving up
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
  // increase the ball speed when the key: _speed_up is pressed
  else if (keysArray[_speed_up]) {
    ball.SetSpeedDelta(_game.SpeedIncrement());
  }
  // decrease the ball speed when the key: _slow_down is pressed
  else if (keysArray[_slow_down]) {
    ball.SetSpeedDelta(-25.0f);
  }
  // start the ball from the paddle when the key: _start is pressed
  else if (keysArray[_start]) {
    // start the ball only if it is in the starting position
    if (!ball.IsMoving() && _game.State() == GameState::Routine) {
      ball.Start();
    }
  }
  // pause the game when the pause key is pressed
  else if (keysArray[_pause_key]) {
    _game.TogglePause();
  }
  // actions to be executed whenever the above keys are not pressed
  else {
    paddle.Stop();
    ball.SetSpin(Spin::sNone);
    ball.SetSpeedDelta(0.0f);
  }
}

// handles input events specific to the game routine running
void Controller::HandleGameOverEvents() const
{
  // get the current keyboard state
  const Uint8* keysArray { SDL_GetKeyboardState(NULL) };
  // restart the game if the key: _start is pressed
  if (keysArray[_restart]) {
    _game.Restart();
  }
  // quit the game if the key: _quit is pressed
  else if (keysArray[_quit]) {
    _game.Quit();
  }
}