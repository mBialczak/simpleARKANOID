#include "Renderer.hpp"
#include "LimitTimer.hpp"
#include "SDLexception.hpp"
#include <iostream> // remove after testing
// constructor
Renderer::Renderer(const std::size_t screenHeight,
    const std::size_t screenWidth, const std::vector<SideWall>& sideWalls)
    : _screen_height(screenHeight)
    , _screen_width(screenWidth)
    , _side_walls(sideWalls)
    // the real ball pointer will be set after
    // the ball is created in Game constructor
    , _ball(nullptr)
    // the real paddle pointer will be set  after renderer creation, after
    // the paddle is created in Game constructor
    , _paddle(nullptr)
{
  // create main game window
  _sdl_window = SDL_CreateWindow("Simple Arkanoid game", SDL_WINDOWPOS_CENTERED,
      SDL_WINDOWPOS_CENTERED, _screen_width, _screen_height, SDL_WINDOW_SHOWN);

  // check if window created successfully
  if (!_sdl_window) {
    throw SDLexception(
        "Could not create window", SDL_GetError(), __FILE__, __LINE__);
  }

  // create graphics renderer
  _sdl_renderer = SDL_CreateRenderer(
      _sdl_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

  // check if renderer created successfully
  if (!_sdl_renderer) {
    throw SDLexception(
        "Could not create renderer", SDL_GetError(), __FILE__, __LINE__);
  }
}

// destructor
Renderer::~Renderer()
{
  // Deestroy graphics renderer
  SDL_DestroyRenderer(_sdl_renderer);
  _sdl_renderer = nullptr;
  // Destroy main game window
  SDL_DestroyWindow(_sdl_window);
  _sdl_window = nullptr;
}

// sets member ball pointer to be rendered
void Renderer::SetBall(const Ball* const ball)
{
  // if Ball is a nullptr the class invariant will not be ok so throw
  if (!ball) {
    // create error message
    std::string error { "The ball pointer cannot be set to nullptr" };
    // add source file name and line info to the message
    error += __FILE__;
    error += std::to_string(__LINE__);
    throw std::logic_error(error);
  }
  _ball = ball;
}

// sets member paddle pointer to be rendered
void Renderer::SetPaddle(const Paddle* const paddle)
{
  // if paddle is a nullptr the class invariant will not be ok so throw
  if (!paddle) {
    // create error message
    std::string error { "The paddle pointer cannot be set to nullptr" };
    // add source file name and line info to the message
    error += __FILE__;
    error += std::to_string(__LINE__);
    throw std::logic_error(error);
  }
  _paddle = paddle;
}

// adds StaticObject to be displayed //NOTE: remove INU
void Renderer::AddStaticObject(const StaticObject* object)
{
  if (object) {
    _static_objects.emplace_back(object);
  }
}
// adds MovableObject to be displayed //NOTE: remove INU
void Renderer::AddMovableObject(const MovableObject* object)
{
  if (object) {
    _movable_objects.emplace_back(object);
  }
}

// displays (renders) game graphics
void Renderer::Display() const
{
  UpdateTitleBar();

  // Set clear screen color
  SDL_SetRenderDrawColor(_sdl_renderer, 0x00, 0x00, 0x00, 0xFF);
  // clear screen
  SDL_RenderClear(_sdl_renderer);

  // REVIEW: remove if no longer used
  // // display the walls
  // for (auto& wall : _side_walls) {
  //   wall.Draw();
  // }

  // REVIEW: remove INU
  // display all static game objects
  for (auto& object : _static_objects) {
    object->Draw();
  }

  // REVIEW: remove if no longer used
  _ball->Draw();

  // display the paddle
  _paddle->Draw();

  // update screen
  SDL_RenderPresent(_sdl_renderer);
}

// update title bar
void Renderer::UpdateTitleBar() const
{
  // create timer for window title udpates
  // the title is to be updated every second (1000 ticks)
  const Uint32 c_title_update_time = 1000;
  static LimitTimer title_update_timer { c_title_update_time };
  // frame count for FPS updating
  static int frames { 0 };

  // update frame count for FPS calculation
  frames++;
  // if 1s has passed, update window title bar
  if (title_update_timer.hasExpired()) {
    // create and display title to be updated every second
    std::string title { "Simple Akranoid game! Have fun!\t\t\tFPS: " };
    title += std::to_string(frames);
    SDL_SetWindowTitle(_sdl_window, title.c_str());

    // reset number of frames for another run
    frames = 0;
    // restart timer for window title udpates
    title_update_timer.restart();
  }
}
