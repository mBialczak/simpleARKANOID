#include "Renderer.hpp"
#include "LimitTimer.hpp"
#include "SDLexception.hpp"

// Constructor. Takes screen size for rendering.
// Throws std::ivalid_argument if the size is non-positive
// Throws SDLexception if initializing SDL_Renderer will fail
Renderer::Renderer(
    const std::size_t screenHeight, const std::size_t screenWidth)
    : screen_height_(screenHeight)
    , screen_width_(screenWidth)
    , sdl_window_(nullptr)
    , sdl_renderer_(nullptr)
{
  // Check if the screensize passed was correct and report error if not
  if (screen_height_ <= 0 || screen_width_ <= 0)
    throw std::invalid_argument("Cannot create Window with size less than 0!");

  // create main game window using unique pointer with custom deleter,
  // which will automatically destroy the window as per RAII
  sdl_window_ = std::unique_ptr<SDL_Window, std::function<void(SDL_Window*)>> {
    SDL_CreateWindow("Simple Arkanoid game", SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED, screen_width_, screen_height_,
        SDL_WINDOW_SHOWN),
    [](SDL_Window* ptr) { SDL_DestroyWindow(ptr); }
  };

  // check if window created successfully, if not - report error
  if (!sdl_window_) {
    throw SDLexception(
        "Could not create window", SDL_GetError(), __FILE__, __LINE__);
  }
  // create graphics renderer using unique pointer with custom deleter,
  // which will automatically destroy the window as per RAII
  sdl_renderer_
      = std::unique_ptr<SDL_Renderer, std::function<void(SDL_Renderer*)>> {
          SDL_CreateRenderer(sdl_window_.get(), -1,
              SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC),
          [](SDL_Renderer* ptr) { SDL_DestroyRenderer(ptr); }
        };

  // check if renderer created successfully, if not - report error
  if (!sdl_renderer_) {
    throw SDLexception(
        "Could not create renderer", SDL_GetError(), __FILE__, __LINE__);
  }
}

// displays (renders) game graphics composed of the passed arguments:
// a vector of static (non-movable) and a vector of movable objects
void Renderer::DisplayScreen(
    const std::vector<const StaticObject*>& staticObjects,
    const std::vector<const MovableObject*>& movableObjects) const
{
  UpdateTitleBar();

  // Set clear screen color
  SDL_SetRenderDrawColor(sdl_renderer_.get(), 0x00, 0x00, 0x00, 0xFF);
  // clear screen
  SDL_RenderClear(sdl_renderer_.get());

  // display all static game objects
  for (auto& object : staticObjects) {
    object->Draw();
  }

  // display all movable game objects
  for (auto& object : movableObjects) {
    object->Draw();
  }

  // update screen
  SDL_RenderPresent(sdl_renderer_.get());
}

// Displays a screen containing static (non-movable) objects sent as argument
void Renderer::DisplayScreen(
    const std::vector<const StaticObject*>& staticObjects) const
{
  UpdateTitleBar();

  // Set clear screen color
  SDL_SetRenderDrawColor(sdl_renderer_.get(), 0x00, 0x00, 0x00, 0xFF);
  // clear screen
  SDL_RenderClear(sdl_renderer_.get());

  // display all text objects game objects
  for (auto& object : staticObjects) {
    object->Draw();
  }

  // update screen
  SDL_RenderPresent(sdl_renderer_.get());
}

// updates game window title bar
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
    SDL_SetWindowTitle(sdl_window_.get(), title.c_str());

    // reset number of frames for another run
    frames = 0;
    // restart timer for window title udpates
    title_update_timer.restart();
  }
}
