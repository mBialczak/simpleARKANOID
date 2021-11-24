#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "SDL.h"

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

  void Display() const;

  private:
  // update title bar
  void UpdateTitleBar() const;

  // owned pointers
  SDL_Window* _sdl_window;
  SDL_Renderer* _sdl_renderer;

  // game window size
  const std::size_t _screen_height;
  const std::size_t _screen_width;
};
#endif // !RENDERER_HPP