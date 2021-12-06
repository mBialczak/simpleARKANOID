#include "Texture.hpp"
#include "LoadedSDLsufrace.hpp"
#include "SDLexception.hpp"
#include <utility>

// for operator""s usage
using std::string_literals::operator""s;

// constructor // REVIEW: COMMENT
// takes path of the image file and renderer for texture creation and rendering
Texture::Texture(const std::string& imagePath, SDL_Renderer* gameRenderer)
    : _sdl_texture(nullptr)
    , _sdl_renderer(gameRenderer)
    , _font(nullptr)
    , _width(0)
    , _height(0)
{
  // REVIEW: perhpas replace with unique_ptr with custom deleter? and delete
  // Loaded.. clas?? load image at specified path into a surface surface will be
  // automatically freed by LoadedSDLsurface wrapper
  LoadedSDLsufrace image_surface(imagePath);

  // create surface from image surface
  _sdl_texture = SDL_CreateTextureFromSurface(
      gameRenderer, image_surface.GetSurfacePtr());

  // check if texture created succesfully and throw if not
  if (!_sdl_texture) {
    throw SDLexception("Failed to create texture from path: "s + imagePath,
        IMG_GetError(), __FILE__, __LINE__);
  }
  // set texture dimensions basing on loaded image size
  _width = image_surface.GetSurfacePtr()->w;
  _height = image_surface.GetSurfacePtr()->h;
}

// REMOVE INU or doesn't work or COMMENT
Texture::Texture(const std::string& fontPath, SDL_Color color, int textSize,
    SDL_Renderer* gameRenderer, const std::string& text)
    : _sdl_texture(nullptr)
    , _sdl_renderer(gameRenderer)
    , _font(nullptr)
    , _width(0)
    , _height(0)
{
  auto font_deleter = [](TTF_Font* ptr) { TTF_CloseFont(ptr); };
  // REVIEW: local variable or property?
  std::unique_ptr<TTF_Font, decltype(font_deleter)> font { TTF_OpenFont(
      fontPath.c_str(), textSize) };

  // REVIEW:check if the font was created succesfully and throw if not
  if (!font) {
    throw SDLexception("Failed to open font from path: "s + fontPath,
        TTF_GetError(), __FILE__, __LINE__);
  }

  auto surface_deleter = [](SDL_Surface* ptr) { SDL_FreeSurface(ptr); };

  std::unique_ptr<SDL_Surface, decltype(surface_deleter)> text_surface {
    TTF_RenderText_Solid(font.get(), text.c_str(), color)
  };

  if (!text_surface) {
    throw SDLexception("Failed to render text surface! text:"s + text,
        TTF_GetError(), __FILE__, __LINE__);
  }

  _sdl_texture
      = SDL_CreateTextureFromSurface(_sdl_renderer, text_surface.get());

  // check if texture created succesfully and throw if not
  if (!_sdl_texture) {
    throw SDLexception("Failed to create texture from  rendered text: "s + text,
        SDL_GetError(), __FILE__, __LINE__);
  }
  // set own dimensions basing on loaded image size
  _width = text_surface->w;
  _height = text_surface->h;
}

// destructor // REVIEW:
// consider making unique_ptr as well
Texture::~Texture()
{ // destroy texture if any exists
  if (_sdl_texture) {
    SDL_DestroyTexture(_sdl_texture);
  }
  // REMOVE if unique works
  // // destroy font if eny exists
  // if (_font) {
  //   TTF_CloseFont(_font);
  // }
}

// REVIEW: if unique_ptr applied for sdl texture
// move constructor
Texture::Texture(Texture&& moved)
    : _sdl_texture(moved._sdl_texture)
    , _font(std::move(moved._font))
    , _width(moved._width)
    , _height(moved._height)
{
  moved._sdl_texture = nullptr;
  moved._width = 0;
  moved._height = 0;
}

// move assignment operator
Texture& Texture::operator=(Texture&& moved)
{
  // REVIEW: if unique pointer applied
  // destroy owned SDL texture
  if (_sdl_texture) {
    SDL_DestroyTexture(_sdl_texture);
  }
  // copy properties from moved object
  _sdl_texture = moved._sdl_texture;
  _width = moved._width;
  _height = moved._height;
  // move properties governed by unique_ptr
  _font = std::move(moved._font);
  // invalidate moved object
  moved._sdl_texture = nullptr;
  moved._width = 0;
  moved._height = 0;
  return *this;
}

// renders texture in position x, y
void Texture::Render(int x, int y) const
{
  // create destination rendering rectangle and set sizes
  SDL_Rect render_rect; // { x, y, _width, _height };
  render_rect.w = _width;
  render_rect.h = _height;
  // center the position of the rectangle around texture center
  render_rect.x = x - render_rect.w / 2;
  render_rect.y = y - render_rect.h / 2;
  // Render to screen; NULL for entire texture
  SDL_RenderCopy(_sdl_renderer, _sdl_texture, NULL, &render_rect);
}