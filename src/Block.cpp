#include "Block.hpp"

// constructor
// takes position of the Block and texture to represnt
Block::Block(float x, float y, const Texture& texture)
    : StaticObject(x, y)
    , _texture(texture)
    , _half_width(_texture.Width() / 2.0f)
    , _half_height(_texture.Height() / 2.0f)
{
}

// renders the block to the screen
void Block::Draw() const
{
  _texture.Render(
      static_cast<int>(_position._x), static_cast<float>(_position._y));
}