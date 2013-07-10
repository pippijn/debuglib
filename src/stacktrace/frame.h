/* Copyright © 2010 Pippijn van Steenhoven
 * See COPYING.AGPL for licence information.
 */
#include <string>

struct stacktrace::frame
{
  std::string func;
  std::string file;
  unsigned long line;

  frame ()
    : func ("func")
    , file ("file")
    , line (0)
  {
  }

  frame (char const *func, char const *file, unsigned long line)
    : func (func)
    , file (file)
    , line (line)
  {
  }

  frame (frame &&rhs)
    : func (std::move (rhs.func))
    , file (std::move (rhs.file))
    , line (std::move (rhs.line))
  {
  }

  frame &operator = (frame &&rhs)
  {
    func = std::move (rhs.func);
    file = std::move (rhs.file);
    line = std::move (rhs.line);

    return *this;
  }

  frame (frame const &rhs) = delete;
  frame &operator = (frame const &rhs) = delete;
};
