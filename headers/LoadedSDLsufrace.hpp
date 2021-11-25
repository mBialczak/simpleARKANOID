#ifndef LoadedSDLsufrace_HPP
#define LoadedSDLsufrace_HPP
#include "SDL.h"
#include "SDL_image.h"
#include <string>

// Simple wrapper class for SDL surface loaded from file specified by Path
// Will free the surface upon destruction, providing safe resource managment,
// espacially in case of exceptions
// Class is designed mainly for short term surface usage, after which no longer
// needed surface will be automatically freed
class LoadedSDLsufrace {
  public:
  // constructor loading SDL surface from file specified by Path
  // throws exception in case of loading problem
  explicit LoadedSDLsufrace(const std::string& Path);

  // destructor freeing the loaded SDL surface
  ~LoadedSDLsufrace();

  // class is not predicted for copying and copying might cause owned resources
  // managment problems so copy operations ara disabled
  LoadedSDLsufrace(const LoadedSDLsufrace&) = delete;
  LoadedSDLsufrace& operator=(const LoadedSDLsufrace&) = delete;

  // class is not predicted for moving so moving operations are disabled
  LoadedSDLsufrace(LoadedSDLsufrace&&) = delete;
  LoadedSDLsufrace& operator=(LoadedSDLsufrace&&) = delete;

  // returns pointer to managed SDL surface pointer
  // user of the class should not explicitly free the surface
  SDL_Surface* GetSurfacePtr() const { return _loaded_surface; }

  private:
  // owned SDL sufrace, loaded from file
  SDL_Surface* _loaded_surface;
};
#endif // !LoadedSDLsufrace_HPP
