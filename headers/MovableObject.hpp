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
  // sets _position vector and sclar speed
  MovableObject(float x, float y, float speed);
  // defalult virtual destructor
  virtual ~MovableObject() = default;
  // updates objet state
  virtual void Update(float deltaTime) = 0;
  // draws the object
  virtual void Draw() const = 0;

  // gets object position
  virtual gMath::Vector2d Position() const { return _position; };

  protected:
  // object position
  gMath::Vector2d _position;
  // objects scalar speed
  float _speed;
};
#endif // !MovableObject_hpp