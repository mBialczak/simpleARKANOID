#ifndef SDLInitializers_HPP
#define SDLInitializers_HPP
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include <cstddef>

// A set of helper classes for initializing SDL support

// RAII class responsible for initializing and closing SDL subsystems
// Throws SDLexception if initialization fails
class SDLinitializer
{
  public:
  // Constructor. Tries to initialize SDL requested subsystems.
  // Takes SDL specific flags (most likely OR-ed)
  // Throws SDLexception if initialization fails
  SDLinitializer(Uint32 flags = SDL_INIT_EVERYTHING);

  // Destructor. Closes SDL subsystems
  ~SDLinitializer();
};

// RAII class responsible for initializing and realeasing SDL_Image resources
// Throws SDLexception if initialization fails
class ImageInitializer
{
  public:
  // Constructor. Tries to initialize SDL Image support for image files
  // specified by SDL Image flags (possibly OR-ed).
  // Throws SDLexception if initialization fails
  ImageInitializer(
      int flags = IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF | IMG_INIT_WEBP);

  // Destructor. Unloads libraries loaded with IMG_Init
  ~ImageInitializer();
};

// RAII class responsible for initializing and closing True Type Font engine
// Throws SDLexception if initialization fails
class TTFinitializer
{
  public:
  // Constructor. Tries to initialize True Type Font SDL engine.
  // Throws SDLexception if initialization fails
  TTFinitializer();

  // Destructor. Closes the True Type Font SDL engine
  ~TTFinitializer();
};

#endif // !SDLInitializers_HPP