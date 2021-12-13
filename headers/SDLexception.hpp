#ifndef SDLexception_HPP
#define SDLexception_HPP

#include <string>

// custom exception class for handling SDL errors
class SDLexception {
  public:
  // constructor
  SDLexception(const std::string& firstPart, const char* errorCodePart,
      const char* file, int line);

  // returns exception description
  std::string Description() const;

  private:
  // description part provided by the programmer
  const std::string _firstPart;
  // description part provided by SDL-related library
  const std::string _errorCodePart;
  // source code file in which error occured
  const std::string _file;
  // line number of the source code file
  const int _line;
  // separator used for output formating
  const std::string _separator = "\n";
};
#endif // !SDLexception_HPP