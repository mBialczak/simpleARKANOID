#include "RandNum.hpp"

// namespace for grouping objects and functions required for the game
// calculations
namespace gMath {

// define static random device for seeding random engine
std::random_device RandNum::device;

// define mersenne twister engine seeding it with a random device
std::mt19937 RandNum::engine(device());

} // end of namespace gMath