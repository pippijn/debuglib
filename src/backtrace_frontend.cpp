/* Copyright © 2010 Pippijn van Steenhoven
 * See COPYING.AGPL for licence information.
 */
#include <exception>
#include <vector>

#include <cmath>
#include <cstdio>
#include <cstddef>
#include <cstring>

#include "backtrace.h"
#include "log.h"

#include "stacktrace/stacktrace.h"
#include "stacktrace/frame.h"

static stacktrace stk;

char**
backtrace_symbols (void* const* buffer, int size) throw ()
try
{
  static size_t const intstr_len = log<10> (SIZE_MAX) + 1;

  std::vector<stacktrace::frame> const frames = stk.backtrace_symbols (buffer, size);

  int const width = 30;
  size_t required = 0;
  for (auto it = frames.begin (),
            et = frames.end ();
       it != et; ++it)
    {
      required += it->file.length ();
      required += 1; // ':'
      required += intstr_len;
      required += width - 3;    // field width for file:line (-3 because a
                                // file and line both have at least 1
                                // character and we have ':' in between)
      required += it->func.length ();
      required += 1; // '\0'
    }

  size_t const memsize = required + frames.size () * sizeof (char*);
  char* const membase = static_cast<char*> (malloc (memsize));
  char* const memend  = membase + memsize;
  char** const syms = reinterpret_cast<char**> (membase);
  char* strings = membase + frames.size () * sizeof (char*);

  for (auto it = frames.begin (),
            et = frames.end ();
       it != et; ++it)
    {
      ptrdiff_t i = it - frames.begin ();
      syms[i] = strings;
      char buf[it->file.length () + intstr_len + 2];
      if (it->line)
        snprintf (buf, sizeof buf, "%s:%lu", it->file.c_str (), it->line);
      else
        snprintf (buf, sizeof buf, "%s:???", it->file.c_str ());
      strings += snprintf (strings, memend - membase,
                           "%-*s%s", width, buf, it->func.c_str ());
      ++strings; // '\0'
    }

  return syms;
}
catch (std::exception const& e)
{
  printf ("caught unexpected exception while building backtrace: %s\n", e.what ());
  return NULL;
}
catch (...)
{
  puts ("caught unexpected exception while building backtrace");
  return NULL;
}

frame*
backtrace_frames (void* const* buffer, int size) throw ()
try
{
  std::vector<stacktrace::frame> const frames = stk.backtrace_symbols (buffer, size);

  frame* const ret = new frame[frames.size () + 1] ();

  for (auto it = frames.begin (),
            et = frames.end ();
       it != et; ++it)
    {
      ptrdiff_t i = it - frames.begin ();

      strcpy (ret[i].func, it->func.c_str ());
      strcpy (ret[i].file, it->file.c_str ());
      ret[i].line = it->line;
    }

  return ret;
}
catch (std::exception const& e)
{
  printf ("caught unexpected exception while building backtrace: %s\n", e.what ());
  return NULL;
}
catch (...)
{
  puts ("caught unexpected exception while building backtrace");
  return NULL;
}

char*
resolve_symbol (void const* sym) throw ()
try
{
  return strdup (stk.resolve_frame (sym).func.c_str ());
}
catch (std::exception const& e)
{
  printf ("caught unexpected exception while resolving symbol: %s\n", e.what ());
  return NULL;
}
catch (...)
{
  puts ("caught unexpected exception while building backtrace");
  return NULL;
}
