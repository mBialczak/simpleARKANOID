#ifndef SideWall_HPP
#define SideWall_HPP
#include "Enums.hpp"
#include "SDL.h"
#include "StaticObject.hpp"
#include "Texture.hpp"

// TODO: consider if should inherit from static object or move the code

// class forward declarations
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

  // returns collision border
  ScreenSide GetScreenSide() const { return _screen_side; }
  // returns half of the wall tickness
  float HalfTickness() const { return _half_tickness; }

  private:
  // collision side of the SideWall
  const ScreenSide _screen_side;
  // texture representing the SideWall
  const Texture& _texture;
  // half of the wall tickness;
  // storing spares some calculations during collision detection
  float _half_tickness;
};
#endif // !SideWall_HPP