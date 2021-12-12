#include "Block.hpp"

// Constructor. Takes: x and y coordinates, texture for drawing and number of
// points the player will be awarded after the block is destroyed. Throws
// std::ivalid_argument if the position coordinates or point value are
// negative
Block::Block(float x, float y, const Texture& texture, unsigned pointValue)
    : StaticObject(x, y)
    , _texture(texture)
    , _half_width(_texture.Width() / 2.0f)
    , _half_height(_texture.Height() / 2.0f)
    , _point_value(pointValue)
{
  // make sure that the invariant is correctly established and report error if
  // not
  if (x < 0.0f || y < 0.0f || _point_value <= 0) {
    throw std::invalid_argument(
        "Invalid argument passed to the Block constructor");
  }
}

// renders the block to the screen
void Block::Draw() const
{
  // we display the block if it has not been destroyed
  if (!_destroyed) {
    _texture.Render(
        static_cast<int>(_position.X()), static_cast<int>(_position.Y()));
  }
}