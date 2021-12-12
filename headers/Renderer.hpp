#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "Ball.hpp"
#include "Paddle.hpp"
#include "SDL.h"
#include "SideWall.hpp"
#include "TextElement.hpp"
#include <functional>
#include <memory>
#include <vector>

// class for rendering graphics
class Renderer
{
  public:
  // Constructor. Takes screen size for rendering.
  // Throws std::ivalid_argument if the size is non-positive
  // Throws SDLexception if creating SDL_Renderer will fail
  Renderer(const std::size_t screenHeight, const std::size_t screenWidth);

  // copying of class object doesn't make sense and copy operations may cause
  // problems with managed resources, so copy operations are disabled
  Renderer(const Renderer&) = delete;
  Renderer& operator=(const Renderer&) = delete;

  // All resources are managed automatically, so default destructor used
  ~Renderer() = default;

  // displays (renders) game graphics composed of the passed arguments:
  // two vectors of objects to display
  void DisplayGameScreen(const std::vector<const StaticObject*>& staticObjects,
      const std::vector<const MovableObject*>& movableObjects) const;

  // Displays a screen containing static objects
  void DisplayStaticScreen(
      const std::vector<const StaticObject*>& staticObjects) const;

  // returns pointer to SDL renderer. User of the class should not explicitly
  // destroy the returned pointer as it is owned by Renderer class.
  SDL_Renderer* GetSDLrenderer() const { return _sdl_renderer.get(); }

  private:
  // update title bar
  void UpdateTitleBar() const;

  // game window size
  const std::size_t _screen_height;
  const std::size_t _screen_width;

  // pointer to SDL window used for displaying
  std::unique_ptr<SDL_Window, std::function<void(SDL_Window*)>> _sdl_window;
  // pointer to renderer used for rendering
  std::unique_ptr<SDL_Renderer, std::function<void(SDL_Renderer*)>>
      _sdl_renderer;
};
#endif // !RENDERER_HPP