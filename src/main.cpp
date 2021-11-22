#include "SDLexception.hpp"
#include "game.hpp"
#include "timer.hpp" //NOTE: remove if no longer needed
#include <chrono> //NOTE: remove if no longer needed
#include <ctime> //NOTE: remove if no longer needed
#include <exception>
#include <iostream>

int main(int argc, char const* argv[])
{
  // constants for game settings
  constexpr std::size_t cScreenHeight { 600 };
  constexpr std::size_t cScreenWidth { 800 };
  constexpr std::size_t cFramesPerSecond { 60 };

  try {

    Game simpleArkanoid(cScreenHeight, cScreenWidth, cFramesPerSecond);
    simpleArkanoid.Run();

    std::cout << "program terminated normally" << std::endl;
    return 0;
  }
  // handle SDL-related exceptions
  catch (const SDLexception& e) {
    std::cerr << e.Description();
    // NOTE: probably remove in the final version
  }
  // handle standard exceptions
  catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
    return -1;
  }
  // handle unpredicted exceptions
  catch (...) {
    std::cerr << "Unknown exception occured!" << std::endl;
    return -2;
  }
}
