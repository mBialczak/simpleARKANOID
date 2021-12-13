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
  // menaged with unique pointers, so copy operations are disabled
  Renderer(const Renderer&) = delete;
  Renderer& operator=(const Renderer&) = delete;

  // All resources are managed automatically, so default destructor is used
  ~Renderer() = default;

  // displays (renders) game graphics composed of the passed arguments:
  // a vector of static (non-movable) and a vector of movable objects
  void DisplayGameScreen(const std::vector<const StaticObject*>& staticObjects,
      const std::vector<const MovableObject*>& movableObjects) const;

  // Displays a screen containing static (non-movable) objects sent as argument
  void DisplayStaticScreen(
      const std::vector<const StaticObject*>& staticObjects) const;

  // returns a raw pointer to SDL renderer, which can be used if needed with SDL
  // functions. User of the class should not explicitly destroy or modify the
  // returned pointer as it is owned by Renderer class.
  SDL_Renderer* GetSDLrenderer() const { return _sdl_renderer.get(); }

  private:
  // updates game window title bar
  void UpdateTitleBar() const;

  // game window size
  const std::size_t _screen_height;
  const std::size_t _screen_width;

  // unique pointer to SDL_Window using a custom window deleter.
  std::unique_ptr<SDL_Window, std::function<void(SDL_Window*)>> _sdl_window;
  // unique pointer to SDL_renderer using a custom renderer deleter
  std::unique_ptr<SDL_Renderer, std::function<void(SDL_Renderer*)>>
      _sdl_renderer;
};
#endif // !RENDERER_HPP