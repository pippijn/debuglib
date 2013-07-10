/* Copyright © 2010 Pippijn van Steenhoven
 * See COPYING.AGPL for licence information.
 */
#include <cmath>
#include <cstring>

#include "stacktrace.h"

#if HAVE_BFD_H
#include "dlbfd.h"
#include "frame.h"

#include "log.h"

#include <cxxabi.h>

static size_t const ptrstr_len = log<16> (SIZE_MAX) + sizeof "0x";

stacktrace::frame
stacktrace::translate_addresses (bfd *abfd, bfd_vma *addr, int naddr)
{
  pc = addr[naddr - 1];

  found = false;
  bfd_map_over_sections (abfd, find_address_in_section, this);

  if (!found)
    {
      char addrstr[ptrstr_len + 3];
      snprintf (addrstr, sizeof addrstr, "0x%lx", (unsigned long)addr[naddr - 1]);
      return { addrstr, "???", 0 };
    }
  else
    {
      if (funcname == NULL || *funcname == '\0')
        funcname = "???";
      if (filename == NULL)
        filename = "???";
      else
        if (char const *h = strrchr (filename, '/'))
          filename = h + 1;
      return { funcname, filename, line };
    }
}

#endif
