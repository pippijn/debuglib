/* Copyright © 2010 Pippijn van Steenhoven
 * See COPYING.AGPL for licence information.
 */
#include "xassert.h"

#include "stacktrace.h"

#if HAVE_BFD_H
#include "dlbfd.h"
#include "frame.h"

#include <boost/foreach.hpp>

#include <string>
#include <unordered_map>

static struct bfd_cache
{
  bfd_cache ()
    : cache ()
  {
  }

  ~bfd_cache ()
  {
    BOOST_FOREACH (auto pair, cache)
      if (pair.second)
        bfd_close (pair.second);
  }

  bfd *operator () (char const *file_name)
  {
    if (cache.find (file_name) != cache.end ())
      return cache[file_name];
    return cache[file_name] = bfd_openr (file_name, NULL);
  }

  std::unordered_map<char const *, bfd *> cache;
} cache_bfd;

stacktrace::frame
stacktrace::process_file (char const *file_name, bfd_vma *addr, int naddr)
{
  bfd *abfd = cache_bfd (file_name);

  if (abfd == NULL)
    bfd_fatal (file_name);

  if (bfd_check_format (abfd, bfd_archive))
    fatal ("%s: can not get addresses from archive", file_name);

  char **matching;
  if (!bfd_check_format_matches (abfd, bfd_object, &matching))
    {
      bfd_nonfatal (bfd_get_filename (abfd));
      if (bfd_get_error () == bfd_error_file_ambiguously_recognized)
        {
          list_matching_formats (matching);
          free (matching);
        }
      exit (1);
    }

  frame frame;
  {
    xassert (!syms);
    syms = read_symtab (abfd);

    frame = translate_addresses (abfd, addr, naddr);

    free (syms);
    syms = NULL;
  }

  return frame;
}

#endif
