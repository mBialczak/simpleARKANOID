#include "Ball.hpp"
#include <iostream> // NOTE: remove after testing
// constructor:
// X, Y - ball centre coordinates
// directionAngle - direction in degrees in which the ball starts moving
// speed - ball sclar speed in pixels / second
// texture - texture used for displaying the ball
// paddle - reference to paddle against ball collision will be checked
Ball::Ball(float X, float Y, float directionAngle, float speed,
    const Texture& texture, const Paddle& paddle)
    : _position(gMath::Vector2d(X, Y))
    , _direction(directionAngle)
    , _speed(speed)
    // set velocity vector: create unit velocity vector and multiply by scalar
    // speed
    , _velocity(gMath::Vector2d(gMath::ToRadians(directionAngle)) * speed)
    , _texture(texture)
    , _radius(texture.Width() / 2.0)
    , _paddle(paddle)
{
}

// update ball state with given time difference from last update
void Ball::Update(float deltaTime)
{
  // if the ball hits the paddle we change direction
  if (HasHitPaddle()) {
    BouncePaddle();
  }
  // update position
  _position += _velocity * deltaTime;
}

// updates ball direction and velocity vector; takes new  direction angle
void Ball::UpdateDirection(float directionAngle)
{
  // update direction
  _direction = directionAngle;
  // update velocity
  _velocity = gMath::Vector2d(gMath::ToRadians(directionAngle)) * _speed;
}

// render the ball
void Ball::Draw() const
{
  _texture.Render(
      static_cast<int>(_position._x), static_cast<float>(_position._y));
}

// checks for collision with the paddle. Returns true if colided, false if not
bool Ball::HasHitPaddle() const
{
  // we assume that ball can hit the paddle only when heading downwards, so
  // first we check the ball directions to spare spourious calculations
  if (_direction <= 180.0f && _direction >= 0.0f) {
    return false;
  }
  //  we can consider the ball only above the paddle.
  // We caalculate vertical distance from ball centre to paddle top
  float distance = _paddle.TopBorderY() - _position._y;

  // vertical condition of collision is met if the distance from from paddle's
  // top border to the ball centre is smaller than the ball radius
  bool vertical_condition = distance < _radius;
  // horizontal condition  of collision is met if the ball center is
  // horizontally between left and right paddle's borders
  bool horizontal_condition = _paddle.LeftX() <= _position._x - _radius
      && _paddle.RightX() > _position._x;
  // if both condistions are met collision occurs;
  return vertical_condition && horizontal_condition;
}
// change ball direction after hitting paddle
// should be called only when the ball hits the paddle
void Ball::BouncePaddle()
{
  // we assume that the ball to hit the paddle can only head downwards
  // and we change th ball direction accordingly

  // if the ball heads downwards and towards the left
  if (_direction > 180.0f && _direction < 270.0f) {
    // angle between paddle plain and the ball direction: alfa = oldDirection -
    // 180
    float alfa = _direction - 180.0f;
    // new ball direction = 180 - alfa
    _direction = 180.0f - alfa;
  }
  // if the ball heads downwards and to the right
  if (_direction >= 270.0f && _direction < 360.0f) {
    // new ball direction = alfa = 360 - oldDirection
    float new_direction = 360.0f - _direction;
    _direction = new_direction;
  }
  // ball vellocity vector needs to be updated in both directions
  _velocity = (gMath::Vector2d(gMath::ToRadians(_direction)) * _speed);
}

// NOTE: might not be needed
// keeps the ball direction angle in range [0,360)
void Ball::ControlDirection()
{
  // for angles exceeding the full 360.0 radius
  if (_direction >= 360.0f) {
    _direction -= 360.0f;
  }
  // for possible (but unlikely) negative angles
  if (_direction < 0.0f) {
    _direction = 360.0f - fabs(_direction);
  }
}