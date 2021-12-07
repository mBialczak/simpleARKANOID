#include "Renderer.hpp"
#include "LimitTimer.hpp"
#include "Paths.hpp" // REMOVE INU
#include "SDLexception.hpp"
#include <iostream> //REVIEW: remove after testing

// constructor takes screen size for rendering
Renderer::Renderer(
    const std::size_t screenHeight, const std::size_t screenWidth)
    : _screen_height(screenHeight)
    , _screen_width(screenWidth)
{
  // create main game window
  _sdl_window = SDL_CreateWindow("Simple Arkanoid game", SDL_WINDOWPOS_CENTERED,
      SDL_WINDOWPOS_CENTERED, _screen_width, _screen_height, SDL_WINDOW_SHOWN);

  // check if window created successfully
  if (!_sdl_window) {
    throw SDLexception(
        "Could not create window", SDL_GetError(), __FILE__, __LINE__);
  }

  // create graphics renderer
  _sdl_renderer = SDL_CreateRenderer(
      _sdl_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

  // check if renderer created successfully
  if (!_sdl_renderer) {
    throw SDLexception(
        "Could not create renderer", SDL_GetError(), __FILE__, __LINE__);
  }
}

// destructor
Renderer::~Renderer()
{
  // Deestroy graphics renderer
  SDL_DestroyRenderer(_sdl_renderer);
  _sdl_renderer = nullptr;
  // Destroy main game window
  SDL_DestroyWindow(_sdl_window);
  _sdl_window = nullptr;
}

// displays (renders) game graphics
void Renderer::DisplayGameScreen(
    const std::vector<const StaticObject*>& staticObjects,
    const std::vector<const MovableObject*>& movableObjects) const
{
  UpdateTitleBar();

  // Set clear screen color
  SDL_SetRenderDrawColor(_sdl_renderer, 0x00, 0x00, 0x00, 0xFF);
  // clear screen
  SDL_RenderClear(_sdl_renderer);

  // display all static game objects
  for (auto& object : staticObjects) {
    object->Draw();
  }

  // display all movable game objects
  for (auto& object : movableObjects) {
    object->Draw();
  }

  // update screen
  SDL_RenderPresent(_sdl_renderer);
}

// REMOVE INU , COMMENT if not
// Displays a screen containing static objects
void Renderer::DisplayStaticScreen(
    const std::vector<const StaticObject*>& staticObjects) const
{
  UpdateTitleBar();

  // Set clear screen color
  SDL_SetRenderDrawColor(_sdl_renderer, 0x00, 0x00, 0x00, 0xFF);
  // clear screen
  SDL_RenderClear(_sdl_renderer);

  // display all text objects game objects
  for (auto& object : staticObjects) {
    object->Draw();
  }

  // update screen
  SDL_RenderPresent(_sdl_renderer);
}

// REMOVE INU
// // Displays single TextElement on the screen
// void Renderer::Display(const TextElement& textElement) const
// {
//   // Set clear screen color
//   SDL_SetRenderDrawColor(_sdl_renderer, 0x00, 0x00, 0x00, 0xFF);
//   // clear screen
//   SDL_RenderClear(_sdl_renderer);

//   // textElement._texture

//   // update screen
//   SDL_RenderPresent(_sdl_renderer);
// }

// update title bar
void Renderer::UpdateTitleBar() const
{
  // create timer for window title udpates
  // the title is to be updated every second (1000 ticks)
  const Uint32 c_title_update_time = 1000;
  static LimitTimer title_update_timer { c_title_update_time };
  // frame count for FPS updating
  static int frames { 0 };

  // update frame count for FPS calculation
  frames++;
  // if 1s has passed, update window title bar
  if (title_update_timer.hasExpired()) {
    // create and display title to be updated every second
    std::string title { "Simple Akranoid game! Have fun!\t\t\tFPS: " };
    title += std::to_string(frames);
    SDL_SetWindowTitle(_sdl_window, title.c_str());

    // reset number of frames for another run
    frames = 0;
    // restart timer for window title udpates
    title_update_timer.restart();
  }
}
