#ifndef GAME_HPP
#define GAME_HPP
#include "Ball.hpp"
#include "Controller.hpp"
#include "Paddle.hpp"
#include "RandNum.hpp"
#include "Renderer.hpp"
#include "SideWall.hpp"
#include "Texture.hpp"
#include <cstddef>
#include <memory>
#include <unordered_map>
#include <vector>

class Game
{

  public:
  // constructor
  Game(const std::size_t screenHeight, const std::size_t screenWidth,
      const std::size_t targetFrameRate);
  // destructor
  ~Game();

  // deleted copy and move operations as the project design predicts only one
  // game object in the entire program
  Game(const Game&) = delete;
  Game(Game&&) = delete;
  Game& operator=(const Game&) = delete;
  Game& operator=(Game&&) = delete;

  // runs the game ; implements main game loop
  void Run();

  private:
  // initialize SDL subsystems
  void InitSubsystems();
  // updates game state
  void UpdateGame();
  // generates game output
  void GenerateOutput() const; // NOTE: const might not be good
  // load all textures used in the game //NOTE: verify
  void LoadTextures();
  // gets a single texture from the stored textures
  const Texture& GetTexture(const std::string& textureName) const;
  // creates the wall limiting the game area
  void CreateWalls();
  // creates the top wall
  void CreateTopWall();
  // creates the left wall
  void CreateLeftWall();
  // creates the right wall
  void CreateRightWall();
  // creates the ball
  void CreateBall();
  // creates the paddle
  void CreatePaddle();

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
  // container for side walls
  std::vector<SideWall> _side_walls;
  // tickness of the walls in pixels
  const int _wall_tickness = 10;
  // pointer to the ball
  std::unique_ptr<Ball> _ball;
  // ball starting scalar speed (pixels per second)
  float _ball_speed = 160.0f;
  // pointer to the paddle
  std::unique_ptr<Paddle> _paddle;
  // paddle scalar moving speed
  const float _paddle_speed = 160.0f;
  // randomizer used for getting random numbers
  gMath::RandNum _randomizer;
};
#endif // !GAME_HPP
