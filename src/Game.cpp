#include "Game.hpp"
#include "IntervalTimer.hpp"
#include "LimitTimer.hpp"
#include "Paths.hpp" // REVIEW: remove INU
#include "SDL.h"
#include "SDL_image.h"
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
}

// destructor
Game::~Game()
{
  // delete graphics renderer
  delete (_renderer);
  _renderer = nullptr;

  // close SDL subsystems
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
{ // udpate display
  _renderer->Display();

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

// VERIFY
// Reads the level data from the file. Takes the number representing the
// level and returns the table representing blocks composition on the screan
std::vector<std::vector<std::string>> Game::LoadLevelData(unsigned level)
{
  // the path to the level data
  std::string path { Paths::pLevels + "Level" + std::to_string(_level)
    + ".txt" };
  // the table representing blocks composition to be returned
  std::vector<std::vector<std::string>> level_data;
  level_data.reserve(_max_rows);
  // open the file
  std::ifstream file_stream(path);
  // check if opening file for reading was successful
  if (!file_stream) {
    throw std::runtime_error(
        "Unable to open stream to the level data!"s + path);
  }
  // single line of file data to be read
  std::string file_line;
  // read and discard the first line which has no meaning for level composition
  // and serves only as helper for level editing
  std::getline(file_stream, file_line);
  // string stream for stripping each line into cells
  std::istringstream line_stream;

  unsigned row_count { 0 };
  // read the file line by line until end of file or maximum number of
  // rows predicted for displaying is rached
  while (std::getline(file_stream, file_line) && row_count < _max_rows) {
    // load the the  line data into string stream
    line_stream.str(file_line);
    // cell representing single block data
    std::string cell;
    // single line of cell data to be composed into returned table
    std::vector<std::string> cell_line;
    cell_line.reserve(_row_size);
    // strip the line data into cells
    while (line_stream >> cell) {
      cell_line.emplace_back(cell);
    }
    // check for correct number of blocks in the signle row
    if (cell_line.size() != 20) {
      throw std::runtime_error(
          "Wrong level-file format detected while reading level file: "s
          + path);
    }
    level_data.emplace_back(std::move(cell_line));
    row_count++;
  }
  // check if the created table is not empty and doesn't exceed max size
  if (level_data.size() == 0 || level_data.size() >= _max_rows) {
    throw std::runtime_error(
        "Error while reading level data from the file"s + path);
  }

  return level_data;
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
  _ball = std::make_unique<Ball>(_ball_speed, GetTexture(Sprite::Ball),
      *_paddle, _screen_height, *this, _side_walls, _blocks);

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
  int paddle_y = _screen_height - paddle_texture.Height() / 2;
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
  _paddle = std::make_unique<Paddle>(
      _screen_width / 2, paddle_y, _paddle_speed, limits, paddle_texture);
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
  // TODO: reserve place for blocks

  // TODO: revise block creation
  auto& block_texture = GetTexture(Sprite::BlockGreen);
  _blocks.emplace_back(_screen_width / 2.0 - 49 / 2.0f,
      _screen_height / 2.0f - 20 / 2.0, block_texture, 1);

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