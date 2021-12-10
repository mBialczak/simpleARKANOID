#include "Game.hpp"
#include "IntervalTimer.hpp" //NOTE: remove if no longer needed
#include "SDL.h" //NOTE: remove if no longer needed
#include "SDLexception.hpp"
#include <exception>
#include <iostream>

int main(int argc, char const* argv[])
{
  // constants for game settings
  constexpr std::size_t c_screen_width { 1024 };
  constexpr std::size_t c_screen_height { 768 };
  constexpr std::size_t c_frames_per_second { 60 };
  constexpr unsigned c_implemented_levels { 2 };

  try {

    Game simple_arkanoid(c_screen_height, c_screen_width, c_frames_per_second,
        c_implemented_levels);

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
