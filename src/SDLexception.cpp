#include "SDLexception.hpp"
#include <string>

// constructor
SDLexception::SDLexception(const std::string& firstPart,
    const char* errorCodePart, const char* file, int line)
    : _firstPart(firstPart)
    , _errorCodePart(errorCodePart)
    , _file(file)
    , _line(line)
{
}

// returns exception description
std::string SDLexception::Description() const
{
  return std::string("SDL-related exception in file: ") + _file
      + "line no.: " + std::to_string(_line) + _separator
      + "PROGRAMMER description: " + _firstPart + _separator
      + "SDL description: " + _errorCodePart + _separator;
}