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

  // if the ball hits a sidewall we change direction
  for (auto& wall : _side_walls) {
    if (HasHitWall(wall)) {
      BounceWall(wall);
      // TODO: handle sound or something
    }
  }

  _position += _velocity * deltaTime;

  // check if has left the screen
  if (HasLeftScreen()) {
    _game.BallEscapeHandler();
  }
}

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

// check if the ball has collided with the specific wall
bool Ball::HasHitWall(const SideWall& wall) const
{
  // check what side of screen the wall is placed
  ScreenSide side = wall.GetScreenSide();
  // we check if the distance from the wall is greater than collision distance:
  // horizontal distance for left and right side wall and the vertical distance
  // for the top wall
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
      throw std::logic_error(
          "Invalid screen side passed to Ball::HasHitWall()");
  }
}

// updates ball's direction angle and velocity vector depending on the side of
// the screen the wall is placed
void Ball::BounceWall(const SideWall& wall)
{
  // check what side of screen the wall is placed
  ScreenSide side = wall.GetScreenSide();
  // new direction to be calculated
  float new_direction {};

  switch (side) {
    case ScreenSide::Left:
      new_direction = NewDirectionLeftWallBounced();
      break;
    case ScreenSide::Right:
      new_direction = NewDirectionRightWallBounced();
      break;
    case ScreenSide::Top:
      new_direction = NewDirectionTopWallBounced();
      break;
    default:
      throw std::logic_error(
          "Invalid screen side passed to Ball::BounceWall()");
  }
  // update ball's direction and velocity vector
  UpdateDirection(new_direction);
}

// calculates the ball new direction after hitting left wall
float Ball::NewDirectionLeftWallBounced()
{
  // Ball could hit the left wall only when heading left (up or down)
  float new_direction {};
  // Check if the ball is heading left and upwards
  if (_direction > 90.0f && _direction <= 180.0f) {
    new_direction = 180.0f - _direction;
    return new_direction;
  }
  // Otherwise the ball is heading left and downwards
  else {
    float deltaAngle = 270.0f - _direction;
    new_direction = 270.0f + deltaAngle;
    return new_direction;
  }

  return new_direction;
}

// calculates the ball new direction after hitting right wall
float Ball::NewDirectionRightWallBounced()
{
  // Ball could hit the right wall only when heading right (up or down)
  float new_direction {};
  // Check if the ball is heading right and upwards
  if (_direction >= 0.0f && _direction <= 90.0f) {
    new_direction = 180.0f - _direction;
  }
  // Otherwise the ball is heading right and downwards
  else {
    float angle = 360.0f - _direction;
    new_direction = 180.0f + angle;
  }

  return new_direction;
}

// calculates the ball new direction after hitting top wall
float Ball::NewDirectionTopWallBounced()
{
  // Ball could hit the top wall only when heading towards top (left or right)
  float new_direction {};
  // Check if the ball is heading towards top and to the left
  if (_direction < 180.0f && _direction >= 90.0f) {
    float angle = 180.0f - _direction;
    new_direction = 180.0f + angle;
  }
  // Otherwise the ball is heading towards top and to the right
  else {
    new_direction = 360.0f - _direction;
  }

  return new_direction;
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