#ifndef SideWall_HPP
#define SideWall_HPP
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
  // enum class for marking which side of the ball is crucial for collision
  // detection
  enum class CollisionBorder
  {
    Bottom = 0,
    Right,
    Left
  };
  // constructor creating the Wall
  SideWall(float x, float y, CollisionBorder border, const Texture& texture);
  // sets object position ; overrides StaticObject function
  void SetPosition(float x, float y) override;
  // render the sideWall
  void Draw() const;

  private:
  // collision side of the SideWall
  const CollisionBorder _coll_border;
  // texture representing the SideWall
  const Texture& _texture;
};
#endif // !SideWall_HPP