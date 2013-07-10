/* Copyright © 2010 Pippijn van Steenhoven
 * See COPYING.AGPL for licence information.
 */
#include "phoenix.h"

#include "stacktrace.h"

#if HAVE_BFD_H
#include "file_match.h"
#include "frame.h"

#include <link.h>

#include <cstring>

stacktrace::frame
stacktrace::resolve_frame_internal (char const *base)
{
  file_match match { 0, base, 0, 0 };
  dl_iterate_phdr (find_matching_file, &match);
  bfd_vma addr = base - match.base;
  if (match.file && std::strlen (match.file))
    return process_file (match.file, &addr, 1);
  else
    return process_file ("/proc/self/exe", &addr, 1);
}

#endif
