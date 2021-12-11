#include "Ball.hpp"
#include "Game.hpp"
#include <iostream> // NOTE: remove after testing

// constructor: // VERIFY comments
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
    , _texture(texture)
    , _radius(texture.Width() / 2.0)
    , _paddle(paddle)
    , _screen_bottom_y(screenBottomY)
    , _game(game)
    , _side_walls(sideWalls)
    , _blocks(blocks)
{
  // REVIEW: consider checking invariants in the constructor
}

// TODO: make clean or break into functions
// update ball state with given time difference from last update
void Ball::Update(float deltaTime)
{
  // until player starts the ball movement,
  //  it should be placed on the
  if (_in_starting_pos) {
    PlaceOnPaddle();
  }
  // otherwise perfom routine update cycle
  else {
    // react to ball-paddle collisioins and apply changes to speed and direction
    // due to spin (only when the the ball hits the paddle)
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
  // update direction angle
  _direction = directionAngle;

  // recalculate the ball direction to fit within 0 - 360 limits
  ControlDirection();

  // update velocity vector
  _velocity = gMath::Vector2d(gMath::ToRadians(directionAngle)) * _speed;
}

// render the ball
void Ball::Draw() const
{
  _texture.Render(
      static_cast<int>(_position._x), static_cast<int>(_position._y));
}

// sets current speed of the object if it is not lower than minimal speed
void Ball::SetSpeed(float speed)
{
  if (speed >= _min_speed) {
    // update speed
    _speed = speed;
    // update velocity vector // VERIFY
    _velocity = gMath::Vector2d(gMath::ToRadians(_direction)) * speed;
  }
}

// resets ball min speed and speed values to the passed value and
// sets position to the starting position on the paddle
void Ball::Reset(float speed)
{
  PlaceOnPaddle();

  _direction = 0.0f;

  _velocity = gMath::Vector2d { 0.0 };

  _min_speed = speed;

  _speed = speed;

  // set bolean indicating state where ball should start from the paddle
  _in_starting_pos = true;
}

// starts the ball movement
void Ball::Start()
{
  // calculate randomized starting direction when ball starts from the paddle
  float starting_direction = _randomizer(45.0f, 135.0f);
  // update ball state;
  UpdateDirectionAndVelocity(starting_direction);
  // mark that the ball doesn't have to start from the paddle
  _in_starting_pos = false;
  // play the correct sound
  _game.PlaySound(Sound::BallPaddleHit);
}

// puts the ball in the starting position on the paddle
void Ball::PlaceOnPaddle()
{
  // calculate the correct position
  float start_x = _paddle.Position()._x;
  float start_y
      = _paddle.Position()._y - _paddle.HalfHeight() - _texture.Height() / 2.0;

  // update position vector
  _position = gMath::Vector2d(start_x, start_y);
}

// checks if the ball has hit the paddle. If so, updates the game state
// accoringly:
// - changes ball direction after bounce - including applying spin
// - increases or decreases speed if the user gave a command via controller
void Ball::HandlePaddleCollisions()
{
  if (HasHitPaddle()) {
    // change direction including spin application
    BouncePaddle();
    // Apply speed change requested via keyboard
    ApplySpeedDelta();
    // play the proper sound
    _game.PlaySound(Sound::BallPaddleHit);
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
  // vertical condition of collision
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
        // play the proper sound
        _game.PlaySound(Sound::BallBounceWall);
        // only one wall can be hit at a certain moment, so no need to change
        // any more
        break;
      }
    }
}

// checks if the ball has hit any of the blocks. If so, updates the game state
// accordingly
void Ball::HandleBlockCollisions()
{
  for (auto& block : _blocks) {
    // checks if the ball has hit any block which hasn't already been
    // destroyed
    if (!block.IsDestroyed() && HasHitBlock(block)) {
      // change the ball direction
      BounceBlock(block);
      // call proper game function
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
      throw std::invalid_argument(
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
      throw std::invalid_argument(
          "Invalid screen side passed to Ball::BounceWall()");
  }
  // update ball's direction and velocity vector
  UpdateDirectionAndVelocity(new_direction);
}

// calculates the ball new direction after hitting left wall
float Ball::NewDirectionLeftWallBounced() const
{
  // Ball should hit the left wall only when heading left (up or down)
  float new_direction {};

  // the ball is heading left and upwards
  if (_direction > 90.0f && _direction <= 180.0f) {
    new_direction = 180.0f - _direction;
  }
  // the ball is heading left and downwards
  else if (_direction > 180.0f && _direction < 270.0f) {
    float deltaAngle = 270.0f - _direction;
    new_direction = 270.0f + deltaAngle;
  }
  // if the direction is different than above simply keeps it unchanged in order
  // to avoid strange cycles when the ball didn't menage to escepe far enough
  // from the wall before next collision is detected
  else {
    new_direction = _direction;
  }

  return new_direction;
}

// calculates the ball new direction after hitting right wall
float Ball::NewDirectionRightWallBounced() const
{
  // Ball should hit the right wall only when heading right (up or down)
  float new_direction {};

  // the ball is heading right and upwards
  if (_direction >= 0.0f && _direction < 90.0f) {
    new_direction = 180.0f - _direction;
  }
  // the ball is heading right and downwards
  else if (_direction > 270.0 && _direction > 0.0f) {
    float angle = 360.0f - _direction;
    new_direction = 180.0f + angle;
  }
  // if the direction is different than above simply keeps it unchanged in order
  // to avoid strange cycles when the ball didn't menage to escepe far enough
  // from the wall before next collision is detected
  else {
    new_direction = _direction;
  }

  return new_direction;
}

// calculates the ball new direction after hitting top wall
float Ball::NewDirectionTopWallBounced() const
{
  // Ball should hit the top wall only when heading towards top (left or right)
  float new_direction {};

  // the ball is heading towards top and to the left
  if (_direction < 180.0f && _direction >= 90.0f) {
    float angle = 180.0f - _direction;
    new_direction = 180.0f + angle;
  }
  // the ball is heading towards top and to the right
  else if (_direction < 90.0f && _direction > 0.0f) {
    new_direction = 360.0f - _direction;
  }
  // if the direction is different than above simply keeps it unchanged in order
  // to avoid strange cycles when the ball didn't menage to escepe far enough
  // from the wall before next collision is detected
  else {
    new_direction = _direction;
  }

  return new_direction;
}

// change ball direction after hitting paddle
// should be called only when the ball hits the paddle
void Ball::BouncePaddle()
{
  float new_direction {};

  // In Ball::HasHitPaddle() we already assured that the ball is heading
  // downwards (left or right) so we use this to change the ball direction
  // accordingly

  // if the ball heads downwards and towards the left
  if (_direction > 180.0f && _direction < 270.0f) {
    float bounce_angle = _direction - 180.0f;
    new_direction = 180.0f - bounce_angle;
    // increment the bounce angle introducing (possibly negative) spin
    new_direction += CalcSpin(bounce_angle);
  }
  // if the ball heads downwards and to the right
  //(_direction >= 270.0f && _direction < 360.0f)
  else {
    new_direction = 360.0f - _direction;
    // increment the bounce angle introducing (possibly negative) spin
    // (bounce_angle == new_direction for the ball heading down and right)
    new_direction += CalcSpin(new_direction);
  }

  // avoid sending the ball to far to the left
  if (new_direction >= 175.0f) {
    new_direction = 175.0f;
  }
  // avoid sending the ball to fart to the right
  if (new_direction <= 5.0f) {
    new_direction = 5.0f;
  }

  // update ball direction and vellocity
  UpdateDirectionAndVelocity(new_direction);
}

// checks if the ball hit a specific block
bool Ball::HasHitBlock(const Block& block) const
{
  // we assume that collision occurs when both the vertical and horizontal
  // distance from ball to block centre are smaller that the sum of ball
  // radius and half block hight or width accordingly
  return (gMath::HorizontalDistance(_position, block.Position())
             < _radius + block.HalfWidth())
      && gMath::VerticalDistance(_position, block.Position())
      < _radius + block.HalfHeight();
}

// determines which block border was hit during collision
void Ball::BounceBlock(const Block& block)
{
  // we use left and right coordinate of the block borders in relation to the
  // ball's position X coordinate to approximate the side of the block being
  // collided
  float left_x = block.Position()._x - block.HalfWidth();
  float right_x = block.Position()._x + block.HalfWidth();

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
  }
  // otherwise it is a bottom border bounce
  else {
    new_direction = 360.0f - _direction;
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

// calculates the spin to be applied
float Ball::CalcSpin(float bounceAngle) const
{
  // reduce wide ball atack in order not to squew the results
  if (bounceAngle > 45.0) {
    bounceAngle = 90.0 - bounceAngle;
  }
  // the spin increment to be added or substracted from basic bounce angle
  float delta { 0.0f };

  // range of spin increment needs to be adjusted depending on the angle values
  // and it was chosen to use random values for increased game experience

  // angles in range (25,45] degrees
  if (bounceAngle > 25.0f) {
    // spin in range of 20-60 % of original bounce angle
    delta = _randomizer(0.2f, 0.6f) * bounceAngle;
  }
  // angles in range (10,25] degrees
  else if (bounceAngle > 10.0f) {
    // spin in range of 40-80 % of original bounce angle
    delta = _randomizer(0.4f, 0.8f) * bounceAngle;
  }
  // angles in range [0,10] degrees
  else {
    // spin range [5.0,10] degrees
    delta = _randomizer(5.0f, 10.0f);
  }

  // depending of the requested spin return negative or positive spin increase
  switch (_spin) {
    case Spin::Left:
      return delta;
    case Spin::Right:
      return -delta;
    default:
      return 0;
  }
}

// increases/deacreases the ball speed by speed delta
void Ball::ApplySpeedDelta()
{
  if (_speed_delta != 0.0f) {
    SetSpeed(_speed + _speed_delta);
  }
}