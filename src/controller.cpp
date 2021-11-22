#include "controller.hpp"
#include "SDL.h"

void Controller::HandleInput(bool& running) const
{
  SDL_Event evt;
  // get all SDL events
  while (SDL_PollEvent(&evt)) {
    // handle quit evt
    if (evt.type == SDL_QUIT) {
      running = false;
    }
  }
}