#ifndef StaticObject_hpp
#define StaticObject_hpp
#include "gMath.hpp"

// pure virtual class for representing static (not moving objects in the game)
class StaticObject {
  public:
  // default constructor
  StaticObject();
  // constructor setting position
  StaticObject(float x, float y);
  // virutual destructor for handling inheritence
  virtual ~StaticObject() = default;
  // gets object positoin
  virtual gMath::Vector2d Position() const { return _position; };
  // pure virtual function for setting position
  virtual void SetPosition(float x, float y) = 0;

  protected:
  // vector representing object's position
  gMath::Vector2d _position;
};
#endif // !StaticObject.hpp
