#ifndef Ball_HPP
#define Ball_HPP
#include "Paddle.hpp"
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

class Ball
{
  public:
  // constructor:
  // X, Y - ball centre coordinates
  // directionAngle - direction in degrees in which the ball starts moving
  // speed - ball sclar speed in pixels / second
  // texture - texture used for displaying the ball
  // paddle - reference to paddle against ball collision will be checked
  // game - reference to the main game object
  Ball(float X, float Y, float directionAngle, float speed,
      const Texture& texture, const Paddle& paddle, float screenBottomY,
      Game& game, const std::vector<SideWall>& sideWalls);
  // update ball state with given time difference from last update
  void Update(float deltaTime);
  // updates ball direction and velocity vector; takes new  direction angle
  void UpdateDirection(float directionAngle);
  // render the ball
  void Draw() const;

  private:
  // checks for collision with the paddle. Returns true if colided, false if not
  bool HasHitPaddle() const;
  // checks if the ball has left the screen
  bool HasLeftScreen() const;
  // change the ball direction after hitting paddle
  void BouncePaddle();
  // keeps the ball direction angle in range [0,360)
  // REVIEW: might not be needed
  void ControlDirection();
  // ball position
  gMath::Vector2d _position;
  // ball direction (angle in degrees)//  REVIEW: check units
  float _direction;
  // ball scalar speed
  float _speed;
  // ball vector of velocity
  gMath::Vector2d _velocity;
  // texture used for displaying the ball
  const Texture& _texture;
  // ball radius ; mainly for collision detection
  const float _radius;
  // reference to paddle for collision detection
  const Paddle& _paddle;
  // y coordinate of the bottom of the screen
  const float _screen_bottom_y;
  // reference to the main game object
  // NOTE: consider const and if necessary
  Game& _game;
  const std::vector<SideWall>& _side_walls;
};

#endif // !Ball_HPP