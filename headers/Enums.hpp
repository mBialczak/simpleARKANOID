#ifndef Enums_hpp
#define Enums_hpp

// enum for distinguishing block border during collision with the ball
enum class RectBorder : short
{
  Top,
  Bottom,
  Right,
  Left
};

// enum for describing the spin of the ball
enum class Spin : short
{
  None,
  Left,
  Right
};

// enum for marking which side of the screen the wall occupies
enum class ScreenSide : short
{
  Top,
  Right,
  Left
};

// enum representing sprites used in the game
enum class Sprite : short
{
  None,
  Ball,
  Paddle,
  WallHorizontal,
  WallVertical,
  BlockGreen
};

// enum representing allowed char values to be put while level-file creation
enum class BlockCharCode : char
{
  None = '0',
  BlockGreen = 'g',

};
// VERIFY
// enum representing possible states of the game
enum class GameState : char
{
  // "nomral" running state, where all the action happens
  Routine,
  // game is paused or not yet started
  Paused,
  // game is over (player lost all the balls)
  Over,
  // game is won (all the levels completed)
  Won

};

// enum representing sound effects usued in the game
enum class Sound : short
{
  None,
  BallBounceWall,
  BallPaddleHit,
  BlockHit,
  BallLost,
  LevelCompleted,
  GameWon
};
#endif // !Enums_hpp
