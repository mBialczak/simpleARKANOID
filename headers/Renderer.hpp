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
  Renderer(const std::size_t screenHeight, const std::size_t screenWidth,
      const std::vector<SideWall>& sideWalls);

  // copying of class object doesn't make sense and copy operations may cause
  // problems with managed resources, so copy operations are disabled
  Renderer(const Renderer&) = delete;
  Renderer& operator=(const Renderer&) = delete;
  // destructor
  ~Renderer();
  // REVIEW: remove if general containers for objects to display work
  void SetBall(const Ball* const ball);
  // sets member paddle pointer to be rendered
  // REVIEW: remove if general containers for objects to display work
  void SetPaddle(const Paddle* const paddle);
  // adds StaticObject to be displayed //NOTE: remove INU
  void AddStaticObject(const StaticObject* object);
  // adds MovableObject to be displayed //NOTE: remove INU
  void AddMovableObject(const MovableObject* object);
  // displays (renders) game graphics
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
  // REVIEW: remove if general containers for objects to display work
  // refference to the container of walls being displayed
  const std::vector<SideWall>& _side_walls;
  // REVIEW: remove if general containers for objects to display work
  // NOT owned pointer to the ball being displayed
  const Ball* _ball;
  // REVIEW: remove if general containers for objects to display work
  // REVIEW: remove if general containers for objects to display work
  // NOT owned pointer to the paddle being displayed
  const Paddle* _paddle;
  // NOTE: remove INU
  // vector of NOT OWNED  StaticObjects to display
  std::vector<const StaticObject*> _static_objects;
  // /NOTE: remove INU
  // vector of NOT OWNED MovableObjects to display
  std::vector<const MovableObject*> _movable_objects;
};
#endif // !RENDERER_HPP