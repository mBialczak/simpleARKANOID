#include "Controller.hpp"
#include "SDL.h"

// Contructor taking the game object to control
Controller::Controller(Game& game)
    : game_(game)
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
    switch (game_.State()) {
      case GameState::Routine:
        HandleRoutineEvents(paddle, ball);
        break;
      case GameState::Paused:
        HandlePausedEvents();
        break;
      case GameState::Over:
        [[fallthrough]];
      case GameState::Won:
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
  if (keysArray[pause_key_]) {
    game_.TogglePause();
  }
}

// handles input events specific to the game routine running
void Controller::HandleRoutineEvents(Paddle& paddle, Ball& ball) const
{
  // get the current keyboard state
  const Uint8* keysArray { SDL_GetKeyboardState(NULL) };
  // move the paddle up when the key: up_ is pressed
  if (keysArray[up_]) {
    // move paddle up only if it has not outrun the ball
    // (with some reasonable margin)
    // helps avoid visual apearance of ball overlaping the paddle
    // when ball hits the paddle but the paddle is still moving up
    if (gMath::VerticalDistance(paddle.Position(), ball.Position())
        > paddle.HalfHeight() + 5 * ball.Radius()) {
      paddle.MoveUp();
    }
  }
  // move the paddle down when the key: down_ is pressed
  else if (keysArray[down_]) {
    paddle.MoveDown();
  }
  // move the paddle down when the key: left_ is pressed
  else if (keysArray[left_]) {
    paddle.MoveLeft();
  }
  // move the paddle right when the key: right_ is pressed
  else if (keysArray[right_]) {
    paddle.MoveRight();
  }
  // apply left spin when the key: spin_left_ is pressed
  else if (keysArray[spin_left_]) {
    ball.SetSpin(Spin::Left);
  }
  // apply right spin when the key: spin_right_ is pressed
  else if (keysArray[spin_right_]) {
    ball.SetSpin(Spin::Right);
  }
  // increase the ball speed when the key: speed_up_ is pressed
  else if (keysArray[speed_up_]) {
    ball.SetSpeedDelta(game_.SpeedIncrement());
  }
  // decrease the ball speed when the key: slow_down_ is pressed
  else if (keysArray[slow_down_]) {
    ball.SetSpeedDelta(-25.0f);
  }
  // start the ball from the paddle when the key: start_ is pressed
  else if (keysArray[start_]) {
    // start the ball only if it is in the starting position
    if (!ball.IsMoving() && game_.State() == GameState::Routine) {
      ball.Start();
    }
  }
  // pause the game when the pause key is pressed
  else if (keysArray[pause_key_]) {
    game_.TogglePause();
  }
  // actions to be executed whenever the above keys are not pressed
  else {
    paddle.Stop();
    ball.SetSpin(Spin::None);
    ball.SetSpeedDelta(0.0f);
  }
}

// handles input events specific to the game over or won
void Controller::HandleGameOverEvents() const
{
  // get the current keyboard state
  const Uint8* keysArray { SDL_GetKeyboardState(NULL) };
  // restart the game if the key: start_ is pressed
  if (keysArray[restart_]) {
    game_.Restart();
  }
  // quit the game if the key: quit_ is pressed
  else if (keysArray[quit_]) {
    game_.Quit();
  }
}