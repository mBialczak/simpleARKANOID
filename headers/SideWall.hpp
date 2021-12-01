#ifndef SideWall_HPP
#define SideWall_HPP
#include "SDL.h"
#include "StaticObject.hpp"
#include "Texture.hpp"

// TODO: consider if should inherit from static object or move the code

// class forward declarations
class Texture;
enum class ScreenSide
{
  Top,
  Right,
  Left
};
// class representing border walls in the game
class SideWall : public virtual StaticObject
{
  public:
  // enum class for marking which side of the ball is crucial for collision
  // detection

  // constructor creating the Wall
  SideWall(float x, float y, ScreenSide sideOfScreen, const Texture& texture);
  // sets object position ; overrides StaticObject function
  void SetPosition(float x, float y) override;
  // render the sideWall
  void Draw() const;
  // reuturns collision border
  ScreenSide GetScreenSide() const { return _screen_side; }

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