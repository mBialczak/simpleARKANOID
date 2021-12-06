#ifndef Texture_HPP
#define Texture_HPP
#include "Renderer.hpp"
#include "SDL.h"
#include "SDL_ttf.h"
#include <functional>
#include <memory>
#include <string>

// wrapper class for creating SDL_texture.
// Used for displaying images and render text
class Texture
{
  public:
  // Constructor creating texture from the image.
  // Takes path of the image file to load and renderer for texture creation and
  // rendering. Throws SDLexception if construction fails
  Texture(const std::string& imagePath, SDL_Renderer* gameRenderer);

  // Constructor creating texture from the text to be rendered.
  // Takes path of the font file, color of text, size of text (pixels),renderer
  // (for texture creation and rendering) and the text to be displayed.
  //  Throws SDLexception if construction fails
  Texture(const std::string& fontPath, SDL_Color color, int textSize,
      SDL_Renderer* gameRenderer, const std::string& text);

  // copying of class not predicted due to use of unique_ptr and no overall
  // sense of copying , so copy operations are disabled
  Texture(const Texture&) = delete;
  Texture& operator=(const Texture&) = delete;
  // because the only owned resource is managed with unique_ptr,
  // default destructor and move operations can be used
  ~Texture() = default;
  Texture(Texture&& other) = default;
  Texture& operator=(Texture&& moved) = default;

  // gets texture width
  int Width() const { return _width; }

  // gets texture height
  int Height() const { return _height; }

  // returns pointer to owned SDL_Texture
  // user of the class should not explicitly destroy the texture
  SDL_Texture* GetSDLtexture() const { return _sdl_texture.get(); }

  // renders texture in position x, y
  void Render(int x, int y) const; // NOTE: const might not work

  private:
  // OWNED unique pointer to SDL texture with custom deleter
  std::unique_ptr<SDL_Texture, std::function<void(SDL_Texture*)>> _sdl_texture;

  // NOT OWNED pointer to SDL renderer for texture creation and future rendering
  // REVIEW: check if Renderer* or Renderer& would be better
  SDL_Renderer* _sdl_renderer;

  // image dimmensions
  int _width;
  int _height;
};

#endif // !Texture_HPP