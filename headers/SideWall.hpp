#ifndef SideWall_HPP
#define SideWall_HPP
#include "Enums.hpp"
#include "SDL.h"
#include "StaticObject.hpp"
#include "Texture.hpp"

// forward declaration
class Texture;

// class representing border walls in the game
class SideWall : public virtual StaticObject
{
  public:
  // constructor creating the Wall
  SideWall(float x, float y, ScreenSide sideOfScreen, const Texture& texture,
      float halfTickness);
  // default virtual constructor
  ~SideWall() override = default;
  // renders the sideWall to the screen
  void Draw() const override;

  // returns the side of the screen where the wall is placed
  ScreenSide GetScreenSide() const { return screen_side_; }
  // returns half of the wall tickness
  float HalfTickness() const { return half_tickness_; }

  private:
  // Side of the screen being placed
  const ScreenSide screen_side_;
  // texture representing the SideWall
  const Texture& texture_;
  // half of the wall tickness;
  // storing spares some calculations during collision detection
  float half_tickness_;
};
#endif // !SideWall_HPP