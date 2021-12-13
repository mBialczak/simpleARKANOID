#ifndef RandNum_hpp
#define RandNum_hpp

#include <random>

// namespace for grouping objects and functions required for the game
// calculations
namespace gMath {

// class for getting random numbers
class RandNum
{
  public:
  // Gets random integral or floating point number depending of parameter type.
  // For integral numbers returns number in range  min to max - both included
  // For floating-point numbers returns number in range from  min (included)) to
  // max (excluded). Can be used both on instantiated objects of class and
  // as a static member function as well. Works for various integral and
  // floating point types.
  // Because all of the data members are static objects, the user can create
  // objects of the class where convenient whithout excessive cost
  template <typename T>
  static T Random(T min, const T max)
  {
    // if T is a integral type
    if constexpr (std::is_integral_v<T>) {
      // define uniform integeral number distribution for the given range
      std::uniform_int_distribution<T> distrib(min, max);
      // get and return random number from distribution using engine
      return distrib(engine);
    }
    // if T is a floating point type
    else if (std::is_floating_point_v<T>) {
      // define uniform floating-point number distribution for the given
      // range
      std::uniform_real_distribution<T> distrib(min, max);
      // get and return random number from distribution using engine
      return distrib(engine);
    }
    // for unpredicted type return the default value type
    else {
      return T {};
    }
  }
  // Alternative function operator doing the same as Random() but on
  // instantiated objects only
  template <class T>
  T operator()(T min, T max) const
  {
    return Random(min, max);
  }

  private:
  // random device used for seeding the random engine
  static std::random_device device;
  // mersenne twister engine used for generating random numbers
  static std::mt19937 engine;
};

} // end of namespace gMath
#endif // !RandNum_hpp
