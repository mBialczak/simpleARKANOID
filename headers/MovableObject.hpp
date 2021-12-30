#ifndef MovableObject_hpp
#define MovableObject_hpp
#include "Vector2d.hpp"

// abstract class for representing movable objects in the game
class MovableObject
{
  public:
  // default constructor;
  MovableObject();
  // constructor taking position coordinates
  // sets position_ vector and sclar speed
  MovableObject(float x, float y, float speed);
  // defalult virtual destructor
  virtual ~MovableObject() = default;
  // updates object state
  virtual void Update(float deltaTime) = 0;
  // draws the object
  virtual void Draw() const = 0;
  // gets object position
  virtual gMath::Vector2d Position() const { return position_; };
  // sets object position
  virtual void SetPosition(float x, float y);
  // gets current speed of the object;
  virtual float Speed() const { return _speed; }
  // sets the scalar speed of the object;
  virtual void SetSpeed(float speed) { _speed = speed; }

  protected:
  // object position
  gMath::Vector2d position_;
  // objects scalar speed
  float _speed;
};
#endif // !MovableObject_hpp