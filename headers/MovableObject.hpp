#ifndef MovableObject_hpp
#define MovableObject_hpp
#include "Vector2d.hpp"

// TODO: comments
class MovableObject
{
  public:
  // default constructor;
  MovableObject();
  // constructor taking position coordinates
  // sets _position vector and sclar speed
  MovableObject(float x, float y, float speed);

  virtual ~MovableObject() = default;

  virtual void Update(float deltaTime) = 0;

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