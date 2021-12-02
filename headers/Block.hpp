#ifndef Block_HPP
#define Block_HPP
#include "StaticObject.hpp"
#include "Texture.hpp"

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

  private:
  // texture representing the ball
  const Texture& _texture;
  // half of the block width; spares calculation while collision detection
  // REVIEW: remove INU
  float _half_width;
  // half of the block height; spares calculation while collision detection
  // REVIEW: remove INU
  float _half_height;
};
#endif // !Block_HPP
