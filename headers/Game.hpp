#ifndef GAME_HPP
#define GAME_HPP
#include "Ball.hpp"
#include "Block.hpp"
#include "Controller.hpp"
#include "LevelData.hpp"
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

  // handles the ball leaving the allowed screen area
  // TODO: to implement
  void HandleBallEscape();
  // handles a block being hit by the ball
  // TODO: to implement
  void HandleBlockHit(Block& block);

  private:
  // initialize SDL subsystems
  void InitSubsystems();
  // updates game state
  void UpdateGame();
  // generates game output
  void GenerateOutput() const; // NOTE: const might not be good
  // load all textures used in the game //NOTE: verify
  void LoadTextures();
  // VERIFY if needed shuld be in the level class
  // Reads the level data from the file. Takes the number representing the
  // level and returns the table representing blocks composition on the screan
  std::vector<std::vector<std::string>> LoadLevelData(unsigned level);
  // gets a single texture from the stored textures
  const Texture& GetTexture(Sprite sprite) const;
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
  // creates blocks
  void CreateBlocks();

  // controls main loop run
  bool _is_running = false;
  // window size properties
  const std::size_t _screen_height;
  const std::size_t _screen_width;
  // REMOVE  and remove if in level class size of the blocks row on the screen
  const std::size_t _row_size = 20;
  // REMOVE maximum number of block rows on the screen
  const std::size_t _max_rows = 15;
  // target frame rate of the display
  const Uint32 _frame_rate;
  // container with all textures used in the game //NOTE: verify
  std::unordered_map<Sprite, std::unique_ptr<Texture>> _textures;

  // controller for handling keyboard input
  Controller _controller;
  // OWNED pointer to renderer // VERIFY if unique_ptr possible
  Renderer* _renderer;
  // container for side walls
  std::vector<SideWall> _side_walls;
  // pointer to the ball
  std::unique_ptr<Ball> _ball;
  // REMOVE if in level class
  // ball starting scalar speed (pixels per second)
  float _ball_speed = 250.0f;
  // pointer to the paddle
  std::unique_ptr<Paddle> _paddle;
  // REMOVE if in level class
  // paddle scalar moving speed
  const float _paddle_speed = 300.0f;
  // randomizer used for getting random numbers
  gMath::RandNum _randomizer;
  // TODO: change into container and create comments
  std::vector<Block> _blocks;
  // takes track of points achieved by the player
  unsigned _total_points = 0;
  // REMOVE current game level
  std::size_t _level = 1;
  // inlcudes all the current level data
  std::unique_ptr<LevelData> _level_data;
};
#endif // !GAME_HPP
