#include "SDLexception.hpp"
#include <string>

// constructor
SDLexception::SDLexception(const std::string& firstPart,
    const char* errorCodePart, const char* file, int line)
    : firstPart_(firstPart)
    , errorCodePart_(errorCodePart)
    , file_(file)
    , line_(line)
{
}

// returns exception description
std::string SDLexception::Description() const
{
  return std::string("SDL-related exception in file: ") + file_
      + "line no.: " + std::to_string(line_) + separator_
      + "PROGRAMMER description: " + firstPart_ + separator_
      + "SDL description: " + errorCodePart_ + separator_;
}