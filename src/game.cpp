#include "game.hpp"
#include "LimitTimer.hpp"
#include "SDL.h"
#include "SDLexception.hpp"

// constructor
Game::Game(const std::size_t screenHeight, const std::size_t screenWidth,
    const std::size_t targetFrameRate)
    : screen_height(screenHeight)
    , screen_width(screenWidth)
    , frame_rate(targetFrameRate)
    , renderer(nullptr)
{
  // Initialize SDL subsystems
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    throw SDLexception(
        "Failed to initialize SDL", SDL_GetError(), __FILE__, __LINE__);
  }
  // create graphics renderer here, only after SDL is initialized
  renderer = new Renderer(screen_height, screen_width);
}

// destructor
Game::~Game()
{
  // delete graphics renderer
  delete (renderer);
  // close SDL
  SDL_Quit();
}

// implements main game loop
void Game::Run()
{
  // calculate desired duration of a single frame
  const Uint32 cSDLticksPerSecond = 1000;
  const Uint32 desired_frame_duration { cSDLticksPerSecond / frame_rate };

  // create timer used for for FPS limiting
  LimitTimer frame_timer { desired_frame_duration };
  // create timer for window title udpates
  const Uint32 cTitleUpdateTime = 1000;
  LimitTimer title_update_timer { cTitleUpdateTime };
  // frame count for FPS updating
  int frames { 0 };
  // main loop quit condition
  bool running = true;
  // main game loop
  while (running) {
    // update frame count for FPS calculation
    frames++;

    // TODO:handle input
    controller.HandleInput(running);
    // TODO: update game state

    // TODO: update display
    renderer->Display();

    // update window title bar
    if (title_update_timer.hasExpired()) {
      renderer->UpdateTitleBar(frames);
      // reset number of frames for FPS count
      frames = 0;
      // restart timer for window title udpates
      title_update_timer.restart();
    }
    // TODO: update sounds

    // execute frame FPS limiting policy by waiting untill
    // each frame time completes
    frame_timer.waitTillExpire();
    // restart frameTimer for time measure in the next frame
    frame_timer.restart();
  }
}