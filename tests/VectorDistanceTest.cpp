#include "Vector2d.hpp"
#include <iostream>

int main(int argc, char const* argv[])
{
  using namespace gMath;
  using namespace std;
  Vector2d Ball { 50.0f, 300.0f };
  Vector2d Wall { 5.0f, 200.0f };
  Vector2d Ball2 { 100.0f, 100.0f };
  Vector2d Wall2 { 500.0f, 5.0f };

  cout << "Horizontal Distance between Ball and Wall: "
       << HorizontalDistance(Ball, Wall)
       << " or: " << HorizontalDistance(Wall, Ball) << endl;

  cout << "Vertical Distance between Ball and Wall: "
       << VerticalDistance(Ball2, Wall2)
       << " or: " << VerticalDistance(Wall2, Ball2) << endl;

  return 0;
}
