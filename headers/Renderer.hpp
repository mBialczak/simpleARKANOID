#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "Ball.hpp"
#include "Paddle.hpp"
#include "SDL.h"
#include "SideWall.hpp"
#include <memory>
#include <vector>

// class forward declarations
class Ball;
class Paddle;
class SideWall;

// class for rendering graphics
class Renderer
{
  public:
  // constructor
  Renderer(const std::size_t screenHeight, const std::size_t screenWidth);

  // copying of class object doesn't make sense and copy operations may cause
  // problems with managed resources, so copy operations are disabled
  Renderer(const Renderer&) = delete;
  Renderer& operator=(const Renderer&) = delete;
  // destructor
  ~Renderer();

  //  sets pointer to walls container to be rendered
  void SetWalls(const std::vector<std::unique_ptr<SideWall>>* const wallsPtr);
  // sets member ball pointer to be rendered
  void SetBall(const Ball* const ball);
  // sets member paddle pointer to be rendered
  void SetPaddle(const Paddle* const paddle);

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

  // NOT owned pointer to the container of walls being displayed
  const std::vector<std::unique_ptr<SideWall>>* _walls_ptr;

  // NOT owned pointer to the ball being displayed
  const Ball* _ball;

  // NOT owned pointer to the paddle being displayed
  const Paddle* _paddle;
};
#endif // !RENDERER_HPP