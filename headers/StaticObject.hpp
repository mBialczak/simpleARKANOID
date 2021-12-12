#ifndef StaticObject_hpp
#define StaticObject_hpp
#include "Vector2d.hpp"

// abstract class for representing static (not moving objects in the game)
class StaticObject
{
  public:
  // default constructor
  StaticObject();
  // Constructor setting position. Takes x and y coordinates of the object
  StaticObject(float x, float y);
  // virutual destructor for handling inheritence
  virtual ~StaticObject() = default;
  // draws the object
  virtual void Draw() const = 0;
  // gets object position
  virtual gMath::Vector2d Position() const { return _position; };

  protected:
  // vector representing object's position
  gMath::Vector2d _position;
};
#endif // !StaticObject.hpp
