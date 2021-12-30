#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "MovableObject.hpp"
#include "SDL.h"
#include "StaticObject.hpp"
#include <functional>
#include <memory>
#include <vector>

// Class for rendering graphics to the screen.
class Renderer
{
  public:
  // Constructor. Takes screen size for rendering.
  // Throws std::ivalid_argument if the size is non-positive
  // Throws SDLexception if initializing SDL_Renderer will fail
  Renderer(const std::size_t screenHeight, const std::size_t screenWidth);

  // copying of class object doesn't make sense and crucial resources are
  // managed with unique pointers, so copy operations are disabled
  Renderer(const Renderer&) = delete;
  Renderer& operator=(const Renderer&) = delete;

  // All resources are managed automatically, so default destructor is used
  ~Renderer() = default;

  // displays (renders) game graphics composed of the passed arguments:
  // a vector of static (non-movable) and a vector of movable objects
  void DisplayScreen(const std::vector<const StaticObject*>& staticObjects,
      const std::vector<const MovableObject*>& movableObjects) const;

  // Displays a screen containing static (non-movable) objects sent as argument
  void DisplayScreen(
      const std::vector<const StaticObject*>& staticObjects) const;

  // returns a raw pointer to SDL renderer, which can be used if needed with SDL
  // functions. User of the class should not explicitly destroy or modify the
  // returned pointer as it is owned by Renderer class.
  SDL_Renderer* GetSDLrenderer() const { return sdl_renderer_.get(); }

  private:
  // updates game window title bar
  void UpdateTitleBar() const;

  // game window size
  const std::size_t screen_height_;
  const std::size_t screen_width_;

  // unique pointer to SDL_Window using a custom window deleter.
  std::unique_ptr<SDL_Window, std::function<void(SDL_Window*)>> sdl_window_;
  // unique pointer to SDL_renderer using a custom renderer deleter
  std::unique_ptr<SDL_Renderer, std::function<void(SDL_Renderer*)>>
      sdl_renderer_;
};
#endif // !RENDERER_HPP