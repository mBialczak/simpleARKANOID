#include "SDLInitializers.hpp"
#include "SDLexception.hpp"

// --------- SDLinitializer ----------- //

// Constructor. Tries to initialize SDL requested subsystems.
// Takes SDL specific flags (most likely OR-ed)
// Throws SDLexception if initialization fails
SDLinitializer::SDLinitializer(Uint32 flags)
{
  // try to initialize SDL subsystems and report error if failed
  if (SDL_Init(flags < 0)) {
    throw SDLexception(
        "Failed to initialize SDL", SDL_GetError(), __FILE__, __LINE__);
  }
}

// Destructor. Closes SDL subsystems
SDLinitializer::~SDLinitializer()
{
  // close SDL subsystems
  SDL_Quit();
}

// --------- ImageInitializer ----------- //

// Constructor. Tries to initialize SDL Image support for image files specified
// by SDL Image flags (possibly OR-ed).
// Throws SDLexception if initialization fails
ImageInitializer::ImageInitializer(int flags)
{
  // set flags to initialize
  int to_init_flags = IMG_INIT_PNG;
  // try to initialize SDL Image support and retrieve flags which IMG_Init()
  // was able to initialize successfully
  int initialized_flags = IMG_Init(to_init_flags);
  // Check if  SDL_Image support was initialized correctly and report error if
  // initialization attempt was unsuccessful
  if ((to_init_flags & initialized_flags) != to_init_flags) {
    throw SDLexception("Failed to initialize SDL IMAGE support", IMG_GetError(),
        __FILE__, __LINE__);
  }
}

// Destructor. Unloads libraries loaded with IMG_Init
ImageInitializer::~ImageInitializer() { IMG_Quit(); }

// --------- TTFinitializer ----------- //

// Constructor. Tries to initialize True Type Font SDL engine.
// Throws SDLexception if initialization fails
TTFinitializer::TTFinitializer()
{
  // try to initialize True Type Font engine and check if it was done
  // successfully. Throw SDLexception if failed
  if (TTF_Init() == -1) {
    throw SDLexception(
        "Failed to initialize TTF support", TTF_GetError(), __FILE__, __LINE__);
  }
}

// Destructor. Closes the True Type Font SDL engine
TTFinitializer::~TTFinitializer()
{
  // Close TTF engine
  TTF_Quit();
}
