#include "RandNum.hpp"

namespace gMath {

// define static random device for seeding random engine
std::random_device RandNum::_device;

// define mersenne twister engine seeding it with a random device
std::mt19937 RandNum::_engine(_device());

} // end of namespace gMath