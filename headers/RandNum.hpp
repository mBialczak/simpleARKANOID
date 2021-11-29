#ifndef RandNum_hpp
#define RandNum_hpp

#include <random>
#include <type_traits>

namespace gMath {

// class for getting random numbers
class RandNum
{
  public:
  // Gets random integral or floating point number depending of parameter type.
  // Can be used both on objects of class and as static function
  // works for various integral and floating point types
  template <typename T>
  static T Random(T min, T max)
  {
    // if T is a integral type
    if constexpr (std::is_integral_v<T>) {
      // define uniform integeral number distribution for the given range
      std::uniform_int_distribution<T> distrib(min, max);
      // get and return random number from distribution using engine
      return distrib(_engine);
    } // if T is a floating point type
    else if (std::is_floating_point_v<T>) {
      // define uniform floating-point number distribution for the given
      // range
      std::uniform_real_distribution<T> distrib(min, max);
      // get and return random number from distribution using engine
      return distrib(_engine);
    }
  }
  // Alternative function operator doing the same as Random()
  template <class T>
  T operator()(T min, T max)
  {
    return Random(min, max);
  }

  private:
  // random device used for seeding the random engine
  static std::random_device _device;
  // mersenne twister engine used for generating random numbers
  static std::mt19937 _engine;
};

} // end of namespace gMath
#endif // !RandNum_hpp
