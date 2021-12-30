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
  // Constructor. Takes: x and y coordinates of the blocks center, texture for
  // drawing and number of points the player will be awarded after the block is
  // destroyed. Throws std::ivalid_argument if the position coordinates or point
  // value are negative
  Block(float x, float y, const Texture& texture, unsigned pointValue);
  // default virtual destructor
  ~Block() override = default;
  // renders the block to the screen
  void Draw() const override;
  // returns half of the width of the block
  float HalfWidth() const { return half_width_; };
  // returns half of the height of the block
  float HalfHeight() const { return half_height_; };
  // checks if the block has not been destroyed
  bool IsDestroyed() const { return destroyed_; }
  // sets the block state to destroyed
  void MarkDestroyed() { destroyed_ = true; }
  // returns points value assigned to the block
  unsigned Points() const { return point_value_; }

  private:
  // texture representing the ball
  const Texture& texture_;
  // half of the block width; spares calculation while collision detection
  float half_width_;
  // half of the block height; spares calculation while collision detection
  float half_height_;
  // indicates if the block should be displayed and affects game
  bool destroyed_ = false;
  // point value assigned to the block
  unsigned point_value_;
};
#endif // !Block_HPP
