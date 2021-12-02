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
    const Texture& texture, Paddle& paddle, float screenBottomY, Game& game,
    const std::vector<SideWall>& sideWalls)
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
    _paddle.Stop();
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
void Ball::UpdateDirectionAndVelocity(float directionAngle)
{
  // REVIEW: remove AT
  float old = _direction;
  // update direction angle
  _direction = directionAngle;
  // / REVIEW: remove AT
  std::cout << "Updated direction!\n"
            << "old= " << old << " new= " << _direction << "\n-------------\n"
            << std::endl;

  // REVIEW: think of removing
  ControlDirection();
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

  bool vertical_conditon
      = gMath::VerticalDistance(_position, _paddle.Position())
      < _radius + _paddle.HalfHeight();

  bool horizontal_condition
      = gMath::HorizontalDistance(_position, _paddle.Position())
      < _radius + _paddle.HalfWidth();

  return vertical_conditon && horizontal_condition;
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
          <= _radius + wall.HalfTickness();
    case ScreenSide::Top:
      return gMath::VerticalDistance(_position, wall.Position())
          <= _radius + wall.HalfTickness();
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
  UpdateDirectionAndVelocity(new_direction);
}

// calculates the ball new direction after hitting left wall
float Ball::NewDirectionLeftWallBounced()
{
  // Ball could hit the left wall only when heading left (up or down)
  float new_direction {};
  // Check if the ball is heading left and upwards
  if (_direction > 90.0f && _direction <= 180.0f) {
    new_direction = 180.0f - _direction;
  }
  // Otherwise the ball is heading left and downwards
  else {
    float deltaAngle = 270.0f - _direction;
    new_direction = 270.0f + deltaAngle;
  }
  // REVIEW: remove after testing
  // std::cout << "Should not be here: Left WallBounce: " << __LINE__
  //           << "new_direction= " << new_direction << std::endl;
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
  // REVIEW: remove after testing
  // std::cout << "Should not be here: Right WallBounce: " << __LINE__
  //           << "new_direction= " << new_direction << std::endl;
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
  // REVIEW: remove after testing
  // std::cout << "Should not be here: Top WallBounce: " << __LINE__
  //           << "new_direction= " << new_direction << std::endl;
  return new_direction;
}

// change ball direction after hitting paddle
// should be called only when the ball hits the paddle
void Ball::BouncePaddle()
{
  // due to game specifics the ball will try to hit the paddle only from upper
  // directions so we use this and change the ball direction accordingly

  float new_direction {};

  // if the ball heads downwards and towards the left
  if (_direction > 180.0f && _direction < 270.0f) {
    float angle = _direction - 180.0f;
    new_direction = 180.0f - angle;
  }
  // if the ball heads downwards and to the right
  if (_direction >= 270.0f && _direction < 360.0f) {
    new_direction = 360.0f - _direction;
  }
  // REVIEW: consider removing
  // introducing some randomization of bounce angle for more fun
  new_direction = RandomizeAngles(new_direction);

  // REVIEW: remove after testing
  // std::cout << "Should not be here: Paddle bounce " << __LINE__
  //           << "new_direction= " << new_direction << std::endl;
  // update direction and vellocity
  UpdateDirectionAndVelocity(new_direction);
}

// NOTE: might not be needed
// keeps the ball direction angle in range [0,360)
void Ball::ControlDirection()
{
  // REVIEW: remove AT
  float old = _direction;
  // for angles exceeding the full 360.0 radius
  if (_direction >= 360.0f) {
    _direction -= 360.0f;
    // REVIEW: remove AT
    std::cout << "ControlDirection modified direction!\n"
              << "old= " << old << " new= " << _direction << std::endl;
  }
  // for possible (but unlikely) negative angles
  if (_direction < 0.0f) {
    _direction = 360.0f - fabs(_direction);
    // REVIEW: remove AT
    std::cout << "ControlDirection modified direction!\n"
              << "old= " << old << " new= " << _direction << std::endl;
  }
}

// returnes slightly randomized angles, especially when ball hits paddle with
// angle affecting game experience in a negative way
float Ball::RandomizeAngles(float angle)
{
  if (angle <= 180.0f && angle >= 150.0f) {
    angle += gMath::RandNum::Random(-30.0f, -15.0f);
  }
  else if (angle <= 100.0f && angle >= 80.0f) {
    angle += gMath::RandNum::Random(-15.0f, 15.0f);
  }
  else if (angle <= 30.0f && angle >= 0.0f) {
    angle += gMath::RandNum::Random(15.0f, 30.0f);
  }
  else {
    angle += gMath::RandNum::Random(-10.0f, 10.0f);
  }
  return angle;
}