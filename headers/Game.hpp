#ifndef GAME_HPP
#define GAME_HPP
#include "AudioMixer.hpp"
#include "Ball.hpp"
#include "Block.hpp"
#include "Controller.hpp"
#include "IntervalTimer.hpp"
#include "LevelData.hpp"
#include "Paddle.hpp"
#include "RandNum.hpp"
#include "Renderer.hpp"
#include "SideWall.hpp"
#include "TextElement.hpp"
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
  // COMMENT
  // constructor
  Game(const std::size_t screenHeight, const std::size_t screenWidth,
      const std::size_t targetFrameRate, unsigned levelsImplemented);
  // destructor
  ~Game();

  // deleted copy and move operations as the project design predicts only one
  // game object in the entire program
  Game(const Game&) = delete;
  Game(Game&&) = delete;
  Game& operator=(const Game&) = delete;
  Game& operator=(Game&&) = delete;

  // runs the game; implements main game loop
  void Run();
  // quits the game by stopping the main loop
  void Quit() { _is_running = false; }
  // Restarts the game
  void Restart();
  // pauses or unpauses the game (pause on/off)
  void TogglePause();
  // returns game state, that is the current bahaviour mode
  GameState State() const { return _state; }
  // handles the ball leaving the allowed screen area
  void HandleBallEscape();
  // handles a block being hit by the ball
  void HandleBlockHit(Block& block);
  // returns the speed increment applied every time the
  // user orders so
  float SpeedIncrement() const { return _speed_increment; };
  // replays the sound correspondin to the sound enum code
  void PlaySound(Sound sound) const { _audio->PlaySound(sound); }

  private:
  // initialize SDL subsystems
  void InitSubsystems();
  // Perfoms actions in routine game state
  void RoutineGameActions();
  // Perfoms actions when the game is paused
  void PausedGameActions();
  // Loads new level. Returns true if new level loaded successfully,
  // false if the current level was the last one implemented
  bool LoadNewLevel(unsigned newLevel);
  // updates game state
  void UpdateGame();
  // Displays the pause screen when the game is paused
  void DisplayPauseScreen() const;
  // Displays the screen after ball leaving the screen
  void DisplayBallLostScreen() const;
  // Displays the screen after the level has been completed
  void DisplayLevelCompleted() const;
  // Displays the screen after all lives/balls have been lost
  void DisplayGameOverScreen() const;
  // Displays the screen when the game is won
  void DisplayGameWonScreen() const;
  // load all image textures used in the game
  void LoadImages();
  // Initializes audio mixer system and loads all the sound
  // effects to be used in the game
  void LoadAudio();
  // gets a single image texture from the stored textures
  const Texture& GetTexture(Sprite sprite) const;
  // creates all the texts which won't change for entire game
  void CreateTexts();
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

  // number levels implemented by the developer
  unsigned _max_level;
  // controls if the main loop is running
  bool _is_running = true; // NOTE: rename?
  // controlls the bahaviour of main loop
  GameState _state = GameState::Paused;
  // window size properties
  const std::size_t _screen_height;
  const std::size_t _screen_width;
  // target frame rate of the display
  const Uint32 _frame_rate;
  // speed increase applied when the user orders so
  const float _speed_increment = 40.0f;
  // container with all image textures used in the game
  std::unordered_map<Sprite, Texture> _images;
  // container with texts constructed once for the entire game duration
  std::vector<TextElement> _texts;
  // pointer to AudioMixer responsible for sound support
  std::unique_ptr<AudioMixer> _audio;
  // pointer to controller for handling keyboard input
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
  // Group all the static and movable objects to be displayed on the game
  // screen. (for display purposes only, objects are managed by other members)
  std::vector<const StaticObject*> _static_for_game_screen;
  std::vector<const MovableObject*> _movable_for_game_screen;
  // takes track of points achieved by the player
  unsigned _total_points = 0;
  // inlcudes all the current level data
  std::unique_ptr<LevelData> _level_data;
  // number of remaining lives (balls) before "game over"
  unsigned _balls_remaining;
  // timer regulating the updates of the game state
  IntervalTimer _timer;
};
#endif // !GAME_HPP
