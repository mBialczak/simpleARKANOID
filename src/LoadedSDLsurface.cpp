#include "LoadedSDLsufrace.hpp"
#include "SDLexception.hpp"

// for operator""s usage
using std::string_literals::operator""s;

// constructor
// tries to load SDL surface from file specified by Path
// throws exception if loading was unsuccessfull
LoadedSDLsufrace::LoadedSDLsufrace(const std::string& path)
    : _loaded_surface(IMG_Load(path.c_str()))
{
  if (_loaded_surface == nullptr) {
    throw SDLexception { "Unable to load image: "s + path, IMG_GetError(),
      __FILE__, __LINE__ };
  }
}

// destructor freeing the loaded SDL surface
LoadedSDLsufrace::~LoadedSDLsufrace() { SDL_FreeSurface(_loaded_surface); }
