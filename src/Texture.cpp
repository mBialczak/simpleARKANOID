#include "Texture.hpp"
#include "LoadedSDLsufrace.hpp"
#include "SDLexception.hpp"
#include <utility>

// constructor
// takes path of the image file and renderer for texture creation and rendering
Texture::Texture(const std::string& path, SDL_Renderer* gameRenderer)
    : _sdl_texture(nullptr)
    , _sdl_renderer(gameRenderer)
    , _width(0)
    , _height(0)
{
  // load image at specified path into a surface
  // surface will be automatically freed by LoadedSDLsurface wrapper
  LoadedSDLsufrace image_surface(path);

  // create surface from image surface
  _sdl_texture = SDL_CreateTextureFromSurface(
      gameRenderer, image_surface.GetSurfacePtr());

  // check if texture created succesfully and throw if not
  if (!_sdl_texture) {
    std::string error { "Failed to create texture from path: " };
    error += path;
    throw SDLexception(error, IMG_GetError(), __FILE__, __LINE__);
  }
  // set texture dimensions basing on louded image size
  _width = image_surface.GetSurfacePtr()->w;
  _height = image_surface.GetSurfacePtr()->h;
}
// destructor
Texture::~Texture()
{
  if (_sdl_texture)
    SDL_DestroyTexture(_sdl_texture);
}

// move constructor
Texture::Texture(Texture&& other)
    : _sdl_texture(other._sdl_texture)
    , _width(other._width)
    , _height(other._height)
{
  other._sdl_texture = nullptr;
  other._width = 0;
  other._height = 0;
}

// move assignment operator
Texture& Texture::operator=(Texture&& moved)
{
  // destroy owned SDL texture
  if (_sdl_texture) {
    SDL_DestroyTexture(_sdl_texture);
  }
  // copy properties from moved object
  _sdl_texture = moved._sdl_texture;
  _width = moved._width;
  _height = moved._height;
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