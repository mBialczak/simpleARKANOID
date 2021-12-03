#include "Game.hpp"
#include "IntervalTimer.hpp" //NOTE: remove if no longer needed
#include "SDL.h" //NOTE: remove if no longer needed
#include "SDLexception.hpp"
#include <exception>
#include <iostream>

int main(int argc, char const* argv[])
{
  // constants for game settings
  constexpr std::size_t c_screen_width { 1000 };
  constexpr std::size_t c_screen_height { 800 };
  constexpr std::size_t c_frames_per_second { 60 };

  try {

    Game simple_arkanoid(c_screen_height, c_screen_width, c_frames_per_second);

    simple_arkanoid.Run();

    std::cout << "program terminated normally" << std::endl;
    return 0;
  }
  // handle SDL-related exceptions
  catch (const SDLexception& e) {
    std::cerr << e.Description();
    return -3;
  }
  // handle standard iherited exceptions
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
