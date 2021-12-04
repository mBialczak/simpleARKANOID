#ifndef Enums_hpp
#define Enums_hpp

// enum class for distinguishing block border during collision with the ball
enum class RectBorder
{
  bTop,
  bBottom,
  bRight,
  bLeft
};

// enum class for describing the spin of the ball
enum class Spin
{
  sNone,
  sLeft,
  sRight
};

// enum class for marking which side of the screen the wall occupies
enum class ScreenSide
{
  Top,
  Right,
  Left
};

#endif // !Enums_hpp
