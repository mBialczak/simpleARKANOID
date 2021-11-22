#include "SDLexception.hpp"
#include <string>

// constructor
SDLexception::SDLexception(
    const char* firstPart, const char* errorCodePart, const char* file, int line)
    : firstPart(firstPart)
    , errorCodePart(errorCodePart)
    , file(file)
    , line(line)
{
}

// returns exception description
std::string SDLexception::Description() const
{
  return std::string("SDL-related exception in file: ") + file + "line no.: "
      + std::to_string(line) + separator + "PROGRAMMER description: " + firstPart
      + separator + "SDL description: " + errorCodePart + separator;
}