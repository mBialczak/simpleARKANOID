#include "Game.hpp"
#include "IntervalTimer.hpp"
#include "LimitTimer.hpp"
#include "SDL.h"
#include "SDL_image.h"
#include "SDLexception.hpp"
#include <algorithm>
#include <exception>

// constructor
Game::Game(const std::size_t screenHeight, const std::size_t screenWidth,
    const std::size_t targetFrameRate)
    : _screen_height(screenHeight)
    , _screen_width(screenWidth)
    , _frame_rate(targetFrameRate)
    , _renderer(nullptr)
{
  // Initialize SDL subsystems
  InitSubsystems();

  // design predicts only 3 walls so reserve enough space
  _side_walls.reserve(3);

  // create graphics renderer here, only after SDL is initialized
  _renderer = new Renderer(_screen_height, _screen_width, _side_walls);

  // load textures used in the game
  LoadTextures();
  // Only after SDL stuff is initialized and textures are created, other
  // components can be created
  CreateWalls();
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
  bool running = true;
  // main game loop
  while (running) {

    // handle input
    _controller.HandleInput(running, *_paddle);

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
  _textures["ball"] = std::make_unique<Texture>(
      "../assets/ball.png", _renderer->GetSDLrenderer());
  // load texture respresenting the paddle
  _textures["paddle"] = std::make_unique<Texture>(
      "../assets/paddle.png", _renderer->GetSDLrenderer());
  // load texture representing the side_wall
  _textures["horizontal_wall"] = std::make_unique<Texture>(
      "../assets/horizontal_wall.png", _renderer->GetSDLrenderer());
  _textures["vertical_wall"] = std::make_unique<Texture>(
      "../assets/vertical_wall.png", _renderer->GetSDLrenderer());
}

// gets a single texture from the stored textures
const Texture& Game::GetTexture(const std::string& textureName) const
{
  // try to find a stored texture of the given name
  auto search = _textures.find(textureName);
  // if texture wasn't found, throw exception
  // by this point the project design assumes that all requried textures were
  // created during game initialization
  if (search == _textures.end()) {
    std::string error { "Unable to get texture: \"" };
    error += textureName + "\"";
    throw std::logic_error(error);
  }
  // return found texture
  return *(search->second);
}

// creates the wall limiting the game area
void Game::CreateWalls()
{
  CreateTopWall();
  CreateLeftWall();
  CreateRightWall();

  // NOTE: remove AR if works
  // let the rernderer know which walls to render
  // _renderer->SetWalls(&_walls);
}

// creates the top wall
void Game::CreateTopWall()
{
  // get texture for top wall
  const Texture& texture { GetTexture("horizontal_wall") };
  // calculate positon of the top wall
  float top_x { _screen_width / 2.0f };
  float top_y { texture.Height() / 2.0f };
  // create top Wall
  _side_walls.emplace_back(
      top_x, top_y, SideWall::CollisionBorder::Bottom, texture);
}

// creates the left wall
void Game::CreateLeftWall()
{
  //  get texture for the left wall
  const Texture& texture { GetTexture("vertical_wall") };
  // calculate positon of the left wall
  float left_x { texture.Width() / 2.0f };
  float left_y { _screen_height / 2.0f };
  // create left Wall
  _side_walls.emplace_back(
      left_x, left_y, SideWall::CollisionBorder::Right, texture);
}

// creates the right wall
void Game::CreateRightWall()
{ //  get texture for the right wall
  const Texture& texture { GetTexture("vertical_wall") };
  // calculate positon of the right wall
  float right_x { _screen_width - texture.Width() / 2.0f };
  float right_y { _screen_height / 2.0f };
  // create the right Wall
  _side_walls.emplace_back(
      right_x, right_y, SideWall::CollisionBorder::Left, texture);
}

// creates the ball
void Game::CreateBall()
{
  // NOTE: randomize starting ball data: angle and speed perhaps
  _ball = std::make_unique<Ball>(_screen_width / 2.0f, _screen_height / 2.0f,
      _randomizer(210.0f, 270.f), _ball_speed, GetTexture("ball"), *_paddle,
      _screen_height);
  // verify if ball created successfully. If not throw exception
  if (!_ball) {
    throw std::logic_error("Unable to create the ball");
  }
  // set the _renderer pointer to the ball for rendering operations
  _renderer->SetBall(_ball.get());
}

// creates the paddle
void Game::CreatePaddle()
{
  // get paddle texture and calculate vertical position
  auto& paddle_texture { GetTexture("paddle") };
  int paddle_y = _screen_height - paddle_texture.Height() / 2;
  // calculate and create rectangle limiting the paddle move range
  SDL_Rect limits;
  // set top-left coordinates of the limiting rectangle
  limits.x = _wall_tickness;
  limits.y = _screen_height * 3 / 4;
  // set width of the limiting rectangle
  limits.w = _screen_width - 2 * _wall_tickness;
  // set height of the limiting rectangle
  limits.h = _screen_height - limits.y;

  // create the actuall paddle and set unique pointer
  _paddle = std::make_unique<Paddle>(
      _screen_width / 2, paddle_y, _paddle_speed, limits, paddle_texture);
  // verify if paddle created successfully. If not throw exception
  if (!_paddle) {
    throw std::logic_error("Unable to create the paddle");
  }
  // set the _renderer pointer to the ball for rendering operations
  _renderer->SetPaddle(_paddle.get());
}