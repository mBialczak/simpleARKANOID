#include "SideWall.hpp"

// constructor creating the Wall
SideWall::SideWall(
    float x, float y, CollisionBorder border, const Texture& texture)
    : StaticObject(x, y)
    , _coll_border(border)
    , _texture(texture)
{
}
// sets object position ; overrides StaticObject function
void SideWall::SetPosition(float x, float y)
{
  // no special behavior, so calls base class function
  StaticObject::SetPosition(x, y);
}

// render the sideWall
void SideWall::Draw() const
{
  _texture.Render(
      static_cast<int>(_position._x), static_cast<float>(_position._y));
}