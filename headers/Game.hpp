#ifndef GAME_HPP
#define GAME_HPP
#include "Controller.hpp"
#include "Renderer.hpp"
#include <cstddef>

class Game {

  public:
  // constructor
  Game(const std::size_t screenHeight, const std::size_t screenWidth,
      const std::size_t targetFrameRate);
  // destructor
  ~Game();

  // deleted copy operations for copy prevention
  Game(const Game&) = delete;
  Game& operator=(const Game&) = delete;

  // implements main game loop
  void Run();

  private:
  // updates game state
  void UpdateGame();
  // generates game output
  void GenerateOutput() const; // NOTE: const might not be good

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
