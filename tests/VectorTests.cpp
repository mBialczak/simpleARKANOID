#include "gMath.hpp"
#include <iomanip>
#include <iostream>

int main(int argc, char const* argv[])
{
  using namespace gMath;
  using std::cout;
  using std::endl;
  using std::fixed;
  using std::setprecision;

  // test Pi value
  cout << "test Pi value: " << setprecision(60) << c_Pi << endl
       << "###############################" << endl;

  // // creating unit vector from angle test
  // Vector2d unit0 { 0.0d };
  // Vector2d unit90 { c_Pi / 2 };
  // Vector2d unit180 { c_Pi };
  // Vector2d unit270 { c_Pi * 2 / 3 };
  // Vector2d unit360 { c_Pi * 2 };

  // cout << "Unit vector of 0 degrees:" << setprecision(8) << endl
  //      << "x: " << unit0._x << endl
  //      << "y: " << unit0._y << endl
  //      << "============================" << endl;

  // cout << "Unit vector of 90 degrees:" << setprecision(8) << endl
  //      << "x: " << unit90._x << endl
  //      << "y: " << unit90._y << endl
  //      << "============================" << endl;

  // cout << "Unit vector of 180 degrees:" << setprecision(8) << endl
  //      << "x: " << unit180._x << endl
  //      << "y: " << unit180._y << endl
  //      << "============================" << endl;

  // cout << "Unit vector of 180 degrees:" << setprecision(8) << endl
  //      << "x: " << unit180._x << endl
  //      << "y: " << unit180._y << endl
  //      << "============================" << endl;
  // return 0;

  // cout << "Unit vector of 270 degrees:" << setprecision(8) << endl
  //      << "x: " << unit270._x << endl
  //      << "y: " << unit270._y << endl
  //      << "============================" << endl;

  // cout << "Unit vector of 360 degrees:" << setprecision(8) << endl
  //      << "x: " << unit360._x << endl
  //      << "y: " << unit360._y << endl
  //      << "============================" << endl;

  // Vector multiplied by speed scalar test
  Vector2d vec45 { ToRadians(45.0) };
  cout << "Unit vector 45 deg: " << endl
       << "x: " << vec45._x << endl
       << "y: " << vec45._y << endl
       << "============================" << endl;

  float speed = 5.0;
  Vector2d vec45_with_speedl = vec45 * speed;
  cout << "Unit vector 45 deg mulitiplied by: " << speed << endl
       << "x: " << vec45_with_speedl._x << endl
       << "y: " << vec45_with_speedl._y << endl
       << "============================" << endl;

  Vector2d vec0 { ToRadians(0) };
  cout << "Unit vector 0 deg: " << endl
       << "x: " << vec0._x << endl
       << "y: " << vec0._y << endl
       << "============================" << endl;

  Vector2d vec0_with_speedl = vec0 * speed;
  cout << "Unit vector 0 deg mulitiplied by: " << speed << endl
       << "x: " << vec0_with_speedl._x << endl
       << "y: " << vec0_with_speedl._y << endl
       << "============================" << endl;
  return 0;
}
