#include "Game.hpp"
#include "IntervalTimer.hpp"
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
    , _renderer(nullptr)
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

  // REVIEW:
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
  const Uint32 c_SDL_ticks_per_second = 1000;
  const Uint32 desired_frame_duration { c_SDL_ticks_per_second / _frame_rate };

  // create timer used for for FPS limiting
  LimitTimer frame_timer { desired_frame_duration };

  // main loop quit condition
  _is_running = true;
  // main game loop
  while (_is_running) {

    // handle input
    _controller.HandleInput(_is_running, *_paddle, *_ball);

    UpdateGame();
    GenerateOutput();

    // execute frame FPS limiting policy by waiting untill
    // each frame time completes
    frame_timer.waitTillExpire();
    // restart frameTimer for the next frame
    frame_timer.restart();
  }
}

// updates the state of the game
void Game::UpdateGame()
{
  // create timer measuring time difference for update calculations
  static IntervalTimer timer;
  // calculate delta time and udpate timer
  auto delta_time = timer.UpdateAndGetInterval();
  // REVIEW: for debugging purposes
  if (delta_time >= 0.5f) {
    // cap delta_time while debugging if time difference is to big
    delta_time = 0.5f;
  }
  // REVIEW: consider one container of moveable objets instead seperate objects

  // upate paddle state
  _paddle->Update(delta_time);
  // update ball state
  _ball->Update(delta_time);
}

// generates all game output
void Game::GenerateOutput() const
{ // update display
  // REVIEW: restore or modify for pausing
  // _renderer->Display();
  _renderer->Display("PAUSE Message :)");
  // TODO: update sounds
}

// / load all textures used in the game //NOTE: verify
void Game::LoadTextures()
{
  // load texture representing the ball
  _textures[Sprite::Ball] = std::make_unique<Texture>(
      Paths::pBallImage, _renderer->GetSDLrenderer());
  // load texture respresenting the paddle
  _textures[Sprite::Paddle] = std::make_unique<Texture>(
      Paths::pPadleImage, _renderer->GetSDLrenderer());
  // load texture representing the side_wall
  _textures[Sprite::WallHorizontal] = std::make_unique<Texture>(
      Paths::pHorizontalWallImage, _renderer->GetSDLrenderer());
  _textures[Sprite::WallVertical] = std::make_unique<Texture>(
      Paths::pVerticalWallImage, _renderer->GetSDLrenderer());
  _textures[Sprite::BlockGreen] = std::make_unique<Texture>(
      Paths::pBlockGreenImage, _renderer->GetSDLrenderer());
}

// gets a single texture from the stored textures
const Texture& Game::GetTexture(Sprite sprite) const
{
  // try to find a stored texture of the given spirte type
  auto search = _textures.find(sprite);
  // if texture wasn't found, throw exception
  // VERIFY if needed such a long comment
  // by this point the project design assumes that all requried textures should
  // be loaded created during game initialization
  if (search == _textures.end()) {
    throw std::runtime_error(
        "Unable to get texture for sprite in function Game::GetTexture()");
  }
  // return found texture
  return *(search->second);
}

// creates the wall limiting the game area
void Game::CreateWalls()
{
  // design predicts 3 walls so reserve enough space
  _side_walls.reserve(3);

  CreateTopWall();
  CreateLeftWall();
  CreateRightWall();

  // add walls to the renderers displayed objects
  for (auto& wall : _side_walls) {
    _renderer->AddStaticObject(&wall);
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

  // Add ball to the collection of objects displayed by the renderer
  _renderer->AddMovableObject(_ball.get());
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

  // add paddle to the collection of objects displayed by the renderer
  _renderer->AddMovableObject(_paddle.get());
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

  // add blocks to the collection of objects displayed by the renderer
  for (auto& block : _blocks) {
    _renderer->AddStaticObject(&block);
  }
}

// TODO: implement if needed
void Game::HandleBallEscape()
{
  std::cout << "HandleBallEscape()!" << std::endl;
  _is_running = false;
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