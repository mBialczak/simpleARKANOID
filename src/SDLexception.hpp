#ifndef SDLexception_HPP
#define SDLexception_HPP

#include <string>

// custom exception class for handling SDL errors
class SDLexception {
  public:
  // constructor
  SDLexception(
      const char* firstPart, const char* errorCodePart, const char* file, int line);

  // returns exception description
  std::string Description() const;

  private:
  // description part provided by programer
  const std::string firstPart;
  // description part provided by SDL-related library
  const std::string errorCodePart;
  // source code file in which error occured
  const std::string file;
  // line number of the source code file
  const int line;
  // separator used for output formating
  const std::string separator = "\n";
};
#endif // !SDLexception_HPP