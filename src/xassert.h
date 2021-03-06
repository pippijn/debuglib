/* Copyright © 2010 Pippijn van Steenhoven
 * See COPYING.AGPL for licence information.
 */
#pragma once

#include "common.h"

#include <stdexcept>

struct VISIBLE assertion_failure
  : std::runtime_error
{
  assertion_failure (char const *msg, char const *file, int line, char const *func)
    : std::runtime_error (construct (msg, file, line, func))
  {
  }

  std::string construct (char const *msg, char const *file, int line, char const *func);
};

#define xassert(cond)                                   \
  do {                                                  \
    if (!(cond))                                        \
      throw assertion_failure (#cond,                   \
                               __FILE__,                \
                               __LINE__,                \
                               __PRETTY_FUNCTION__);    \
  } while (0)
