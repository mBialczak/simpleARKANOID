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
  const std::size_t _screen_height;
  const std::size_t _screen_width;
  // target frame rate of the display
  const Uint32 _frame_rate;
  // controller for handling keyboard output
  Controller _controller;
  // owned pointers
  Renderer* _renderer;
};
#endif // !GAME_HPP
