#ifndef Ball_HPP
#define Ball_HPP
#include "MovableObject.hpp"
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

class Ball : public virtual MovableObject
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
      const Texture& texture, Paddle& paddle, float screenBottomY, Game& game,
      const std::vector<SideWall>& sideWalls);

  ~Ball() = default;

  // update ball state with given time difference from last update
  void Update(float deltaTime) override;
  // updates ball direction and velocity vector; takes new  direction angle
  void UpdateDirectionAndVelocity(float directionAngle);
  // render the ball
  void Draw() const override;

  private:
  // checks for collision with the paddle. Returns true if colided, false if not
  bool HasHitPaddle() const;
  // checks if the ball has left the screen
  bool HasLeftScreen() const;
  // check if the ball has collided with the specific wall
  bool HasHitWall(const SideWall& wall) const;
  // updates ball's direction angle and velocity vector depending on wall hit
  void BounceWall(const SideWall& wall);
  // calculates the ball new direction after hitting left wall
  float NewDirectionLeftWallBounced();
  // calculates the ball new direction after hitting right wall
  float NewDirectionRightWallBounced();
  // calculates the ball new direction after hitting top wall
  float NewDirectionTopWallBounced();
  // change the ball direction after hitting paddle
  void BouncePaddle();
  // keeps the ball direction angle in range [0,360)
  // REVIEW: REMOVE INU
  void ControlDirection();
  // returnes slightly randomized angles, especially when ball hits paddle with
  // angle affecting game experience in a negative way
  float RandomizeAngles(float angle);

  // ball direction (angle in degrees)//  REVIEW: check units
  float _direction;
  // ball vector of velocity
  gMath::Vector2d _velocity;
  // texture used for displaying the ball
  const Texture& _texture;
  // ball radius ; mainly for collision detection
  const float _radius;
  // reference to paddle for collision detection
  Paddle& _paddle;
  // y coordinate of the bottom of the screen
  const float _screen_bottom_y;
  // reference to the main game object
  // NOTE: consider const and if necessary
  Game& _game;
  // reference to vector of sidewall for collision detection
  const std::vector<SideWall>& _side_walls;
};

#endif // !Ball_HPP