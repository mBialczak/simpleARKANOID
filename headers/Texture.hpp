#ifndef Texture_HPP
#define Texture_HPP
#include "Renderer.hpp"
#include "SDL.h"
#include <string>

// wrapper class for creating SDL_texture
class Texture
{
  public:
  // constructor creating SDL texture from file designated by path
  Texture(const std::string& Path, SDL_Renderer* gameRenderer);

  // copying of class not predicted and copy operations may cause
  // problems with managed resources, so copy operations are disabled
  Texture(const Texture&) = delete;
  Texture& operator=(const Texture&) = delete;

  // custom move constructor
  Texture(Texture&& other);

  // custom move assignment operator
  Texture& operator=(Texture&& moved);

  // destructor destroys the owned SDL surface
  ~Texture();

  // gets texture width
  // NOTE: may be not used - consider removing
  int Width() const { return _width; }

  // gets texture height
  int Height() const { return _height; }

  // returns pointer to owned SDL_Texture
  // user of the class should not explicitly destroy the texture
  SDL_Texture* GetSDLtexture() const { return _sdl_texture; }

  // renders texture in position x, y
  void Render(int x, int y) const; // NOTE: const might not work

  private:
  // OWNED pointer to SDL texture
  SDL_Texture* _sdl_texture;

  // NOT OWNED pointer to SDL renderer for texture creation and future rendering
  // REVIEW: check if Renderer* or Renderer& would be better
  SDL_Renderer* _sdl_renderer;

  // image dimmensions
  int _width;
  int _height;
};

#endif // !Texture_HPP