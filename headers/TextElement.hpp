#ifndef TextElement_HPP
#define TextElement_HPP
#include "Texture.hpp"
#include <memory>
#include <string>

// TODO: COMMENT
// simple class containing atexture with text to display
struct TextElement : public StaticObject
{
  public:
  // Constructor.
  // Takes: x,y coordinates of text element on the screen,
  // path of the font file, color of text, size of text (pixels), renderer
  // (for texture creation and rendering) and the text to be displayed.
  //  May throw SDLexception if construction of member _texture fails
  TextElement(float x, float y, const std::string& fontPath, SDL_Color color,
      int textSize, SDL_Renderer* gameRenderer, const std::string& text);

  // REVIEW:
  // Displays text texture to the screen

  private:
  // pointer texture containing text to be displayed
  std::unique_ptr<Texture> _texture;
};

#endif // !TextElement_HPP
