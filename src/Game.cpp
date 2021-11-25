#include "Game.hpp"
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
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    throw SDLexception(
        "Failed to initialize SDL", SDL_GetError(), __FILE__, __LINE__);
  }
  // create graphics renderer here, only after SDL is initialized
  _renderer = new Renderer(_screen_height, _screen_width);

  // initialize SDL_Image support
  int to_init_flags = IMG_INIT_PNG;
  int initialized_flags = IMG_Init(to_init_flags);
  // check if SDL_Image support was initialized correctly
  if ((to_init_flags & initialized_flags) != to_init_flags) {
    throw SDLexception("Failed to initialize SDL IMAGE support", IMG_GetError(),
        __FILE__, __LINE__);
  }

  // load textures used in the game
  LoadTextures();
  // create the ball; it has to be done here, after SDL stuff is initilized and
  // textures are created
  _ball = std::make_unique<Ball>(
      _screen_width / 2, _screen_height / 2, GetTexture("ball"));
  // set the _renderer pointer to the ball for rendering operations
  _renderer->SetBall(_ball.get());
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
    _controller.HandleInput(running);

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
void Game::UpdateGame() { }

// generates all game output
void Game::GenerateOutput() const
{ // udpate display
  _renderer->Display();

  // TODO: update sounds
}

// / load all textures used in the game //NOTE: verify
void Game::LoadTextures()
{
  // load textue representing the ball
  _textures["ball"] = std::make_unique<Texture>(
      "../assets/ball.png", _renderer->GetSDLrenderer());
}

// gets a single texture from the stored textures
const Texture& Game::GetTexture(const std::string& TextureName) const
{
  // try to find a stored texture of the given name
  auto search = _textures.find(TextureName);
  // if texture wasn't found, throw exception
  // by this point the project design assumes that all requried textures were
  // created during game initialization
  if (search == _textures.end()) {
    std::string error { "Unable to get texture: \"" };
    error += TextureName + "\"";
    throw std::logic_error(error);
  }
  // return found texture
  return *(search->second);
}