#include "Block.hpp"

// Constructor. Takes: x and y coordinates, texture for drawing and number of
// points the player will be awarded after the block is destroyed. Throws
// std::ivalid_argument if the position coordinates or point value are
// negative
Block::Block(float x, float y, const Texture& texture, unsigned pointValue)
    : StaticObject(x, y)
    , texture_(texture)
    , half_width_(texture_.Width() / 2.0f)
    , half_height_(texture_.Height() / 2.0f)
    , point_value_(pointValue)
{
  // make sure that the invariant is correctly established and report error if
  // not
  if (x < 0.0f || y < 0.0f || point_value_ <= 0) {
    throw std::invalid_argument(
        "Invalid argument passed to the Block constructor");
  }
}

// renders the block to the screen
void Block::Draw() const
{
  // we display the block if it has not been destroyed
  if (!destroyed_) {
    texture_.Render(
        static_cast<int>(position_.X()), static_cast<int>(position_.Y()));
  }
}