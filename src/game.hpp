#ifndef GAME_HPP
#define GAME_HPP
#include "controller.hpp"
#include "renderer.hpp"
#include <cstddef>

class Game {

  public:
  // constructor
  Game(const std::size_t screenHeight, const std::size_t screenWidth,
      const std::size_t targetFrameRate);
  // destructor
  ~Game();

  // implements main game loop
  void Run();

  private:
  // window size properties
  const std::size_t screen_height;
  const std::size_t screen_width;

  // target frame rate of the display
  const Uint32 frame_rate;

  Controller controller;
  // owned pointers
  Renderer* renderer;
};
#endif // !GAME_HPP
