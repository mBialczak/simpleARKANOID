#include "TextElement.hpp"

// Constructor.
// Takes: x,y coordinates of text element on the screen,
// path of the font file, color of text, size of text (pixels), renderer
// (for texture creation and rendering) and the text to be displayed.
//  May throw SDLexception if construction of member _texture fails
TextElement::TextElement(float x, float y, const std::string& fontPath,
    SDL_Color color, int textSize, SDL_Renderer* gameRenderer,
    const std::string& text)
    : _texture(std::make_unique<Texture>(
        fontPath, color, textSize, gameRenderer, text))
    , _x(x)
    , _y(y)
{
}