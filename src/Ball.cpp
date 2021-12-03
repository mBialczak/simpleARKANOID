#include "Ball.hpp"
#include "Game.hpp"
#include <iostream> // NOTE: remove after testing

// constructor:
// speed - ball sclar speed in pixels / second
// texture - texture used for displaying the ball
// paddle - reference to paddle against ball collision will be checked
// game - reference to the main game object
// sideWalls - reference to sideWalls for collision detection
// blocks - blocks to be shot at; for collision detection
Ball::Ball(float speed, const Texture& texture, Paddle& paddle,
    float screenBottomY, Game& game, const std::vector<SideWall>& sideWalls,
    std::vector<Block>& blocks)
    : MovableObject(0.0f, 0.0f, speed)
    , _direction(0.0f)
    , _velocity(gMath::Vector2d())
    //(gMath::Vector2d(gMath::ToRadians(directionAngle)) * speed)
    , _texture(texture)
    , _radius(texture.Width() / 2.0)
    , _paddle(paddle)
    , _screen_bottom_y(screenBottomY)
    , _game(game)
    , _side_walls(sideWalls)
    , _blocks(blocks)
{
}

// TODO: make clean or break into functions
// update ball state with given time difference from last update
void Ball::Update(float deltaTime)
{
  // until player starts the ball movement, it should be placed on the
  // paddle
  if (_in_starting_pos) {
    PlaceOnPaddle();
  }
  // otherwise perfom routine update cycle
  else {
    HandlePaddleCollisions();

    HandleWallCollisions();

    HandleBlockCollisions();

    // update ball positoon
    _position += _velocity * deltaTime;

    // react if the ball has left the screen
    if (HasLeftScreen()) {
      _game.HandleBallEscape();
    }
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
  // recalculate the ball direction to fit within 0 - 360 limitas
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

// starts the ball movement
void Ball::Start()
{
  // calculate randomized starting direction when ball starts from the paddle
  float starting_direction = _randomizer(20.0f, 160.0f);
  // update ball state;
  UpdateDirectionAndVelocity(starting_direction);
  _in_starting_pos = false;
}

// puts the ball in the starting position on the paddle
void Ball::PlaceOnPaddle()
{
  float start_x = _paddle.Position()._x;
  float start_y
      = _paddle.Position()._y - _paddle.HalfHeight() - _texture.Height() / 2.0;
  _position = gMath::Vector2d(start_x, start_y);
}

// checks if the ball has hit the paddle. If so, updates the game state
// accoringly
void Ball::HandlePaddleCollisions()
{
  if (HasHitPaddle()) {
    // change direction
    BouncePaddle();
    // REVIEW: verify IN
    _paddle.Stop();
    // TODO: handle sound
  }
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

// checks if the ball has hit any of the walls. If so, updates the game state
// accordingly
void Ball::HandleWallCollisions()
{
  for (auto& wall : _side_walls)
    if (HasHitWall(wall)) {
      {
        // change direction
        BounceWall(wall);
        // REVIEW: think if needed
        // if one wall hit, no need to check the others in this run
        break;
        // TODO: handle sound or something
      }
    }
}

// checks if the ball has hit any of the blocks. If so, updates the game state
// accordingly
void Ball::HandleBlockCollisions()
{
  // for limiting repetitions REVIEW: refactor
  // bool switched_direction = false;

  // TODO: COMMENT
  for (auto& block : _blocks) {
    // checks if the ball has hit any block which hasn't already been
    // destroyed
    if (!block.IsDestroyed() && HasHitBlock(block)) {
      // TODO: limit repetitions if two balls hit in one go

      // chagne ball direction
      BounceBlock(block);
      // REVIEW: play sound or something
      _game.HandleBlockHit(block);
    }
  }
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
float Ball::NewDirectionLeftWallBounced() const
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
float Ball::NewDirectionRightWallBounced() const
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
float Ball::NewDirectionTopWallBounced() const
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

  // update ball direction and vellocity
  UpdateDirectionAndVelocity(new_direction);
}
// TODO: COMMENTS
// checks if the ball hit a specific block
bool Ball::HasHitBlock(const Block& block) const
{
  // we assume that collision occurs when both the vertical and horizontal
  // distance from ball to block centre are smaller that the sum of ball radius
  // and half block hight or width accordingly
  return (gMath::HorizontalDistance(_position, block.Position())
             < _radius + block.HalfWidth())
      && gMath::VerticalDistance(_position, block.Position())
      < _radius + block.HalfHeight();
}

// TODO: review and clean
// determines which block border was hit during collision
void Ball::BounceBlock(const Block& block)
{
  // we use left and right coordinate of the block borders in relation to the
  // ball's position X coordinate to approximate the side of the block being
  // collided
  float left_x = block.Position()._x - block.HalfWidth();
  float right_x = block.Position()._x + block.HalfWidth();
  // REVIEW: remove if X approch is better
  // float bottom_y = block.Position()._y + block.HalfHeight();
  // float top_y = block.Position()._y - block.HalfHeight();
  // new direction of the ball to be calculated

  float new_direction {};

  // for the ball heading upwards and to the right
  if (_direction >= 0.0f && _direction <= 90.0f) {
    new_direction = BounceBlockGoingUpRight(left_x);
  }
  // for the ball  heading upwards and to the left
  else if (_direction > 90.0f && _direction <= 180.0f) {
    new_direction = BounceBlockGoingUpLeft(right_x);
  }
  // for the ball heading downwards and to the left
  else if (_direction > 180.0f && _direction <= 270.0f) {
    new_direction = BounceBlockGoingDownLeft(right_x);
  }
  // for the ball heading downwards and to the right,
  else {
    new_direction = BounceBlockGoingDownRight(left_x);
  }

  // update ball direction and vellocity
  UpdateDirectionAndVelocity(new_direction);
}
// returns new direction after block bounce when going up and right
float Ball::BounceBlockGoingUpRight(float leftX) const
{
  // If the ball is heading upwards and to the right,
  // either left or bottom  border of the block could be hit
  float new_direction {};

  // left border bounce
  if (_position._x <= leftX) {
    new_direction = 180.0f - _direction;
    // REVIEW: Consider removing or adjusting
    // add random increase for low angles to simulate collision effect
    if (new_direction >= 90.0f && new_direction <= 95.0f) {
      new_direction += _randomizer(5.0f, 8.0f);
      // REVIEW: remove AT
      std::cout << "added random value!" << std::endl;
    }
  }
  // otherwise it is a bottom border bounce
  else {
    new_direction = 360.0f - _direction;
    // REVIEW: Consider removing or adjusting
    // decrease by random value for low angles to simulate collision effect
    // if (new_direction >= 350.0f && new_direction <= 360.0f) {
    //   new_direction -= _randomizer(3.0f, 5.0f);
    //   // REVIEW: remove AT
    //   std::cout << "substracted random value!" << std::endl;
    // }
  }
  return new_direction;
}

// returns new direction after block bounce when going up and left
float Ball::BounceBlockGoingUpLeft(float rightX) const
{
  // if the ball is heading upwards and to the left
  // either right or bottom border of the block could be hit

  // left border bounce
  if (_position._x >= rightX) {
    return 180.0f - _direction;
  }
  // bottom border bounce
  else {
    return 360.0f - _direction;
  }
}
// returns new direction after block bounce when going down and left
float Ball::BounceBlockGoingDownLeft(float rightX) const
{
  // if the ball is heading downwards and to the left,
  // either right or top border of the block could be hit

  // right border bounce
  if (_position._x >= rightX) {
    float angle = _direction - 180.0f;
    // new direction = 360 - angle
    return 360.0f - angle;
  }
  // top border bounce
  else {
    return 360.0f - _direction;
  }
}
// returns new direction after block bounce when going down and right
float Ball::BounceBlockGoingDownRight(float leftX) const
{
  // if the ball is heading downwards and to the right,
  // either left or top border of the block could be hit

  // left border bounce
  if (_position._x <= leftX) {
    float angle = 360.0f - _direction;
    return 180 + angle;
  }
  // top border bounce
  else {
    return 360 - _direction;
  }
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