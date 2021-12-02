#ifndef StaticObject_hpp
#define StaticObject_hpp
#include "Vector2d.hpp"
// #include "gMath.hpp" // NOTE: remvoe INU

// pure virtual class for representing static (not moving objects in the game)
class StaticObject
{
  public:
  // default constructor
  StaticObject();
  // constructor setting position
  StaticObject(float x, float y);
  // virutual destructor for handling inheritence
  virtual ~StaticObject() = default;
  // gets object position
  virtual gMath::Vector2d Position() const { return _position; };

  protected:
  // vector representing object's position
  gMath::Vector2d _position;
};
#endif // !StaticObject.hpp
