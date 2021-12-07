#ifndef Enums_hpp
#define Enums_hpp

// enum for distinguishing block border during collision with the ball
enum class RectBorder : short
{
  bTop,
  bBottom,
  bRight,
  bLeft
};

// enum for describing the spin of the ball
enum class Spin : short
{
  sNone,
  sLeft,
  sRight
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
#endif // !Enums_hpp