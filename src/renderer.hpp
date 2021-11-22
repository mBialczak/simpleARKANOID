#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "SDL.h"

// class for rendering graphics
class Renderer {
  public:
  // constructor
  Renderer(const std::size_t screenHeight, const std::size_t screenWidth);
  ~Renderer();

  void Display() const;
  void UpdateTitleBar(int frames) const;

  private:
  // update title bar

  SDL_Window* sdl_window;
  SDL_Renderer* sdl_renderer;

  // game window size
  const std::size_t screen_height;
  const std::size_t screen_width;
};
#endif // !RENDERER_HPP