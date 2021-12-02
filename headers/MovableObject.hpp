#ifndef MovableObject_hpp
#define MovableObject_hpp
#include "Vector2d.hpp"

// TODO: comments
class MovableObject
{
  public:
  // default constructor;
  MovableObject();

  MovableObject(float x, float y);

  virtual ~MovableObject() = default;

  virtual void Update(float deltaTime) = 0;

  virtual void Draw() const;

  // gets object position
  virtual gMath::Vector2d Position() const { return _position; };

  protected:
  // object position
  gMath::Vector2d _position;
  // object scalar speed
  float _speed;
  // object vector of velocity
  gMath::Vector2d _velocity;
};
#endif // !MovableObject_hpp