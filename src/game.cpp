#include "game.hpp"
#include "LimitTimer.hpp"
#include "SDL.h"
#include "SDLexception.hpp"

// constructor
Game::Game(const std::size_t screenHeight, const std::size_t screenWidth,
    const std::size_t targetFrameRate)
    : _screen_height(screenHeight)
    , _screen_width(screenWidth)
    , _frame_rate(targetFrameRate)
    , _renderer(nullptr)
{
  // Initialize SDL subsystems
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    throw SDLexception(
        "Failed to initialize SDL", SDL_GetError(), __FILE__, __LINE__);
  }
  // create graphics renderer here, only after SDL is initialized
  _renderer = new Renderer(_screen_height, _screen_width);
}

// destructor
Game::~Game()
{
  // delete graphics renderer
  delete (_renderer);
  // close SDL
  SDL_Quit();
}

// implements main game loop
void Game::Run()
{
  // calculate desired duration of a single frame
  const Uint32 c_SDL_ticks_per_second = 1000;
  const Uint32 desired_frame_duration { c_SDL_ticks_per_second / _frame_rate };

  // create timer used for for FPS limiting
  LimitTimer frame_timer { desired_frame_duration };

  // main loop quit condition
  bool running = true;
  // main game loop
  while (running) {

    // TODO:handle input
    _controller.HandleInput(running);
    // TODO: update game state

    // TODO: update display
    _renderer->Display();

    // TODO: update sounds

    // execute frame FPS limiting policy by waiting untill
    // each frame time completes
    frame_timer.waitTillExpire();
    // restart frameTimer for time measure in the next frame
    frame_timer.restart();
  }
}