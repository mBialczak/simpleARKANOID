#include "Texture.hpp"
#include "SDLexception.hpp"
#include <iostream> // REMOVE AT
#include <utility>

// for operator""s usage
using std::string_literals::operator""s;

// Default Constructor
Texture::Texture()
    : _sdl_texture(nullptr)
    , _sdl_renderer(nullptr)
    , _width(0)
    , _height(0)
{
}

// Constructor creating texture from the image.
// Takes path of the image file to load and renderer for texture creation and
// rendering. Throws SDLexception if construction fails
Texture::Texture(const std::string& imagePath, SDL_Renderer* gameRenderer)
    : _sdl_texture(nullptr)
    , _sdl_renderer(gameRenderer)
    , _width(0)
    , _height(0)
{
  // create pointer for temporary SDL_Surface which is needed only for texture
  // creation and will be  automatically destroyed on leaving function or if an
  // exception occurs. Custom deleter is used due to specific SDL destruction
  // function
  std::unique_ptr<SDL_Surface, std::function<void(SDL_Surface*)>> img_surface(
      IMG_Load(imagePath.c_str()),
      [](SDL_Surface* ptr) { SDL_FreeSurface(ptr); });

  // check if suface was created successfully and throw if not
  if (!img_surface)
    throw SDLexception { "Unable to load image: "s + imagePath, IMG_GetError(),
      __FILE__, __LINE__ };

  // Set the ptr to SDL Texture.
  // Needs to be done here (not in the constructor initilializer list), because
  // first the the img_surface needs to be created. Custom deleter is used due
  // to specific SDL texture destructon function
  _sdl_texture = std::move(
      std::unique_ptr<SDL_Texture, std::function<void(SDL_Texture*)>> {
          SDL_CreateTextureFromSurface(gameRenderer, img_surface.get()),
          [](SDL_Texture* ptr) { SDL_DestroyTexture(ptr); } });

  // check if texture created succesfully and throw if not
  if (!_sdl_texture) {
    throw SDLexception("Failed to create texture from path: "s + imagePath,
        IMG_GetError(), __FILE__, __LINE__);
  }
  // set texture dimensions basing on loaded image size
  _width = img_surface.get()->w;
  _height = img_surface.get()->h;
}

// Constructor creating texture from the text to be rendered.
// Takes path of the font file, color of text, size of text (pixels),renderer
// (for texture creation and rendering) and the text to be displayed.
//  Throws SDLexception if construction fails
Texture::Texture(const std::string& fontPath, SDL_Color color, int textSize,
    SDL_Renderer* gameRenderer, const std::string& text)
    : _sdl_texture(nullptr)
    , _sdl_renderer(gameRenderer)
    , _width(0)
    , _height(0)
{
  //  create pointer for temporary TTF_Font, which is needed only for
  //  SDL_Surface creation (in the next step) and will be  automatically
  //  destroyed on leaving function or if an exception occurs. Custom deleter is
  //  used due to specific SDL destructionfunction
  std::unique_ptr<TTF_Font, std::function<void(TTF_Font*)>> font {
    TTF_OpenFont(fontPath.c_str(), textSize),
    [](TTF_Font* ptr) { TTF_CloseFont(ptr); }
  };

  // check if the font was created succesfully and throw if not
  if (!font) {
    throw SDLexception("Failed to open font from path: "s + fontPath,
        TTF_GetError(), __FILE__, __LINE__);
  }

  // create pointer for temporary SDL_Surface which is needed only for texture
  // creation and will be  automatically destroyed on leaving function or if an
  // exception occurs. Custom deleter is used due to specific SDL destruction
  // function
  std::unique_ptr<SDL_Surface, std::function<void(SDL_Surface*)>> text_surface {
    TTF_RenderText_Solid(font.get(), text.c_str(), color),
    [](SDL_Surface* ptr) { SDL_FreeSurface(ptr); }
  };

  // check if texture created succesfully and throw if not
  if (!text_surface) {
    throw SDLexception("Failed to render text surface! text:"s + text,
        TTF_GetError(), __FILE__, __LINE__);
  }

  // Set the ptr to SDL Texture.
  // Needs to be done here (not in the constructor initilializer list),
  // because first the the text_surface needs to be created.
  // Custom deleter is used due to specific SDL texture destructon function
  _sdl_texture = std::move(
      std::unique_ptr<SDL_Texture, std::function<void(SDL_Texture*)>> {
          SDL_CreateTextureFromSurface(gameRenderer, text_surface.get()),
          [](SDL_Texture* ptr) { SDL_DestroyTexture(ptr); } });

  // check if texture created succesfully and throw if not
  if (!_sdl_texture) {
    throw SDLexception("Failed to create texture from  rendered text: "s + text,
        SDL_GetError(), __FILE__, __LINE__);
  }
  // set own dimensions basing on loaded image size
  _width = text_surface->w;
  _height = text_surface->h;
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
  SDL_RenderCopy(_sdl_renderer, _sdl_texture.get(), NULL, &render_rect);
}