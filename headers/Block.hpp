#ifndef Block_HPP
#define Block_HPP
#include "StaticObject.hpp"
#include "Texture.hpp"

// class forward declarations
class Texture;
// class representing a block which can be destroyed by the ball
class Block : public virtual StaticObject
{
  public:
  // constructor
  Block(float x, float y, const Texture& texture);
  // default virtual destructor
  ~Block() override = default;
  // renders the block to the screen
  void Draw() const override;
  // returns half of the width of the block
  float HalfWidth() const { return _half_width; };
  // returns half of the height of the block
  float HalfHeight() const { return _half_height; };
  // checks if the block has not been destroyed
  bool IsDestroyed() const { return _destroyed; }
  // sets the block state to destroyed
  void MakeDestroyed() { _destroyed = true; }
  // returns points value assigned to the block
  unsigned Points() const { return _point_value; }

  private:
  // texture representing the ball
  const Texture& _texture;
  // half of the block width; spares calculation while collision detection
  // REVIEW: remove INU
  float _half_width;
  // half of the block height; spares calculation while collision detection
  // REVIEW: remove INU
  float _half_height;
  // indicates if the block should be displayed and affects game
  bool _destroyed = false;
  // point value assigned to the block
  unsigned _point_value = 10;
};
#endif // !Block_HPP
