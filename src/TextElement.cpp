#include "TextElement.hpp"

// Constructor.
// Takes: x,y coordinates of text element on the screen,
// path of the font file, color of text, size of text (pixels), renderer
// (for texture creation and rendering) and the text to be displayed.
//  May throw SDLexception if construction of member _texture fails
TextElement::TextElement(float x, float y, const std::string& fontPath,
    SDL_Color color, int textSize, SDL_Renderer* gameRenderer,
    const std::string& text)
    : StaticObject(x, y)
    , _texture(std::make_unique<Texture>(
          fontPath, color, textSize, gameRenderer, text))
{
}

// Displays text onto the screen
void TextElement::Draw() const
{
  _texture->Render(
      static_cast<int>(_position._x), static_cast<int>(_position._y));
}

// Returns width of the text element
float TextElement::Width() const { return _texture->Width(); }