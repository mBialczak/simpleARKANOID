#include "SideWall.hpp"

// constructor creating the Wall
SideWall::SideWall(float x, float y, ScreenSide sideOfScreen,
    const Texture& texture, float HalfTickness)
    : StaticObject(x, y)
    , screen_side_(sideOfScreen)
    , texture_(texture)
    , half_tickness_(HalfTickness)
{
}

// renders the sideWall to the screen
void SideWall::Draw() const
{
  texture_.Render(
      static_cast<int>(_position.X()), static_cast<int>(_position.Y()));
}