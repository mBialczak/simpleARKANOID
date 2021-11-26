#ifndef GAME_HPP
#define GAME_HPP
#include "Ball.hpp"
#include "Controller.hpp"
#include "Renderer.hpp"
#include "Texture.hpp"
#include <cstddef>
#include <memory>
#include <unordered_map>

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
  // load all textures used in the game //NOTE: verify
  void LoadTextures();
  // gets a single texture from the stored textures
  const Texture& GetTexture(const std::string& TextureName) const;

  // window size properties
  const std::size_t _screen_height;
  const std::size_t _screen_width;
  // target frame rate of the display
  const Uint32 _frame_rate;
  // container with all textures used in the game //NOTE: verify
  std::unordered_map<std::string, std::unique_ptr<Texture>> _textures;

  // controller for handling keyboard input
  Controller _controller;
  // owned pointers
  Renderer* _renderer;
  // pointer to the ball
  std::unique_ptr<Ball> _ball;
  // ball starting scalar speed (pixels per second)
  float _ball_speed = 50.0f;
};
#endif // !GAME_HPP
