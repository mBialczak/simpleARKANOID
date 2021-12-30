#include "Game.hpp"
#include "Colors.hpp"
#include "IntervalTimer.hpp"
#include "LimitTimer.hpp"
#include "Paths.hpp"
#include "SDL.h"
#include "SDLexception.hpp"
#include <algorithm>
#include <exception>

// Constructor. Takes game window height and width, desired FPS rate and numbers
// off game levels implemented.
// Subobjects will throw exceptions if initialization fails (SDLexception and
// exceptions derived from std::exception)
Game::Game(const std::size_t screenHeight, const std::size_t screenWidth,
    const std::size_t targetFrameRate, unsigned levelsImplemented)
    : // try to initialize SDL video and audio subsystems
    sdl_initializer_(SDLinitializer(SDL_INIT_VIDEO | SDL_INIT_AUDIO))
    // try to initialize SDL_Image support for PNG files
    , image_initializer_(ImageInitializer(IMG_INIT_PNG))
    // try to initialize True Type Font support
    , ttf_initializer_(TTFinitializer())
    , screen_height_(screenHeight)
    , screen_width_(screenWidth)
    , renderer_(std::make_unique<Renderer>(screenHeight, screenWidth))
    , max_level_(levelsImplemented)
    , frame_rate_(targetFrameRate)
    , audio_(nullptr)
    , controller_(std::make_unique<Controller>(*this))
    // load all the data for the first level
    , level_data_(std::make_unique<LevelData>(Paths::pLevels))
    , balls_remaining_(level_data_->Lives())
{
  // load images used in the game
  LoadImages();

  // Initialize audio mixer system and load all the sound
  // effects to be used in the game
  LoadAudio();

  // reserve space in the cointainer storing pointers of static
  // objects to be displayed in the game
  static_for_game_screen_.reserve(
      LevelData::max_rows * LevelData::row_size + 3);

  // Create all the game visible components
  CreateWalls();
  CreateBlocks();
  CreatePaddle();
  CreateBall();

  // create all displayable text elements which will not change in the game
  CreateTexts();
}

// runs the game; implements main game loop
void Game::Run()
{
  // calculate desired duration of a single frame
  const Uint32 desired_frame_duration { static_cast<Uint32>(
      IntervalTimer::ticks_per_second / frame_rate_) };

  // create timer used for for FPS limiting
  LimitTimer frame_timer { desired_frame_duration };

  // main game loop
  while (is_running_) {
    // handle the game input
    controller_->HandleInput(is_running_, *paddle_, *ball_);

    // depending on the current game state dispatch the control
    // to proper helper function
    switch (state_) {
      case GameState::Routine:
        RoutineGameActions();
        break;
      case GameState::Paused:
        PausedGameActions();
        break;
      case GameState::Over:
        DisplayGameOverScreen();
        break;
      case GameState::Won:
        DisplayGameWonScreen();
        break;
      default:
        // report error if unexpected game state was received
        throw std::runtime_error(
            "Uknown game state occured in the main game loop!");
    }

    // execute frame FPS limiting policy by waiting untill
    // each frame time completes
    frame_timer.waitTillExpire();
    // restart frameTimer for the next frame
    frame_timer.restart();
  }
}

// Restarts the game
void Game::Restart()
{
  // Load level one once again
  LoadNewLevel(1);

  total_points_ = 0;

  // mark correct game state
  state_ = GameState::Routine;
}

// pauses or unpauses the game (pause on/off)
void Game::TogglePause()
{
  if (state_ == GameState::Paused) {
    state_ = GameState::Routine;
  }
  else {
    state_ = GameState::Paused;
  }
}

// Perfoms actions in routine game state
void Game::RoutineGameActions()
{
  // update state of the game objects (ball, paddle, blocks, etc.)
  UpdateGame();
  // Display the game screen
  renderer_->DisplayScreen(static_for_game_screen_, movable_for_game_screen_);

  // Load next level if all the blocks have been destroyed
  if (std::all_of(blocks_.begin(), blocks_.end(),
          [](const Block& block) { return block.IsDestroyed(); })) {

    // if loading a new level succeds
    if (LoadNewLevel(level_data_->Level() + 1)) {
      PlaySound(Sound::LevelCompleted);

      DisplayLevelCompleted();
      // halt execution for the the time of display;
      SDL_Delay(4000);
    }
    // LoadNewLevel returns fasle only when there are no more levels,
    // so the game is won
    else {
      state_ = GameState::Won;
      PlaySound(Sound::GameWon);
    }
  }
}

// Perfoms actions in when the game is paused
void Game::PausedGameActions()
{
  // Pause the game timer for correct update calculations.
  // If not paused, the game update calculations will break
  // all the display after unpausing
  timer_.Pause();

  DisplayPauseScreen();
}

// Loads new level. Returns true if new level loaded successfully,
// false if the current level was the last one implemented
bool Game::LoadNewLevel(unsigned newLevel)
{
  // if the current_level was the last one implemented,
  // false indicates that the game has reached the end
  if (newLevel > max_level_) {
    return false;
  }

  // load all the data for the new level
  level_data_ = std::make_unique<LevelData>(Paths::pLevels, newLevel);

  // set player ramaining balls/lives
  balls_remaining_ = (level_data_->Lives());

  // clear the vector of static objects to be displayed
  // and reserve enough space for new, reloaded one
  static_for_game_screen_.clear();
  static_for_game_screen_.reserve(
      LevelData::max_rows * LevelData::row_size + 3);

  // add walls to the list of static objects to be displayed
  for (auto& wall : side_walls_) {
    static_for_game_screen_.emplace_back(&wall);
  };

  // empty the container of blocks and create  new ones for the level
  blocks_.clear();
  CreateBlocks();

  // set paddle position to default and speed to paddle level speed
  paddle_->SetPosition(
      screen_width_ / 2.0f, screen_height_ - paddle_->HalfHeight());
  paddle_->SetSpeed(level_data_->PaddleSpeed());

  // reset ball speed and place it on the paddle
  ball_->Reset(level_data_->BallSpeed());

  // level loaded succesfully
  return true;
}

// updates the state of the game objects
void Game::UpdateGame()
{
  // calculate delta time and udpate timer
  auto delta_time = timer_.UpdateAndGetInterval();
  // upate paddle state
  paddle_->Update(delta_time);
  // update ball state
  ball_->Update(delta_time);
}

// load all image textures used in the game
void Game::LoadImages()
{
  // load texture representing the ball
  images_[Sprite::Ball]
      = Texture(Paths::pBallImage, renderer_->GetSDLrenderer());

  // load texture respresenting the paddle
  images_[Sprite::Paddle]
      = Texture(Paths::pPadleImage, renderer_->GetSDLrenderer());

  // load texture representing the top_wall
  images_[Sprite::WallHorizontal]
      = Texture(Paths::pHorizontalWallImage, renderer_->GetSDLrenderer());

  // load texture representing side_walls
  images_[Sprite::WallVertical]
      = Texture(Paths::pVerticalWallImage, renderer_->GetSDLrenderer());

  // load texture representing a green block
  images_[Sprite::BlockGreen]
      = Texture(Paths::pBlockGreenImage, renderer_->GetSDLrenderer());

  // load texture representing a silver block
  images_[Sprite::BlockSilver]
      = Texture(Paths::pBlockSilverImage, renderer_->GetSDLrenderer());

  // load texture representing a blue block
  images_[Sprite::BlockBlue]
      = Texture(Paths::pBlockBlueImage, renderer_->GetSDLrenderer());

  // load texture representing an orange block
  images_[Sprite::BlockOrange]
      = Texture(Paths::pBlockOrangeImage, renderer_->GetSDLrenderer());

  // load texture representing a purple block
  images_[Sprite::BlockPurple]
      = Texture(Paths::pBlockPurpleImage, renderer_->GetSDLrenderer());

  // load texture representing a red block
  images_[Sprite::BlockRed]
      = Texture(Paths::pBlockRedImage, renderer_->GetSDLrenderer());

  // load texture representing a teal block
  images_[Sprite::BlockTeal]
      = Texture(Paths::pBlockTealImage, renderer_->GetSDLrenderer());

  // load texture representing a yellow block
  images_[Sprite::BlockYellow]
      = Texture(Paths::pBlockYellowImage, renderer_->GetSDLrenderer());
}

// Initializes audio mixer system and loads all the sound
// effects to be used in the game
void Game::LoadAudio()
{
  // create map assigning paths of audio files to enum sound codes
  std::unordered_map<Sound, std::string> sound_vs_path
      = { { Sound::BallPaddleHit, Paths::pSoundBallBouncePaddle },
          { Sound::BallBounceWall, Paths::pSoundBallBounceWall },
          { Sound::BlockHit, Paths::pSoundBlockDestroyed },
          { Sound::BallLost, Paths::pSoundBallLost },
          { Sound::LevelCompleted, Paths::pSoundLevelCompleted },
          { Sound::GameWon, Paths::pSoundGameWon } };
  // create audio mixer sending the created container
  audio_ = std::make_unique<AudioMixer>(sound_vs_path);
}

// creates all the texts which won't change for the entire game
void Game::CreateTexts()
{
  // helper const for marking centre x position
  const float horizontal_centre = screen_width_ / 2.0f;

  // Create text for the top
  std::string top { "Thank's for trying out my game:" };
  const float top_x = horizontal_centre;
  const float top_y = 30.0f;
  texts_.emplace_back(top_x, top_y, Paths::pFontRobotoRegular, Color::Yellow,
      30, renderer_->GetSDLrenderer(), top);

  // Create text for game title
  std::string title { " s i m p l e   A r k a n o i d  ! ! !" };
  const float title_x = horizontal_centre;
  const float title_y = top_y + 70.0f;
  texts_.emplace_back(title_x, title_y, Paths::pFontRobotoBoldItalic,
      Color::Green, 70, renderer_->GetSDLrenderer(), title);

  // Create text about the paused state
  std::string paused { "Game PAUSED" };
  const float paused_x = horizontal_centre;
  const float paused_y = title_y + 70.0f;
  texts_.emplace_back(paused_x, paused_y, Paths::pFontRobotoBold, Color::Red,
      36, renderer_->GetSDLrenderer(), paused);

  // Create instruction about unpausing
  std::string unpause { "( press 'ESCAPE' key to unpause )" };
  const float unpause_x = horizontal_centre;
  const float unpause_y = paused_y + 40.0f;
  texts_.emplace_back(unpause_x, unpause_y, Paths::pFontRobotoRegular,
      Color::Red, 22, renderer_->GetSDLrenderer(), unpause);

  // spacer for the area where dynamically created text whill apear
  const float spacer = 240.0f;

  // Create "Game Instructions" text
  std::string instructions {
    "G   A   M   E       C   O   N   T   R   O   L   S"
  };
  const float instr_x = horizontal_centre;
  const float instr_y = unpause_y + spacer;
  texts_.emplace_back(instr_x, instr_y, Paths::pFontRobotoBold, Color::Violet,
      32, renderer_->GetSDLrenderer(), instructions);

  // spacer between instructions
  const float instr_spacer = 35.0f;
  // 1st line of instructions
  std::string line_1 { "S P A C E   -   start the ball from the paddle" };
  const float line_1_x = horizontal_centre;
  const float line_1_y = instr_y + instr_spacer + 10.0f;
  texts_.emplace_back(line_1_x, line_1_y, Paths::pFontRobotoRegular,
      Color::Yellow, 22, renderer_->GetSDLrenderer(), line_1);

  // 2nd line of instructions
  std::string line_2 { "E S C A P E  -   pause / unpause the game" };
  const float line_2_x = horizontal_centre;
  const float line_2_y = line_1_y + instr_spacer;
  texts_.emplace_back(line_2_x, line_2_y, Paths::pFontRobotoRegular,
      Color::Orange, 22, renderer_->GetSDLrenderer(), line_2);

  // 3rd line of instructions
  std::string line_3 {
    "L E F T   and   R I G H T  arrows  -  move the paddle HORIZONTALLY"
  };
  const float line_3_x = horizontal_centre;
  const float line_3_y = line_2_y + instr_spacer;
  texts_.emplace_back(line_3_x, line_3_y, Paths::pFontRobotoRegular,
      Color::Yellow, 22, renderer_->GetSDLrenderer(), line_3);

  // 4th line of instructions
  std::string line_4 { "U P   and   D O W N   arrows  -  move the paddle "
                       "VERTICALLY (within allowed limits)" };
  const float line_4_x = horizontal_centre;
  const float line_4_y = line_3_y + instr_spacer;
  texts_.emplace_back(line_4_x, line_4_y, Paths::pFontRobotoRegular,
      Color::Orange, 22, renderer_->GetSDLrenderer(), line_4);

  // 5th line of instructions
  std::string line_5 { "THESE  WORK  ONLY  WHEN  HELD  WHILE  "
                       "THE  BALL  HITS  THE  PADDLE :" };
  const float line_5_x = horizontal_centre;
  const float line_5_y = line_4_y + instr_spacer + 10.0f;
  texts_.emplace_back(line_5_x, line_5_y, Paths::pFontRobotoRegular,
      Color::Violet, 26, renderer_->GetSDLrenderer(), line_5);

  // 6th line of instructions
  std::string line_6 {
    "A -  spin the ball to the left          F -  spin the ball to the right"
  };
  const float line_6_x = horizontal_centre;
  const float line_6_y = line_5_y + instr_spacer;
  texts_.emplace_back(line_6_x, line_6_y, Paths::pFontRobotoRegular,
      Color::Orange, 22, renderer_->GetSDLrenderer(), line_6);

  // 7th line of instructions
  std::string line_7 { "E - speed up the ball (steps with no limit)     D - "
                       "slow down the ball (no more than  the level minimum)" };
  const float line_7_x = horizontal_centre;
  const float line_7_y = line_6_y + instr_spacer;
  texts_.emplace_back(line_7_x, line_7_y, Paths::pFontRobotoRegular,
      Color::Yellow, 22, renderer_->GetSDLrenderer(), line_7);
}

// Generates container of static objects to be displayed on the pause screen
void Game::DisplayPauseScreen() const
{
  // create container of static objects to be displayed
  std::vector<const StaticObject*> all_texts;
  // reserve container space for all predicted elements
  all_texts.reserve(texts_.size() + 6);

  // copy addresses of all stored texts which won't change during game
  for (auto& text : texts_) {
    all_texts.emplace_back(&text);
  }

  // create remaining balls counter display
  std::string balls_str { std::to_string(balls_remaining_) };
  const float balls_x = screen_width_ / 2.0f;
  const float balls_y = screen_height_ / 3.0f + 45.0f;
  TextElement balls { balls_x, balls_y, Paths::pFontRobotoBold, Color::Yellow,
    70, renderer_->GetSDLrenderer(), balls_str };

  // create "remaining balls" text
  std::string remaining_str { "Balls left" };
  const float remaining_x = screen_width_ / 2.0f;
  const float remaining_y = balls_y + 70;
  TextElement remaining { remaining_x, remaining_y, Paths::pFontRobotoBold,
    Color::Yellow, 36, renderer_->GetSDLrenderer(), remaining_str };

  // create level counter display
  std::string level_str { std::to_string(level_data_->Level()) };
  const float level_x
      = (remaining.Position().X() - (remaining.Width() / 2.0f)) / 2.0f;
  const float level_y = balls_y;
  TextElement level { level_x, level_y, Paths::pFontRobotoBold, Color::Blue, 70,
    renderer_->GetSDLrenderer(), level_str };

  // create "level" text
  std::string lvl_txt_str { "Level" };
  const float lvl_txt_x = level_x;
  const float lvl_txt_y = remaining_y;
  TextElement lvl_txt { lvl_txt_x, lvl_txt_y, Paths::pFontRobotoBold,
    Color::Blue, 36, renderer_->GetSDLrenderer(), lvl_txt_str };

  // create total score counter display
  std::string score_str { std::to_string(total_points_) };
  const float score_x = screen_width_ - level_x;
  const float score_y = balls_y;
  TextElement score { score_x, score_y, Paths::pFontRobotoBold, Color::Green,
    70, renderer_->GetSDLrenderer(), score_str };

  // create "score" text
  std::string score_txt_str { "SCORE" };
  const float score_txt_x = score_x;
  const float score_txt_y = lvl_txt_y;
  TextElement score_txt { score_txt_x, score_txt_y, Paths::pFontRobotoBold,
    Color::Green, 36, renderer_->GetSDLrenderer(), score_txt_str };

  // add text elements to the container
  all_texts.emplace_back(&remaining);
  all_texts.emplace_back(&balls);
  all_texts.emplace_back(&level);
  all_texts.emplace_back(&lvl_txt);
  all_texts.emplace_back(&score);
  all_texts.emplace_back(&score_txt);

  // Display all text on screen
  renderer_->DisplayScreen(all_texts);
}

// Displays the screen after ball leaving the screen
void Game::DisplayBallLostScreen() const
{
  // create container of static objects to be displayed
  std::vector<const StaticObject*> texts;
  // reserve container space for all predicted elements
  texts.reserve(texts_.size() + 5);

  // create "ball lost" text
  std::string ball_out_str { " B A L L   L E F T " };
  const float ball_out_x = screen_width_ / 2.0f;
  const float ball_out_y = screen_height_ / 5.0f;
  TextElement ball_out { ball_out_x, ball_out_y, Paths::pFontRobotoBold,
    Color::Red, 90, renderer_->GetSDLrenderer(), ball_out_str };

  // create "ball lost" text - 2nd part
  std::string ball_out2_str { "T H E   S C R E E N" };
  const float ball_out2_x = screen_width_ / 2.0f;
  const float ball_out2_y = ball_out_y + 100.0f;
  TextElement ball_2out { ball_out2_x, ball_out2_y, Paths::pFontRobotoBold,
    Color::Red, 90, renderer_->GetSDLrenderer(), ball_out2_str };

  // create remaining balls counter display
  std::string balls_str { std::to_string(balls_remaining_) };
  const float balls_x = screen_width_ / 2.0f;
  const float balls_y = screen_height_ / 2.0f + 20.f;
  TextElement balls { balls_x, balls_y, Paths::pFontRobotoBold, Color::Orange,
    120, renderer_->GetSDLrenderer(), balls_str };

  // create "remaining balls" text
  std::string remaining_str { "Ball(s) remaining" };
  const float remaining_x = screen_width_ / 2.0f;
  const float remaining_y = balls_y + 120;
  TextElement remaining { remaining_x, remaining_y, Paths::pFontRobotoBold,
    Color::Orange, 36, renderer_->GetSDLrenderer(), remaining_str };

  // create "resuming" text
  std::string resume_str { "Game will resume in a couple of seconds . . ." };
  const float resume_x = screen_width_ / 2.0f;
  const float resume_y = remaining_y + 100.0f;
  TextElement resume { resume_x, resume_y, Paths::pFontRobotoBold, Color::Green,
    45, renderer_->GetSDLrenderer(), resume_str };

  // add text elements to the container
  texts.emplace_back(&remaining);
  texts.emplace_back(&balls);
  texts.emplace_back(&ball_out);
  texts.emplace_back(&resume);
  texts.emplace_back(&ball_2out);

  // Display all text on screen
  renderer_->DisplayScreen(texts);
}

// Displays the screen after the level has been completed
void Game::DisplayLevelCompleted() const
{
  // create container of static objects to be displayed
  std::vector<const StaticObject*> texts;
  // reserve container space for all predicted elements
  texts.reserve(texts_.size() + 5);

  // create "level" text
  std::string completed_str { "L E V E L   " };
  // need to decrease level number by one, because after loading
  // a new level by now the counter is already set to new level
  completed_str += std::to_string(level_data_->Level() - 1);
  const float completed_x = screen_width_ / 2.0f;
  const float completed_y = screen_height_ / 6.0f;
  TextElement completed { completed_x, completed_y, Paths::pFontRobotoBold,
    Color::Orange, 60, renderer_->GetSDLrenderer(), completed_str };

  // create "congrats" text
  std::string congrats_str { "C O M P L E T E D ! ! !" };
  const float congrats_x = screen_width_ / 2.0f;
  const float congrats_y = completed_y + 110.0f;
  TextElement congrats { congrats_x, congrats_y, Paths::pFontRobotoBold,
    Color::Green, 80, renderer_->GetSDLrenderer(), congrats_str };

  // create total score counter display
  std::string score_str { std::to_string(total_points_) };
  const float score_x = screen_width_ / 2.0f;
  const float score_y = screen_height_ / 2.0f;
  TextElement score { score_x, score_y, Paths::pFontRobotoBold, Color::Yellow,
    120, renderer_->GetSDLrenderer(), score_str };

  // create "total score" text
  std::string score_txt_str { "T O T A L   S C O R E" };
  const float score_txt_x = screen_width_ / 2.0f;
  const float score_txt_y = score_y + 100;
  TextElement score_txt { score_txt_x, score_txt_y, Paths::pFontRobotoBold,
    Color::Yellow, 36, renderer_->GetSDLrenderer(), score_txt_str };

  // create info about starting a new level
  std::string starting_str {
    "S T A R T I N G    N  E  W    L  E  V  E  L  . . ."
  };
  const float starting_x = screen_width_ / 2.0f;
  const float starting_y = screen_height_ * 2.5f / 3.0f;
  TextElement starting { starting_x, starting_y, Paths::pFontRobotoBold,
    Color::Orange, 40, renderer_->GetSDLrenderer(), starting_str };

  // add text elements to the container
  texts.emplace_back(&completed);
  texts.emplace_back(&congrats);
  texts.emplace_back(&score);
  texts.emplace_back(&score_txt);
  texts.emplace_back(&starting);

  // Display all text on screen
  renderer_->DisplayScreen(texts);
}

// Displays the screen after all lives/balls have been lost
void Game::DisplayGameOverScreen() const
{
  // create container of static objects to be displayed
  std::vector<const StaticObject*> texts;
  // reserve container space for all predicted elements
  texts.reserve(texts_.size() + 6);

  // create "game over" text
  std::string g_over_str { "G A M E    O V E R" };
  const float g_over_x = screen_width_ / 2.0f;
  const float g_over_y = screen_height_ / 5.0f;
  TextElement g_over { g_over_x, g_over_y, Paths::pFontRobotoBold, Color::Red,
    90, renderer_->GetSDLrenderer(), g_over_str };

  // create "all ball lost" text
  std::string all_lost_str { "( all balls have been lost )" };
  const float all_lost_x = screen_width_ / 2.0f;
  const float all_lost_y = g_over_y + 70.0f;
  TextElement all_lost { all_lost_x, all_lost_y, Paths::pFontRobotoRegular,
    Color::Red, 30, renderer_->GetSDLrenderer(), all_lost_str };

  // create total score counter display
  std::string score_str { std::to_string(total_points_) };
  const float score_x = screen_width_ / 2.0f;
  const float score_y = screen_height_ / 2.0f - 50.f;
  TextElement score { score_x, score_y, Paths::pFontRobotoBold, Color::Green,
    120, renderer_->GetSDLrenderer(), score_str };

  // create "total score" text
  std::string score_txt_str { "T O T A L   S C O R E" };
  const float score_txt_x = screen_width_ / 2.0f;
  const float score_txt_y = score_y + 100;
  TextElement score_txt { score_txt_x, score_txt_y, Paths::pFontRobotoBold,
    Color::Green, 36, renderer_->GetSDLrenderer(), score_txt_str };

  // create offer of restarting the game text
  std::string restart_str {
    "Press    ' E N T E R   ( R E T U R N ) '    to    restart"
  };
  const float restart_x = screen_width_ / 2.0f;
  const float restart_y = screen_height_ * 2.5f / 3.0f;
  TextElement restart { restart_x, restart_y, Paths::pFontRobotoBold,
    Color::Orange, 45, renderer_->GetSDLrenderer(), restart_str };

  // create offer for quiting text
  std::string quit_str { "Press    ' E S C A P E '    to    quit" };
  const float quit_x = restart_x - 10.0f;
  const float quit_y = restart_y + 80.0f;
  TextElement quit { quit_x, quit_y, Paths::pFontRobotoBold, Color::Blue, 45,
    renderer_->GetSDLrenderer(), quit_str };

  // add text elements to the container
  texts.emplace_back(&g_over);
  texts.emplace_back(&all_lost);
  texts.emplace_back(&score);
  texts.emplace_back(&score_txt);
  texts.emplace_back(&restart);
  texts.emplace_back(&quit);

  // Display all text on screen
  renderer_->DisplayScreen(texts);
}

// Displays the screen when the game is won
void Game::DisplayGameWonScreen() const
{
  // create container of static objects to be displayed
  std::vector<const StaticObject*> texts;
  // reserve container space for all predicted elements
  texts.reserve(texts_.size() + 6);

  // create "game won" text
  std::string won_str { "Y O U    W O N  ! ! !" };
  const float won_x = screen_width_ / 2.0f;
  const float won_y = screen_height_ / 3.0f;
  TextElement won { won_x, won_y, Paths::pFontRobotoBold, Color::Green, 90,
    renderer_->GetSDLrenderer(), won_str };

  // create "Congratulations" text
  std::string congrats_str { "C O N G R A T U L A T I O N S ! ! !" };
  const float congrats_x = screen_width_ / 2.0f;
  const float congrats_y = screen_height_ / 6.5f;
  TextElement congrats { congrats_x, congrats_y, Paths::pFontRobotoRegular,
    Color::Green, 50, renderer_->GetSDLrenderer(), congrats_str };

  // create total score counter display
  std::string score_str { std::to_string(total_points_) };
  const float score_x = screen_width_ / 2.0f;
  const float score_y = screen_height_ / 2.0f;
  TextElement score { score_x, score_y, Paths::pFontRobotoBold, Color::Yellow,
    120, renderer_->GetSDLrenderer(), score_str };

  // create "total score" text
  std::string score_txt_str { "T O T A L   S C O R E" };
  const float score_txt_x = screen_width_ / 2.0f;
  const float score_txt_y = score_y + 100;
  TextElement score_txt { score_txt_x, score_txt_y, Paths::pFontRobotoBold,
    Color::Yellow, 36, renderer_->GetSDLrenderer(), score_txt_str };

  // create offer of restarting the game text
  std::string restart_str {
    "Press   ' E N T E R   ( R E T U R N ) '   to   play   again"
  };
  const float restart_x = screen_width_ / 2.0f;
  const float restart_y = screen_height_ * 2.5f / 3.0f;
  TextElement restart { restart_x, restart_y, Paths::pFontRobotoBold,
    Color::Orange, 40, renderer_->GetSDLrenderer(), restart_str };

  // create offer for quiting text
  std::string quit_str { "Press    ' E S C A P E '    to    quit" };
  const float quit_x = restart_x - 10.0f;
  const float quit_y = restart_y + 80.0f;
  TextElement quit { quit_x, quit_y, Paths::pFontRobotoBold, Color::Blue, 45,
    renderer_->GetSDLrenderer(), quit_str };

  // add text elements to the container
  texts.emplace_back(&won);
  texts.emplace_back(&congrats);
  texts.emplace_back(&score);
  texts.emplace_back(&score_txt);
  texts.emplace_back(&restart);
  texts.emplace_back(&quit);

  // Display all text on screen
  renderer_->DisplayScreen(texts);
}

// gets a single texture from the stored textures
const Texture& Game::GetTexture(Sprite sprite) const
{
  // try to find a stored texture of the given spirte type
  auto search = images_.find(sprite);

  // If texture wasn't found, THROW exception
  // (by this point the project design assumes that all requried textures
  // should be loaded created during game initialization)
  if (search == images_.end()) {
    throw std::runtime_error(
        "Unable to get texture for sprite in function Game::GetTexture()");
  }
  // return found texture
  return search->second;
}

// creates the wall limiting the game area
void Game::CreateWalls()
{
  // design predicts 3 walls so reserve enough space
  side_walls_.reserve(3);

  CreateTopWall();
  CreateLeftWall();
  CreateRightWall();

  // add walls to the displayed objects
  for (auto& wall : side_walls_) {
    static_for_game_screen_.emplace_back(&wall);
  };
}

// creates the top wall
void Game::CreateTopWall()
{
  // get texture for top wall
  const Texture& texture { GetTexture(Sprite::WallHorizontal) };
  // calculate positon of the top wall
  float top_x { screen_width_ / 2.0f };
  float top_y { texture.Height() / 2.0f };
  // create top Wall
  side_walls_.emplace_back(
      top_x, top_y, ScreenSide::Top, texture, texture.Height() / 2.0f);
}

// creates the left wall
void Game::CreateLeftWall()
{
  //  get texture for the left wall
  const Texture& texture { GetTexture(Sprite::WallVertical) };
  // calculate positon of the left wall
  float left_x { texture.Width() / 2.0f };
  float left_y { screen_height_ / 2.0f };
  // create left Wall
  side_walls_.emplace_back(
      left_x, left_y, ScreenSide::Left, texture, texture.Width() / 2.0f);
}

// creates the right wall
void Game::CreateRightWall()
{ //  get texture for the right wall
  const Texture& texture { GetTexture(Sprite::WallVertical) };
  // calculate positon of the right wall
  float right_x { screen_width_ - texture.Width() / 2.0f };
  float right_y { screen_height_ / 2.0f };
  // create the right Wall
  side_walls_.emplace_back(
      right_x, right_y, ScreenSide::Right, texture, texture.Width() / 2.0f);
}

// creates the ball
void Game::CreateBall()
{
  ball_ = std::make_unique<Ball>(level_data_->BallSpeed(),
      GetTexture(Sprite::Ball), *paddle_, screen_height_, *this, side_walls_,
      blocks_);

  // verify if ball created successfully. If not throw exception
  if (!ball_) {
    throw std::runtime_error("Unable to create the ball");
  }

  // Add ball to the collection of movable objects to be displayed
  movable_for_game_screen_.emplace_back(ball_.get());
}

// creates the paddle
void Game::CreatePaddle()
{
  // get paddle texture and calculate vertical position
  auto& paddle_texture { GetTexture(Sprite::Paddle) };
  float paddle_y = screen_height_ - paddle_texture.Height() / 2.0f;
  // calculate and create rectangle limiting the paddle move range
  int wall_tickness { GetTexture(Sprite::WallVertical).Width() };
  SDL_Rect limits;
  // set top-left coordinates of the limiting rectangle
  limits.x = wall_tickness;
  limits.y = screen_height_ * 4 / 5;
  // set width of the limiting rectangle
  limits.w = screen_width_ - 2 * wall_tickness;
  // set height of the limiting rectangle
  limits.h = screen_height_ - limits.y;

  // create the actuall paddle and set unique pointer
  paddle_ = std::make_unique<Paddle>(screen_width_ / 2.0f, paddle_y,
      level_data_->PaddleSpeed(), limits, paddle_texture);
  // verify if paddle created successfully. If not throw exception
  if (!paddle_) {
    throw std::runtime_error("Unable to create the paddle");
  }

  // add paddle to the collection of objects to be displayed
  movable_for_game_screen_.emplace_back(paddle_.get());
}

// creates blocks
void Game::CreateBlocks()
{
  // reserve space in the container for blocks
  blocks_.reserve(LevelData::max_rows * LevelData::row_size);

  // aquire the sprite table representing block layout
  auto& sprite_table = level_data_->SpriteTable();
  // position offset of each block
  const float wall_offset { static_cast<float>(
      GetTexture(Sprite::WallVertical).Width()) };
  // horizontal position offset of each concecutive block from the other
  const float block_width { LevelData::block_width };
  // vertical position offset of each consecutive block from the other
  const float block_height { LevelData::block_height };
  // point value assigned to block on current game level
  const unsigned point_value { level_data_->PointsPerBlock() };
  // temporary helper variables to be calculated on most loop passes
  float block_x {};
  float block_y {};
  // parse each row of the spirte_table
  for (std::size_t row = 0; row < sprite_table.size(); row++) {
    // parse each column of the row
    for (std::size_t col = 0; col < sprite_table[row].size(); col++) {
      // construct only those blocks, for which there is a sprite set in the
      // sprite table
      if (sprite_table[row][col] != Sprite::None) {
        block_x = (block_width * col) + wall_offset + (block_width / 2.0f) - 1;
        block_y = (block_height * row) + wall_offset + (block_height / 2.0f);
        auto& texture { GetTexture(sprite_table[row][col]) };
        blocks_.emplace_back(block_x, block_y, texture, point_value);
      }
    }
  }
  // add blocks to the collection of static objects to be displayed
  for (auto& block : blocks_) {
    static_for_game_screen_.emplace_back(&block);
  }
}

// handles the ball leaving the allowed screen area
void Game::HandleBallEscape()
{
  PlaySound(Sound::BallLost);

  // decrease number of balls available
  balls_remaining_--;

  // check if the player run out of lives/balls
  if (balls_remaining_ <= 0) {
    // the game is over
    state_ = GameState::Over;
  }
  // player still has lives/balls left
  else {
    DisplayBallLostScreen();

    // halt execution for a couple of seconds
    SDL_Delay(4000);

    // reset the ball passing the level starting speed
    // to get the ball back on the paddle and cancelling
    // all speed increases applied by the player
    ball_->Reset(level_data_->BallSpeed());
  }
}

// handles a block being hit by the ball
void Game::HandleBlockHit(Block& block)
{
  PlaySound(Sound::BlockHit);

  // mark block as destroyed to skip further rendering and collision checks
  block.MarkDestroyed();

  // increase points score with point value assigned to the block
  total_points_ += block.Points();
}