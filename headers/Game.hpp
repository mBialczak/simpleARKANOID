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
#include "SDLInitializers.hpp"
#include "SideWall.hpp"
#include "TextElement.hpp"
#include "Texture.hpp"
#include <cstddef>
#include <memory>
#include <unordered_map>
#include <vector>

// class forward declarations
class Controller;
class SDLinitializer;

// main game class
class Game
{
  public:
  // Constructor. Takes game window height and width, desired FPS rate and
  // numbers off game levels implemented.
  // Subobjects will throw exceptions if initialization fails (SDLexception and
  // exceptions derived from std::exception)
  Game(const std::size_t screenHeight, const std::size_t screenWidth,
      const std::size_t targetFrameRate, unsigned levelsImplemented);

  // RAII mechanism allow to use defalut destructor
  ~Game() = default;

  // deleted copy operations as the project design predicts only one game object
  // in the entire program and many objects are handled by unique pointers
  Game(const Game&) = delete;
  Game& operator=(const Game&) = delete;

  // runs the game; implements main game loop
  void Run();
  // quits the game by stopping the main loop
  void Quit() { is_running_ = false; }
  // Restarts the game
  void Restart();
  // pauses or unpauses the game (pause on/off)
  void TogglePause();
  // returns game state, that is the current bahaviour mode
  GameState State() const { return state_; }
  // handles the ball leaving the allowed screen area
  void HandleBallEscape();
  // handles a block being hit by the ball
  void HandleBlockHit(Block& block);
  // returns the speed increment applied every time the
  // user orders so
  float SpeedIncrement() const { return speed_increment_; };
  // replays the sound corresponding to the sound enum code
  void PlaySound(Sound sound) const { audio_->PlaySound(sound); }

  private:
  // Perfoms actions in routine game state
  void RoutineGameActions();
  // Perfoms actions when the game is paused
  void PausedGameActions();
  // Loads new level. Returns true if new level loaded successfully,
  // false if the current level was the last one implemented
  bool LoadNewLevel(unsigned newLevel);
  // updates the state of the game objects
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

  // RAII class object responsible for initializing and closing SDL subsystems
  SDLinitializer sdl_initializer_;
  // RAII class object responsible for initializing and closing SDL_Image
  // support
  ImageInitializer image_initializer_;
  // RAII class object responsible for initializing and True Type Font support
  TTFinitializer ttf_initializer_;
  // window size properties
  const std::size_t screen_height_;
  const std::size_t screen_width_;
  // unique pointer to renderer
  std::unique_ptr<Renderer> renderer_;
  // number levels implemented by the developer
  unsigned max_level_;
  // controls if the main loop is running
  bool is_running_ = true;
  // controlls the bahaviour of main loop
  GameState state_ = GameState::Paused;
  // target frame rate of the display
  const Uint32 frame_rate_;
  // speed increase applied when the user orders so
  const float speed_increment_ = 30.0f;
  // container with all image textures used in the game
  std::unordered_map<Sprite, Texture> images_;
  // container with texts constructed once for the entire game duration
  std::vector<TextElement> texts_;
  // pointer to AudioMixer responsible for sound support
  std::unique_ptr<AudioMixer> audio_;
  // pointer to controller for handling keyboard input
  std::unique_ptr<Controller> controller_;
  // container for side walls
  std::vector<SideWall> side_walls_;
  // pointer to the ball
  std::unique_ptr<Ball> ball_;
  // pointer to the paddle
  std::unique_ptr<Paddle> paddle_;
  // container of all the blocks in the current level
  std::vector<Block> blocks_;
  // Containers grouping all the static and movable objects to be displayed on
  // the game screen. (for display purposes only, objects are managed by other
  // members)
  std::vector<const StaticObject*> static_for_game_screen_;
  std::vector<const MovableObject*> movable_for_game_screen_;
  // takes track of points achieved by the player
  unsigned total_points_ = 0;
  // inlcudes all the current level data
  std::unique_ptr<LevelData> level_data_;
  // number of remaining lives (balls) before "game over"
  unsigned balls_remaining_;
  // timer regulating the updates of the game state
  IntervalTimer timer_;
};

#endif // !GAME_HPP
