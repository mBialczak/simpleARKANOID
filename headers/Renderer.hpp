#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "Ball.hpp"
#include "Paddle.hpp"
#include "SDL.h"
#include "SideWall.hpp"
#include "TextElement.hpp" // REVIEW:
#include <memory>
#include <vector>

// REVIEW: check what can be deleted
// class forward declarations
class Ball;
class Paddle;
class SideWall;
class TextElement;

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

  // displays game screen filled with game objects
  void DisplayGameScreen(const std::vector<const StaticObject*>& staticObjects,
      const std::vector<const MovableObject*>& movableObjects) const;
  // REMOVE INU , COMMENT if not
  // Displays a screen containing static objects
  void DisplayStaticScreen(
      const std::vector<const StaticObject*>& staticObjects) const;

  //  REMOVE INU
  // // Displays single TextElement on the screen
  // void Display(const TextElement& textElement) const;

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
};
#endif // !RENDERER_HPP