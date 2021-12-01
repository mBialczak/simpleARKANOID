#include "Ball.hpp"
#include "Game.hpp"
#include <iostream> // NOTE: remove after testing
// constructor:
// X, Y - ball centre coordinates
// directionAngle - direction in degrees in which the ball starts moving
// speed - ball sclar speed in pixels / second
// texture - texture used for displaying the ball
// paddle - reference to paddle against ball collision will be checked
// game - reference to the main game object
// sideWalls - reference to sideWalls for collision detection
Ball::Ball(float X, float Y, float directionAngle, float speed,
    const Texture& texture, const Paddle& paddle, float screenBottomY,
    Game& game, const std::vector<SideWall>& sideWalls)
    : _position(gMath::Vector2d(X, Y))
    , _direction(directionAngle)
    , _speed(speed)
    // set velocity vector: create unit velocity vector and multiply by scalar
    // speed
    , _velocity(gMath::Vector2d(gMath::ToRadians(directionAngle)) * speed)
    , _texture(texture)
    , _radius(texture.Width() / 2.0)
    , _paddle(paddle)
    , _screen_bottom_y(screenBottomY)
    , _game(game)
    , _side_walls(sideWalls)
{
}

// TODO: make clean
// update ball state with given time difference from last update
void Ball::Update(float deltaTime)
{
  // if the ball hits the paddle we change direction
  if (HasHitPaddle()) {
    BouncePaddle();
    // TODO: handle sound
  }
  // else {
  //   // update position
  //   _position += _velocity * deltaTime;
  // }

  // if the ball hits a sidewall
  if (HandleWallCollisions()) {
    // TODO: handle sound or something
  }
  // else {
  // update position
  _position += _velocity * deltaTime;
  // }

  // else
  // {
  //   // update position
  //   _position += _velocity * deltaTime;
  // }

  // check if has left the screen
  if (HasLeftScreen()) {
    _game.BallEscapeHandler();
  }
}

// REVIEW: remove if not used
// updates ball direction and velocity vector; takes new  direction angle
void Ball::UpdateDirection(float directionAngle)
{
  // update direction angle
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

// checks if the ball has left the screen
bool Ball::HasLeftScreen() const
{
  return _position._y - _radius > _screen_bottom_y;
}

// REVIEW: alternative
// check if the ball has collided with the wall
bool Ball::HasHitWall(const SideWall& wall) const
{
  ScreenSide side = wall.GetScreenSide();

  switch (side) {
    case ScreenSide::Left:
      [[fallthrough]];
    case ScreenSide::Right:
      return gMath::HorizontalDistance(_position, wall.Position())
          < _radius + wall.HalfTickness();
    case ScreenSide::Top:
      return gMath::VerticalDistance(_position, wall.Position())
          < _radius + wall.HalfTickness();
    default:
      return false;
  }
}

// check for collision with the walls // NOTE: consider const
bool Ball::HandleWallCollisions()
{
  for (auto& wall : _side_walls) {
    // TODO: for every wall check collisions first
    if (wall.GetScreenSide() == ScreenSide::Left) {
      // checks collision with the left wall
      // if (gMath::HorizontalDistance(_position, wall.Position())
      //     < _radius + wall.HalfTickness()) {
      if (HasHitWall(wall)) {
        BounceLeftWall();
        return true;
      }
    }
  }
  return false;
  // wall.)
}

// change the ball direction after hitting left wall
void Ball::BounceLeftWall()
{
  // Ball can hit the left wall only when heading left

  // Ball heading left and upwards
  if (_direction > 90.0f && _direction <= 180.0f) {
    // newDirection = 180 - oldDirection
    _direction = 180.0f - _direction;
  }
  // Ball heading left and downwards
  else {
    // alfaAngle = 270 - oldDirection
    // newDirection = 270 + alfa
    float alfaAngle = 270.0f - _direction;
    _direction = 270.0f + alfaAngle;
  }
  // ball vellocity vector needs to be updated in both cases
  _velocity = (gMath::Vector2d(gMath::ToRadians(_direction)) * _speed);
}

// change ball direction after hitting paddle
// should be called only when the ball hits the paddle
void Ball::BouncePaddle()
{
  // we assume that the ball to hit the paddle can only head downwards
  // and we change th ball direction accordingly

  // if the ball heads downwards and towards the left
  if (_direction > 180.0f && _direction < 270.0f) {
    // angle between paddle plain and the ball direction: alfa = oldDirection
    // - 180
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
  // ball vellocity vector needs to be updated in both cases
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