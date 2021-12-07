#ifndef GAME_HPP
#define GAME_HPP
#include "Ball.hpp"
#include "Block.hpp"
#include "Controller.hpp"
#include "IntervalTimer.hpp"
#include "LevelData.hpp"
#include "Paddle.hpp"
#include "RandNum.hpp"
#include "Renderer.hpp"
#include "SideWall.hpp"
#include "TextElement.hpp" // REVIEW
#include "Texture.hpp"
#include <cstddef>
#include <memory>
#include <unordered_map>
#include <vector>

// class forward declarations
class Controller;

// COMMENT
// main game class
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
  // pauses or unpauses the game (pause on/off)
  void TogglePause();
  // returns true if the game is paused, false if not
  bool isPaused() const { return _paused; }
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
  bool _is_running = false; // NOTE: rename?
  //  controls if the game is paused
  bool _paused = false;
  // window size properties
  const std::size_t _screen_height;
  const std::size_t _screen_width;
  // target frame rate of the display
  const Uint32 _frame_rate;
  // REVIEW: rename to "images?"
  // container with all image textures used in the game
  std::unordered_map<Sprite, std::unique_ptr<Texture>> _images;

  //  REVIEW: container with static text elements to be displayed
  // on the welcome/pause screen
  // std::vector<TextElement> _texts;
  // controller for handling keyboard input
  std::unique_ptr<Controller> _controller;
  // OWNED pointer to renderer // VERIFY if unique_ptr possible
  Renderer* _renderer;
  // container for side walls
  std::vector<SideWall> _side_walls;
  // pointer to the ball
  std::unique_ptr<Ball> _ball;
  // pointer to the paddle
  std::unique_ptr<Paddle> _paddle;
  // randomizer used for getting random numbers
  gMath::RandNum _randomizer;
  // container of all the blocks in the current level
  std::vector<Block> _blocks;
  // REVIEW: perhaps can be put into one drawable parent class
  // Group all the static and movable objects to be displayed on the game
  // screen. (for display purposes only, objects are managed by other members)
  std::vector<const StaticObject*> _static_for_game_screen;
  std::vector<const MovableObject*> _movable_for_game_screen;
  // takes track of points achieved by the player
  unsigned _total_points = 0;
  // REMOVE if used from LevelData class current game level
  std::size_t _level = 1;
  // inlcudes all the current level data
  std::unique_ptr<LevelData> _level_data;
  // REMOVE INU
  // number of remaining lives (balls) befor "game over"
  unsigned _lives_remaining;
  // timer regulating the updates of the game state
  IntervalTimer _timer;
};
#endif // !GAME_HPP
