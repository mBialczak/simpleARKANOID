#include "Game.hpp"
#include "Colors.hpp" // REMOVE INU
#include "IntervalTimer.hpp" //REMOVE most likely as is in header
#include "LimitTimer.hpp"
#include "Paths.hpp" // REVIEW: remove INU
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h" // REVIEW: remove INU
#include "SDLexception.hpp"
#include <algorithm>
#include <exception>
#include <fstream>
#include <iostream> // NOTE: remove AT
#include <sstream> // VERIFY if needed

// for operator""s usage // VERIFY if used in this file
using std::string_literals::operator""s;

// constructor
Game::Game(const std::size_t screenHeight, const std::size_t screenWidth,
    const std::size_t targetFrameRate)
    : _screen_height(screenHeight)
    , _screen_width(screenWidth)
    , _frame_rate(targetFrameRate)
    , _controller(std::make_unique<Controller>())
    , _renderer(nullptr)
    // , _controller(Controller()) // REMOVE?
    // load all the data for the first level
    , _level_data(std::make_unique<LevelData>(Paths::pLevels))
    , _lives_remaining(_level_data->Lives())
{
  // Initialize SDL subsystems
  InitSubsystems();

  // create graphics renderer here, only after SDL is initialized
  _renderer = new Renderer(_screen_height, _screen_width);

  // load textures used in the game
  LoadTextures();

  // REVIEW:
  // reserve space for all pointers to objects to be displayed on the game
  // screen (all the blocks + walls)
  _static_for_game_screen.reserve(
      LevelData::_max_rows * LevelData::_row_size + 3);

  // Only after SDL stuff is initialized and textures are created, other
  // components can be created
  CreateWalls();

  // REMOVE LevelData test
  // {
  //   std::cout << "Level Data test\n-----------------\n"
  //             << "Level number: " << _level_data->Level() << std::endl
  //             << "Ball speed: " << _level_data->BallSpeed() << std::endl
  //             << "Paddle speed: " << _level_data->PaddleSpeed() << std::endl
  //             << "Points per block: " << _level_data->PointsPerBlock()
  //             << std::endl
  //             << "Level lives: " << _level_data->Lives() << std::endl
  //             << std::endl;

  //   std::cout << "Sprites layout\n-------------\n";

  //   auto layout = _level_data->SpriteTable();

  //   for (std::size_t row = 0; row < layout.size(); row++) {
  //     std::cout << "Row " << std::to_string(row + 1) << ":\t";

  //     for (std::size_t col = 0; col < layout[row].size(); col++) {
  //       char sprite;
  //       switch (layout[row][col]) {
  //         case Sprite::None:
  //           sprite = '0';
  //           break;
  //         case Sprite::BlockGreen:
  //           sprite = 'g';
  //           break;
  //         default:
  //           sprite = '?';
  //       }
  //       std::cout << sprite << "  ";
  //     }
  //     std::cout << std::endl;
  //   }
  // }

  CreateBlocks();
  CreatePaddle();
  CreateBall();
  // REVIEW:
  // create all displayable texts which will not change in the game
  CreateTexts();
}
// initialize SDL subsystems
void Game::InitSubsystems()
{
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    throw SDLexception(
        "Failed to initialize SDL", SDL_GetError(), __FILE__, __LINE__);
  }

  // initialize SDL_Image support
  int to_init_flags = IMG_INIT_PNG;
  int initialized_flags = IMG_Init(to_init_flags);
  // check if SDL_Image support was initialized correctly
  if ((to_init_flags & initialized_flags) != to_init_flags) {
    throw SDLexception("Failed to initialize SDL IMAGE support", IMG_GetError(),
        __FILE__, __LINE__);
  }

  // initialize True Type Font support and check if it was done successfully
  if (TTF_Init() == -1) {
    throw SDLexception(
        "Failed to initialize TTF support", TTF_GetError(), __FILE__, __LINE__);
  }
}

// destructor
Game::~Game()
{
  // delete graphics renderer
  delete (_renderer);
  _renderer = nullptr;

  // REVIEW:
  // close SDL subsystems
  TTF_Quit();
  IMG_Quit();
  SDL_Quit();
}

// implements main game loop
void Game::Run()
{
  // calculate desired duration of a single frame
  const Uint32 desired_frame_duration { static_cast<Uint32>(
      IntervalTimer::_ticks_per_second / _frame_rate) };

  // create timer used for for FPS limiting
  LimitTimer frame_timer { desired_frame_duration };

  // REVIEW:
  // Pause the game to display welcom screen/instructions
  TogglePause();

  // main loop quit condition
  _is_running = true;
  // main game loop
  while (_is_running) {
    // handle input
    _controller->HandleInput(_is_running, *_paddle, *_ball, *this);
    // Update game state only if it isn't paused
    if (!_paused) {
      UpdateGame();
    }

    // REMOVE INU COMMENT
    switch (expression) {
      case /* constant-expression */:
        /* code */
        break;

      default:
        break;
    }

    GenerateOutput();

    // execute frame FPS limiting policy by waiting untill
    // each frame time completes
    frame_timer.waitTillExpire();
    // restart frameTimer for the next frame
    frame_timer.restart();
  }
}

// pauses or unpauses the game (pause on/off)
void Game::TogglePause()
{
  _paused = !_paused;
  // need to pause the timer if the game is paused
  if (_paused == true) {
    _timer.Pause();
  }
}

// updates the state of the game
void Game::UpdateGame()
{
  // calculate delta time and udpate timer
  auto delta_time = _timer.UpdateAndGetInterval();

  // REVIEW: for debugging purposes
  // if (delta_time >= 0.5f) {
  //   // cap delta_time while debugging if time difference is to big
  //   delta_time = 0.5f;
  // }
  // REVIEW: consider one container of moveable objects instead seperate objects

  // upate paddle state
  _paddle->Update(delta_time);
  // update ball state
  _ball->Update(delta_time);
}

// REVIEW: general implementation
// generates all game output
void Game::GenerateOutput() const
{
  // if not paused perform routime game output
  if (!_paused) {
    // display all static and movable objects
    _renderer->DisplayGameScreen(
        _static_for_game_screen, _movable_for_game_screen);
    // TODO: update sounds somewhere
  }
  else {
    DisplayPauseScreen();
  }
}

// / load all textures used in the game //NOTE: verify
void Game::LoadTextures()
{
  // load texture representing the ball
  _images[Sprite::Ball] = std::make_unique<Texture>(
      Paths::pBallImage, _renderer->GetSDLrenderer());
  // load texture respresenting the paddle
  _images[Sprite::Paddle] = std::make_unique<Texture>(
      Paths::pPadleImage, _renderer->GetSDLrenderer());
  // load texture representing the side_wall
  _images[Sprite::WallHorizontal] = std::make_unique<Texture>(
      Paths::pHorizontalWallImage, _renderer->GetSDLrenderer());
  _images[Sprite::WallVertical] = std::make_unique<Texture>(
      Paths::pVerticalWallImage, _renderer->GetSDLrenderer());
  _images[Sprite::BlockGreen] = std::make_unique<Texture>(
      Paths::pBlockGreenImage, _renderer->GetSDLrenderer());
}

// creates all the texts which won't change for the entire game
void Game::CreateTexts()
{
  // helper const for marking centre x position
  const float horizontal_centre = _screen_width / 2.0f;

  // Create text for the top
  std::string top { "Thank's for trying out my game:" };
  const float top_x = horizontal_centre;
  const float top_y = 30.0f;
  _texts.emplace_back(top_x, top_y, Paths::pFontRobotoRegular, Color::Yellow,
      30, _renderer->GetSDLrenderer(), top);

  // Create text for game title
  std::string title { " s i m p l e   A r k a n o i d  ! ! !" };
  const float title_x = horizontal_centre;
  const float title_y = top_y + 70.0f;
  _texts.emplace_back(title_x, title_y, Paths::pFontRobotoBoldItalic,
      Color::Green, 70, _renderer->GetSDLrenderer(), title);

  // Create text about the paused state
  std::string paused { "Game PAUSED" };
  const float paused_x = horizontal_centre;
  const float paused_y = title_y + 70.0f;
  _texts.emplace_back(paused_x, paused_y, Paths::pFontRobotoBold, Color::Red,
      36, _renderer->GetSDLrenderer(), paused);

  // Create instruction about unpausing
  std::string unpause { "( press 'ESCAPE' key to unpause )" };
  const float unpause_x = horizontal_centre;
  const float unpause_y = paused_y + 40.0f;
  _texts.emplace_back(unpause_x, unpause_y, Paths::pFontRobotoRegular,
      Color::Red, 22, _renderer->GetSDLrenderer(), unpause);

  // spacer for the area where dynamically created text whill apear
  const float spacer = 240.0f;

  // Create "Game Instructions" text
  std::string instructions {
    "G   A   M   E       C   O   N   T   R   O   L   S"
  };
  const float instr_x = horizontal_centre;
  const float instr_y = unpause_y + spacer;
  _texts.emplace_back(instr_x, instr_y, Paths::pFontRobotoBold, Color::Violet,
      32, _renderer->GetSDLrenderer(), instructions);

  // spacer between instructions
  const float instr_spacer = 35.0f;
  // 1st line of instructions
  std::string line_1 { "S P A C E   -   start the ball from the paddle" };
  const float line_1_x = horizontal_centre;
  const float line_1_y = instr_y + instr_spacer + 10.0f;
  _texts.emplace_back(line_1_x, line_1_y, Paths::pFontRobotoRegular,
      Color::Yellow, 22, _renderer->GetSDLrenderer(), line_1);

  // 2nd line of instructions
  std::string line_2 { "E S C A P E  -   pause / unpause the game" };
  const float line_2_x = horizontal_centre;
  const float line_2_y = line_1_y + instr_spacer;
  _texts.emplace_back(line_2_x, line_2_y, Paths::pFontRobotoRegular,
      Color::Orange, 22, _renderer->GetSDLrenderer(), line_2);

  // 3rd line of instructions
  std::string line_3 {
    "L E F T   and   R I G H T  arrows  -  move the paddle HORIZONTALLY"
  };
  const float line_3_x = horizontal_centre;
  const float line_3_y = line_2_y + instr_spacer;
  _texts.emplace_back(line_3_x, line_3_y, Paths::pFontRobotoRegular,
      Color::Yellow, 22, _renderer->GetSDLrenderer(), line_3);

  // 4th line of instructions
  std::string line_4 { "U P   and   D O W N   arrows  -  move the paddle "
                       "VERTICALLY (within allowed limits)" };
  const float line_4_x = horizontal_centre;
  const float line_4_y = line_3_y + instr_spacer;
  _texts.emplace_back(line_4_x, line_4_y, Paths::pFontRobotoRegular,
      Color::Orange, 22, _renderer->GetSDLrenderer(), line_4);

  // 5th line of instructions
  std::string line_5 { "CONTROLS  WORKING  ONLY  WHILE  "
                       "THE  BALL  HITS  THE  PADDLE :" };
  const float line_5_x = horizontal_centre;
  const float line_5_y = line_4_y + instr_spacer + 10.0f;
  _texts.emplace_back(line_5_x, line_5_y, Paths::pFontRobotoRegular,
      Color::Violet, 26, _renderer->GetSDLrenderer(), line_5);

  // 6th line of instructions
  std::string line_6 {
    "A -  spin the ball to the left          F -  spin the ball to the right"
  };
  const float line_6_x = horizontal_centre;
  const float line_6_y = line_5_y + instr_spacer;
  _texts.emplace_back(line_6_x, line_6_y, Paths::pFontRobotoRegular,
      Color::Orange, 22, _renderer->GetSDLrenderer(), line_6);

  // 7th line of instructions
  std::string line_7 { "E - speed up the ball (steps with no limit)     D - "
                       "slow down the ball (no more than  the level minimum)" };
  const float line_7_x = horizontal_centre;
  const float line_7_y = line_6_y + instr_spacer;
  _texts.emplace_back(line_7_x, line_7_y, Paths::pFontRobotoRegular,
      Color::Yellow, 22, _renderer->GetSDLrenderer(), line_7);
}

// REVIEW:and COMMENT
// Generates container of static objects to be displayed on the pause screen
void Game::DisplayPauseScreen() const
{
  // create container of static objects to be displayed
  std::vector<const StaticObject*> all_texts;
  // reserve container space for all predicted elements
  all_texts.reserve(_texts.size() + 6);

  // copy addresses of all stored texts which won't change during game
  for (auto& text : _texts) {
    all_texts.emplace_back(&text);
  }

  // create remaining balls counter display
  std::string balls_str { std::to_string(_lives_remaining) };
  const float balls_x = _screen_width / 2.0f;
  const float balls_y = _screen_height / 3.0f + 45.0f;
  TextElement balls { balls_x, balls_y, Paths::pFontRobotoBold, Color::Yellow,
    70, _renderer->GetSDLrenderer(), balls_str };

  // create "remaining balls" text
  std::string remaining_str { "Balls left" };
  const float remaining_x = _screen_width / 2.0f;
  const float remaining_y = balls_y + 70;
  TextElement remaining { remaining_x, remaining_y, Paths::pFontRobotoBold,
    Color::Yellow, 36, _renderer->GetSDLrenderer(), remaining_str };

  // create level counter display
  std::string level_str { std::to_string(_level_data->Level()) };
  const float level_x
      = (remaining.Position()._x - (remaining.Width() / 2.0f)) / 2.0f;
  const float level_y = balls_y;
  TextElement level { level_x, level_y, Paths::pFontRobotoBold, Color::Blue, 70,
    _renderer->GetSDLrenderer(), level_str };

  // create "level" text
  std::string lvl_txt_str { "Level" };
  const float lvl_txt_x = level_x;
  const float lvl_txt_y = remaining_y;
  TextElement lvl_txt { lvl_txt_x, lvl_txt_y, Paths::pFontRobotoBold,
    Color::Blue, 36, _renderer->GetSDLrenderer(), lvl_txt_str };

  // create total score counter display
  std::string score_str { std::to_string(_total_points) };
  const float score_x = _screen_width - level_x;
  const float score_y = balls_y;
  TextElement score { score_x, score_y, Paths::pFontRobotoBold, Color::Green,
    70, _renderer->GetSDLrenderer(), score_str };

  // create "score" text
  std::string score_txt_str { "SCORE" };
  const float score_txt_x = score_x;
  const float score_txt_y = lvl_txt_y;
  TextElement score_txt { score_txt_x, score_txt_y, Paths::pFontRobotoBold,
    Color::Green, 36, _renderer->GetSDLrenderer(), score_txt_str };

  // add text elements to the container
  all_texts.emplace_back(&remaining);
  all_texts.emplace_back(&balls);
  all_texts.emplace_back(&level);
  all_texts.emplace_back(&lvl_txt);
  all_texts.emplace_back(&score);
  all_texts.emplace_back(&score_txt);

  // Display all text on screen
  _renderer->DisplayStaticScreen(all_texts);
}

// Displays the screen after ball leaving the screen
void Game::DisplayBallLostScreen() const
{
  // create container of static objects to be displayed
  std::vector<const StaticObject*> texts;
  // TODO: reserve container space for all predicted elements
  texts.reserve(_texts.size() + 5);

  // create "ball lost" text
  std::string ball_out_str { " B A L L   L E F T " };
  const float ball_out_x = _screen_width / 2.0f;
  const float ball_out_y = _screen_height / 5.0f;
  TextElement ball_out { ball_out_x, ball_out_y, Paths::pFontRobotoBold,
    Color::Red, 90, _renderer->GetSDLrenderer(), ball_out_str };

  // create "ball lost" text - 2nd part
  std::string ball_out2_str { "T H E   S C R E E N" };
  const float ball_out2_x = _screen_width / 2.0f;
  const float ball_out2_y = ball_out_y + 100.0f;
  TextElement ball_2out { ball_out2_x, ball_out2_y, Paths::pFontRobotoBold,
    Color::Red, 90, _renderer->GetSDLrenderer(), ball_out2_str };

  // create remaining balls counter display
  std::string balls_str { std::to_string(_lives_remaining) };
  const float balls_x = _screen_width / 2.0f;
  const float balls_y = _screen_height / 2.0f + 20.f;
  TextElement balls { balls_x, balls_y, Paths::pFontRobotoBold, Color::Orange,
    120, _renderer->GetSDLrenderer(), balls_str };

  // create "remaining balls" text
  std::string remaining_str { "Ball(s) remaining" };
  const float remaining_x = _screen_width / 2.0f;
  const float remaining_y = balls_y + 120;
  TextElement remaining { remaining_x, remaining_y, Paths::pFontRobotoBold,
    Color::Orange, 36, _renderer->GetSDLrenderer(), remaining_str };

  // create "resuming" text
  std::string resume_str { "Game will resume in a couple of seconds . . ." };
  const float resume_x = _screen_width / 2.0f;
  const float resume_y = remaining_y + 100.0f;
  TextElement resume { resume_x, resume_y, Paths::pFontRobotoBold, Color::Green,
    45, _renderer->GetSDLrenderer(), resume_str };

  // add text elements to the container
  texts.emplace_back(&remaining);
  texts.emplace_back(&balls);
  texts.emplace_back(&ball_out);
  texts.emplace_back(&resume);
  texts.emplace_back(&ball_2out);

  // Display all text on screen
  _renderer->DisplayStaticScreen(texts);
}

// Displays the screen after all lives/balls have been lost
void Game::DisplayGameOverScreen() const
{
  // create container of static objects to be displayed
  std::vector<const StaticObject*> texts;
  // TODO: reserve container space for all predicted elements
  // texts.reserve(_texts.size() + 6); // REVIEW:

  // create "game over" text
  std::string g_over_str { "G A M E    O V E R" };
  const float g_over_x = _screen_width / 2.0f;
  const float g_over_y = _screen_height / 5.0f;
  TextElement g_over { g_over_x, g_over_y, Paths::pFontRobotoBold, Color::Red,
    90, _renderer->GetSDLrenderer(), g_over_str };

  // create "all ball lost" text
  std::string all_lost_str { "( all balls have been lost )" };
  const float all_lost_x = _screen_width / 2.0f;
  const float all_lost_y = g_over_y + 60.0f;
  TextElement all_lost { all_lost_x, all_lost_y, Paths::pFontRobotoBold,
    Color::Red, 90, _renderer->GetSDLrenderer(), all_lost_str };

  // create total score counter display
  std::string score_str { std::to_string(_lives_remaining) };
  const float score_x = _screen_width / 2.0f;
  const float score_y = _screen_height / 2.0f + 20.f;
  TextElement score { score_x, score_y, Paths::pFontRobotoBold, Color::Orange,
    120, _renderer->GetSDLrenderer(), score_str };

  // create "total score" text
  std::string score_txt_str { "T O T A L   S C O R E" };
  const float score_txt_x = _screen_width / 2.0f;
  const float score_txt_y = score_y + 120;
  TextElement score_txt { score_txt_x, score_txt_y, Paths::pFontRobotoBold,
    Color::Orange, 36, _renderer->GetSDLrenderer(), score_txt_str };

  // // create "resuming" text
  // std::string resume_str { "Game will resume in a couple of seconds . . ." };
  // const float resume_x = _screen_width / 2.0f;
  // const float resume_y = remaining_y + 100.0f;
  // TextElement resume { resume_x, resume_y, Paths::pFontRobotoBold,
  // Color::Green,
  //   45, _renderer->GetSDLrenderer(), resume_str };

  // add text elements to the container
  texts.emplace_back(&g_over);
  texts.emplace_back(&all_lost);
  texts.emplace_back(&score);
  texts.emplace_back(&score_txt);
  // texts.emplace_back(&ball_2out);

  // Display all text on screen
  _renderer->DisplayStaticScreen(texts);

  SDL_Event evt;
  // get all SDL events
  while (SDL_PollEvent(&evt)) {
    // filetre out push down key events
    if (evt.type == SDL_KEYDOWN) {
      // and the the key was the esc key
      if (evt.key.keysym.sym == SDLK_ESCAPE) {
        std::cout << "ESC!!!!" << std::endl;
      }
      else if (evt.key.keysym.sym == SDLK_SPACE) {
        std::cout << "SPACE!!!!" << std::endl;
      }
    }
  }
}

// gets a single texture from the stored textures
const Texture& Game::GetTexture(Sprite sprite) const
{
  // try to find a stored texture of the given spirte type
  auto search = _images.find(sprite);
  // if texture wasn't found, throw exception
  // VERIFY if needed such a long comment
  // by this point the project design assumes that all requried textures
  // should be loaded created during game initialization
  if (search == _images.end()) {
    throw std::runtime_error(
        "Unable to get texture for sprite in function Game::GetTexture()");
  }
  // return found texture
  return *(search->second);
}

// creates the wall limiting the game area
void Game::CreateWalls()
{
  // REVIEW:
  // design predicts 3 walls so reserve enough space
  _side_walls.reserve(3);

  CreateTopWall();
  CreateLeftWall();
  CreateRightWall();

  // add walls to the displayed objects
  for (auto& wall : _side_walls) {
    _static_for_game_screen.emplace_back(&wall);
  };
}

// creates the top wall
void Game::CreateTopWall()
{
  // get texture for top wall
  const Texture& texture { GetTexture(Sprite::WallHorizontal) };
  // calculate positon of the top wall
  float top_x { _screen_width / 2.0f };
  float top_y { texture.Height() / 2.0f };
  // create top Wall
  _side_walls.emplace_back(
      top_x, top_y, ScreenSide::Top, texture, texture.Height() / 2.0f);
}

// creates the left wall
void Game::CreateLeftWall()
{
  //  get texture for the left wall
  const Texture& texture { GetTexture(Sprite::WallVertical) };
  // calculate positon of the left wall
  float left_x { texture.Width() / 2.0f };
  float left_y { _screen_height / 2.0f };
  // create left Wall
  _side_walls.emplace_back(
      left_x, left_y, ScreenSide::Left, texture, texture.Width() / 2.0f);
}

// creates the right wall
void Game::CreateRightWall()
{ //  get texture for the right wall
  const Texture& texture { GetTexture(Sprite::WallVertical) };
  // calculate positon of the right wall
  float right_x { _screen_width - texture.Width() / 2.0f };
  float right_y { _screen_height / 2.0f };
  // create the right Wall
  _side_walls.emplace_back(
      right_x, right_y, ScreenSide::Right, texture, texture.Width() / 2.0f);
}

// creates the ball
void Game::CreateBall()
{
  _ball = std::make_unique<Ball>(_level_data->BallSpeed(),
      GetTexture(Sprite::Ball), *_paddle, _screen_height, *this, _side_walls,
      _blocks);

  // verify if ball created successfully. If not throw exception
  if (!_ball) {
    throw std::runtime_error("Unable to create the ball");
  }

  // Add ball to the collection of movable objects to be displayed
  _movable_for_game_screen.emplace_back(_ball.get());
}

// creates the paddle
void Game::CreatePaddle()
{
  // get paddle texture and calculate vertical position
  auto& paddle_texture { GetTexture(Sprite::Paddle) };
  float paddle_y = _screen_height - paddle_texture.Height() / 2.0f;
  // calculate and create rectangle limiting the paddle move range
  int wall_tickness { GetTexture(Sprite::WallVertical).Width() / 2 };
  SDL_Rect limits;
  // set top-left coordinates of the limiting rectangle
  limits.x = wall_tickness;
  limits.y = _screen_height * 4 / 5;
  // set width of the limiting rectangle
  limits.w = _screen_width - 2 * wall_tickness;
  // set height of the limiting rectangle
  limits.h = _screen_height - limits.y;

  // create the actuall paddle and set unique pointer
  _paddle = std::make_unique<Paddle>(_screen_width / 2, paddle_y,
      _level_data->PaddleSpeed(), limits, paddle_texture);
  // verify if paddle created successfully. If not throw exception
  if (!_paddle) {
    throw std::runtime_error("Unable to create the paddle");
  }

  // add paddle to the collection of objects to be displayed
  _movable_for_game_screen.emplace_back(_paddle.get());
}
// TODO: refactor and COMMENTS
// creates blocks
void Game::CreateBlocks()
{
  // reserve place for blocks
  _blocks.reserve(LevelData::MaxRows() * LevelData::RowSize());

  // aquire the sprite table representing block layout
  auto& sprite_table = _level_data->SpriteTable();
  // position offset of each block
  const float wall_offset { static_cast<float>(
      GetTexture(Sprite::WallVertical).Width()) };
  // horizontal position offset of each concecutive block from the other
  const float block_width { LevelData::_block_width };
  // vertical position offset of each consecutive block from the other
  const float block_height { LevelData::_block_height };
  // point value assigned to block on current game level
  const unsigned point_value { _level_data->PointsPerBlock() };
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
        block_x = (block_width * col) + wall_offset + (block_width / 2.0f);
        block_y = (block_height * row) + wall_offset + (block_height / 2.0f);
        auto& texture { GetTexture(sprite_table[row][col]) };
        _blocks.emplace_back(block_x, block_y, texture, point_value);
      }
    }
  }

  // add blocks to the collection of static objects to be displayed
  for (auto& block : _blocks) {
    _static_for_game_screen.emplace_back(&block);
  }
}

// TODO: implement if needed
void Game::HandleBallEscape()
{
  // decrease number of balls available
  _lives_remaining--;
  // check if game is over
  if (_lives_remaining <= 0) {
    // TODO: handle game over
    // - display game over screen
    // - play some sound
    // + save high score
    // - check if player wants to start again
    //    -> yes - reset game state
    //    -> no - display goodbye! and close the game

    // TODO: sound

    DisplayGameOverScreen();
  }
  // handle the case where player still have lives/balls left
  else {
    DisplayBallLostScreen();
    // halt execution a couple of seconds
    LimitTimer timer(4000);
    timer.waitTillExpire();

    // reset the ball passing the level starting speed
    _ball->Reset(_level_data->BallSpeed());
  }
}

// handles a block being hit by the ball
// TODO: to implement
void Game::HandleBlockHit(Block& block)
{
  // set block as destroyed to skip further rendering and collision checks
  block.MakeDestroyed();
  // increase points score with point value assigned to the block
  _total_points += block.Points();
  // REVIEW: remove AT
  std::cout << "A block has been hit!!!\n" << std::endl;
  std::cout << "Total points = " << _total_points
            << "\n----------------------\n"
            << std::endl;
}