#include "demangle.h"

#include <cstring>
#include <cxxabi.h>

char *unDName (char *buffer, const char *mangled, int buflen, void *memget (size_t), void memfree (void*), unsigned short int flags);

void
demangle_symbol (char const *input, std::string &output)
{
  if (!strncmp (input, "_Z", 2))
    {
      char buffer[1024];
      size_t length = sizeof buffer;
      int status;
      __cxxabiv1::__cxa_demangle (input, buffer, &length, &status);
      switch (status)
        {
        case 0:
          output.assign (buffer, length);
          break;
        case -1:
          output = "<allocation failure>";
          break;
        case -2:
          output = "<invalid mangled name>";
          break;
        case -3:
          output = "<invalid argument>";
          break;
        default:
          output = "<uknown error>";
          break;
        }
    }
  else if (input[0] == '?')
    {
      char buffer[1024] = { 0 };
      unDName (buffer, input, sizeof buffer, malloc, free, 0);
      output = buffer;
    }
  else
    {
      output = input;
    }
}
