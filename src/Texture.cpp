#include "Texture.hpp"
#include "LoadedSDLsufrace.hpp"
#include "SDLexception.hpp"
#include <utility>

// constructor
// takes path of the image file and renderer for texture creation and rendering
Texture::Texture(const std::string& Path, SDL_Renderer* GameRenderer)
    : _sdl_texture(nullptr)
    , _sdl_renderer(GameRenderer)
    , _width(0)
    , _height(0)
{
  // load image at specified path into a surface
  // surface will be automatically freed by LoadedSDLsurface wrapper
  LoadedSDLsufrace image_surface(Path);

  // create surface from image surface
  _sdl_texture = SDL_CreateTextureFromSurface(
      GameRenderer, image_surface.GetSurfacePtr());

  // check if texture created succesfully and throw if not
  if (!_sdl_texture) {
    std::string error { "Failed to create texture from path: " };
    error += Path;
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
Texture::Texture(Texture&& Other)
    : _sdl_texture(Other._sdl_texture)
    , _width(Other._width)
    , _height(Other._height)
{
  Other._sdl_texture = nullptr;
  Other._width = 0;
  Other._height = 0;
}

// move assignment operator
Texture& Texture::operator=(Texture&& Moved)
{
  // destroy owned SDL texture
  if (_sdl_texture) {
    SDL_DestroyTexture(_sdl_texture);
  }
  // copy properties from moved object
  _sdl_texture = Moved._sdl_texture;
  _width = Moved._width;
  _height = Moved._height;
  // invalidate moved object
  Moved._sdl_texture = nullptr;
  Moved._width = 0;
  Moved._height = 0;
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