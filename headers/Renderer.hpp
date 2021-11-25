#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "Ball.hpp"
#include "SDL.h"

// Ball forward declaration
class Ball;

// class for rendering graphics
class Renderer {
  public:
  // constructor
  Renderer(const std::size_t screenHeight, const std::size_t screenWidth);

  // copying of class object doesn't make sense and copy operations may cause
  // problems with managed resources, so copy operations are disabled
  Renderer(const Renderer&) = delete;
  Renderer& operator=(const Renderer&) = delete;
  // destructor
  ~Renderer();
  // sets member ball pointer to be rendered
  void SetBall(const Ball* const Ball) { _ball = Ball; }

  void Display() const;

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

  // NOT owned pointer to the ball being displayed
  const Ball* _ball;
};
#endif // !RENDERER_HPP