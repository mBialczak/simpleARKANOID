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
  // constructor creating the Wall
  SideWall(float x, float y, const Texture& texture);
  // sets object position ; overrides StaticObject function
  void SetPosition(float x, float y) override;
  // render the sideWall
  void Draw() const;

  private:
  // texture representing the Wall
  const Texture& _texture;
};
#endif // !SideWall_HPP