#ifndef Ball_HPP
#define Ball_HPP
#include "Block.hpp"
#include "Enums.hpp"
#include "MovableObject.hpp"
#include "Paddle.hpp"
#include "RandNum.hpp"
#include "SideWall.hpp"
#include "Texture.hpp"
#include "Vector2d.hpp"
#include <vector>
// #include "gMath.hpp" // NOTE: remove INU

// class forward declarations
class Texture;
class Paddle;
class Game;
class SideWall;
class Block;

class Ball : public virtual MovableObject
{
  public:
  // constructor:
  // speed - ball sclar speed in pixels / second
  // texture - texture used for displaying the ball
  // paddle - reference to paddle against ball collision will be checked
  // game - reference to the main game object
  // blocks - blocks to be shot at; for collision detection
  Ball(float speed, const Texture& texture, Paddle& paddle, float screenBottomY,
      Game& game, const std::vector<SideWall>& sideWalls,
      std::vector<Block>& blocks);

  // default virtual destructor
  ~Ball() override = default; // NOTE: override?

  // update ball state with given time difference from last update
  void Update(float deltaTime) override;
  // render the ball
  void Draw() const override;
  // sets current speed of the object if it is not lower than minimal speed
  void SetSpeed(float speed) override;
  // sets the speed increase/decrease applied when the ball hits the paddle
  void SetSpeedDelta(float speedChange) { _speed_delta = speedChange; }
  // updates ball direction and velocity vector; takes new  direction angle
  void UpdateDirectionAndVelocity(float directionAngle);
  // resets ball min speed and speed values to the passed value and
  // sets position to the starting position on the paddle
  void Reset(float speed);
  // starts the ball movement
  void Start();
  // puts the ball in the starting position on the paddle
  void PlaceOnPaddle();
  // sets ball spin
  void SetSpin(Spin spinDirection) { _spin = spinDirection; }

  // returns ball radius
  float Radius() const { return _radius; }
  // checks if the ball is in the starting position
  bool IsMoving() const { return !_in_starting_pos; }

  private:
  // checks for collision with the paddle. Returns true if colided, false if not
  bool HasHitPaddle() const;
  // checks if the ball has hit the paddle. If so, updates the game state
  // accoringly
  void HandlePaddleCollisions();
  // checks if the ball has hit any of the walls. If so, updates the game state
  // accordingly
  void HandleWallCollisions();
  // checks if the ball has hit any of the blocks. If so, updates the game state
  // accordingly
  void HandleBlockCollisions();
  // checks if the ball has left the screen
  bool HasLeftScreen() const;
  // check if the ball has collided with the specific wall
  bool HasHitWall(const SideWall& wall) const;
  // updates ball's direction angle and velocity vector depending on wall hit
  void BounceWall(const SideWall& wall);
  // calculates the ball new direction after hitting left wall
  float NewDirectionLeftWallBounced() const;
  // calculates the ball new direction after hitting right wall
  float NewDirectionRightWallBounced() const;
  // calculates the ball new direction after hitting top wall
  float NewDirectionTopWallBounced() const;
  // change the ball direction after hitting paddle
  void BouncePaddle();
  // checks if the ball hit a specific block
  bool HasHitBlock(const Block& block) const;
  // change the ball direction after hitting particualr block
  void BounceBlock(const Block& block);
  // returns new direction after block bounce when going up and right
  float BounceBlockGoingUpRight(float leftX) const;
  // returns new direction after block bounce when going up and left
  float BounceBlockGoingUpLeft(float rightX) const;
  // returns new direction after block bounce when going down and left
  float BounceBlockGoingDownLeft(float rightX) const;
  // returns new direction after block bounce when going down and right
  float BounceBlockGoingDownRight(float leftX) const;
  // keeps the ball direction angle in range [0,360)
  void ControlDirection();
  // returns slightly randomized angles, especially when ball hits paddle with
  // angle affecting game experience in a negative way
  // REMOVE if not used; most likely superseded by Spin()
  // float RandomizeAngles(float angle);

  // calculates the spin to be applied
  float CalcSpin(float bounceAngle) const;
  // increases/deacreases the ball speed by speed delta
  void ApplySpeedDelta();

  // ball direction in degrees (0.0 - 360.0)
  float _direction;
  // ball vector of velocity
  gMath::Vector2d _velocity;
  // texture used for displaying the ball
  const Texture& _texture;
  // ball radius
  const float _radius;
  // indicates if the ball is in starting position on the paddle
  bool _in_starting_pos = true;
  // current value of spin to be applied
  Spin _spin = Spin::None;
  // ball minimum speed in the current level
  float _min_speed = _speed;
  // speed change (pixels/second) aplied every time the ball hits the paddle
  float _speed_delta = 0.0;
  // reference to paddle for collision detection
  Paddle& _paddle;
  // y coordinate of the bottom of the screen
  const float _screen_bottom_y;
  // reference to the main game object
  // NOTE: consider const and if necessary
  Game& _game;
  // random number generator used for simulating angle changes
  gMath::RandNum _randomizer;
  // sideWalls for collision detection
  const std::vector<SideWall>& _side_walls;
  // blocks for collision detection
  std::vector<Block>& _blocks;
};

#endif // !Ball_HPP