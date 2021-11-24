#include "LoadedSDLsufrace.hpp"
#include "SDLexception.hpp"

// constructor
// tries to load SDL surface from file specified by Path
// throws exception if loading was unsuccessfull
LoadedSDLsufrace::LoadedSDLsufrace(const std::string& Path)
    : _loaded_surface(IMG_Load(Path.c_str()))
{
  if (_loaded_surface == nullptr) {
    std::string error { "Unable to load image: " };
    error += Path;
    throw SDLexception { error, IMG_GetError(), __FILE__, __LINE__ };
  }
}

// destructor freeing the loaded SDL surface
LoadedSDLsufrace::~LoadedSDLsufrace() { SDL_FreeSurface(_loaded_surface); }
