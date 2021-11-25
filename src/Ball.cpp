#include "Ball.hpp"

// constructor
Ball::Ball(float X, float Y, const Texture& texture)
    : _position(Vector2d(X, Y))
    , _texture(texture)
{
}

// render the ball
void Ball::Render() const { _texture.Render(_position._x, _position._y); }
