#include "SideWall.hpp"

// constructor creating the Wall
SideWall::SideWall(float x, float y, ScreenSide sideOfScreen,
    const Texture& texture, float HalfTickness)
    : StaticObject(x, y)
    , _screen_side(sideOfScreen)
    , _texture(texture)
    , _half_tickness(HalfTickness)
{
}

// renders the sideWall to the screen
void SideWall::Draw() const
{
  _texture.Render(
      static_cast<int>(_position._x), static_cast<float>(_position._y));
}