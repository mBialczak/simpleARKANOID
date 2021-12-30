#ifndef SDLexception_HPP
#define SDLexception_HPP

#include <string>

// custom exception class for handling SDL errors
class SDLexception
{
  public:
  // constructor
  SDLexception(const std::string& firstPart, const char* errorCodePart,
      const char* file, int line);

  // returns exception description
  std::string Description() const;

  private:
  // description part provided by the programmer
  const std::string firstPart_;
  // description part provided by SDL-related library
  const std::string errorCodePart_;
  // source code file in which error occured
  const std::string file_;
  // line number of the source code file
  const int line_;
  // separator used for output formating
  const std::string separator_ = "\n";
};
#endif // !SDLexception_HPP