#ifndef Texture_HPP
#define Texture_HPP
#include "Renderer.hpp"
#include "SDL.h"
#include "SDL_ttf.h"
#include <string>

// TODO: update comments
// wrapper class for creating SDL_texture
class Texture
{
  public:
  // constructor creating SDL texture from file designated by path
  Texture(const std::string& imagePath, SDL_Renderer* gameRenderer);

  // REMOVE INU or doesn't work or COMMENT
  Texture(const std::string& fontPath, SDL_Color color, int textSize,
      SDL_Renderer* gameRenderer, const std::string& text);

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
  // NOTE: consider returning back int from float
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

  // REVIEW: changes made for text display MAYBE not needed
  // OWNED pointer to true type font
  TTF_Font* _font;
  // image dimmensions
  int _width;
  int _height;
};

#endif // !Texture_HPP