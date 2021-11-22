#include "renderer.hpp"
#include "SDLexception.hpp"
// constructor
Renderer::Renderer(
    const std::size_t screenHeight, const std::size_t screenWidth)
    : screen_height(screenHeight)
    , screen_width(screenWidth)
{
  // create main game window
  sdl_window = SDL_CreateWindow("Simple Arkanoid game", SDL_WINDOWPOS_CENTERED,
      SDL_WINDOWPOS_CENTERED, screen_width, screen_height, SDL_WINDOW_SHOWN);

  // check if window created successfully
  if (!sdl_window) {
    throw SDLexception(
        "Could not create window", SDL_GetError(), __FILE__, __LINE__);
  }

  // create graphics renderer
  sdl_renderer = SDL_CreateRenderer(
      sdl_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

  // check if renderer created successfully
  if (!sdl_renderer) {
    throw SDLexception(
        "Could not create renderer", SDL_GetError(), __FILE__, __LINE__);
  }
}

// destructor
Renderer::~Renderer()
{
  // Deestroy graphics renderer
  SDL_DestroyRenderer(sdl_renderer);
  // Destroy main game window
  SDL_DestroyWindow(sdl_window);
}

// displays (renders) game graphics
// NOTE: helper color #00aa00
void Renderer::Display() const
{
  // Set clear screen color
  SDL_SetRenderDrawColor(sdl_renderer, 0x00, 0x00, 0x00, 0xFF);
  // clear screen
  SDL_RenderClear(sdl_renderer);

  // update screen
  SDL_RenderPresent(sdl_renderer);
}

// update title bar
void Renderer::UpdateTitleBar(int frames) const
{
  std::string title { "Simple Akranoid game! FPS: " };
  title += std::to_string(frames);
  SDL_SetWindowTitle(sdl_window, title.c_str());
}