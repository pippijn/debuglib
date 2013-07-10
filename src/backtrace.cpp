/* Copyright © 2010 Pippijn van Steenhoven
 * See COPYING.AGPL for licence information.
 */
#include "backtrace.h"
#include "array_size.h"
#include "demangle.h"

#include <boost/foreach.hpp>

#include <cassert>
#include <climits>
#include <cstdarg>
#include <cstdio>
#include <cstdlib>

#if HAVE_EXECINFO_H
#include <execinfo.h>
#endif
#include <unistd.h>

void
std_print (char const *fmt)
{
  printf ("==%d==    %s\n", getpid (), fmt);
}

static void
printf (print_fn print, char const *fmt, ...)
{
  va_list ap;

  va_start (ap, fmt);
  char buffer[512];
  vsnprintf (buffer, sizeof buffer, fmt, ap);
  va_end (ap);

  print (buffer);
}

frame *
backtrace_frames_current (int skip) throw ()
{
  ++skip;
#if HAVE_EXECINFO_H
  static void *buffer[1024];
  size_t size = backtrace (buffer, ARRAY_SIZE (buffer));
  assert (size <= INT_MAX);
  return backtrace_frames (buffer + skip, (int)size - skip);
#else
  return NULL;
#endif
}

void
print_frames (frame const *frames, print_fn print) throw ()
{
  if (frames)
    {
      frame const *current = frames;

      char const *skip[] = {
        "_ZL12fault_actioniP7siginfoPv",
        "fault_action",
        "killpg",
        "raise",
        "abort",
      };
      char const *ignore[] = {
        "__libc_start_main",
        "_start",
      };
      BOOST_FOREACH (char const *func, skip)
        {
          if (!strncmp (current->func, func, strlen (func)))
            {
              ++current;
              ++frames;
            }
        }
      while (current->file[0])
        {
          std::string func;
          demangle_symbol (current->func, func);
          printf ( print
                 , "%s %s (%s:%d)"
                 , current == frames ? "at" : "by"
                 , func.c_str ()
                 , current->file
                 , current->line
                 );
          ++current;
          BOOST_FOREACH (char const *func, ignore)
            {
              if (!strcmp (current->func, func))
                {
                  ++current;
                  ++frames;
                }
            }
        }
    }
  else
    print ("stack trace is not available on this system");
}

void
print_backtrace (print_fn print) throw ()
{
  frame *frames = backtrace_frames_current (1);

  print_frames (frames, print);
  delete[] frames;
}
