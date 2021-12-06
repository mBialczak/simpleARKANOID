#include "Block.hpp"

// constructor
// takes: x and y coordinates, texture for drawing and point value of block
Block::Block(float x, float y, const Texture& texture, unsigned pointValue)
    : StaticObject(x, y)
    , _texture(texture)
    , _half_width(_texture.Width() / 2.0f)
    , _half_height(_texture.Height() / 2.0f)
    , _point_value(pointValue)
{
}

// renders the block to the screen
void Block::Draw() const
{
  // we display the block if it has not been destroyed
  if (!_destroyed) {
    _texture.Render(
        static_cast<int>(_position._x), static_cast<float>(_position._y));
  }
}