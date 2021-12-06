#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "Ball.hpp"
#include "Paddle.hpp"
#include "SDL.h"
#include "SideWall.hpp"
#include <memory>
#include <vector>

// REVIEW: check what can be deleted
// class forward declarations
class Ball;
class Paddle;
class SideWall;

// class for rendering graphics
class Renderer
{
  public:
  // constructor takes screen size for rendering
  Renderer(const std::size_t screenHeight, const std::size_t screenWidth);

  // copying of class object doesn't make sense and copy operations may cause
  // problems with managed resources, so copy operations are disabled
  Renderer(const Renderer&) = delete;
  Renderer& operator=(const Renderer&) = delete;
  // destructor
  ~Renderer();
  // adds StaticObject to be displayed
  void AddStaticObject(const StaticObject* object);
  // adds MovableObject to be displayed
  void AddMovableObject(const MovableObject* object);
  // displays (renders) game graphics
  void Display() const;
  // REMOVE INU , COMMENT if not
  void Display(std::string message) const;

  // returns the owned SDL renderer
  // user of the class should not explicitly destroy the renderer
  SDL_Renderer* GetSDLrenderer() const { return _sdl_renderer; }

  private:
  // update title bar
  void UpdateTitleBar() const;

  // OWNED pointer to SDL window used for displaying
  SDL_Window* _sdl_window;
  // OWNED pointer to renderer used for rendering
  SDL_Renderer* _sdl_renderer;

  // game window size
  const std::size_t _screen_height;
  const std::size_t _screen_width;

  // vector of NOT OWNED StaticObjects to display
  std::vector<const StaticObject*> _static_objects;
  // vector of NOT OWNED MovableObjects to display
  std::vector<const MovableObject*> _movable_objects;
};
#endif // !RENDERER_HPP