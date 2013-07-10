/* Copyright © 2010 Pippijn van Steenhoven
 * See COPYING.AGPL for licence information.
 */
#include "xassert.h"

#include "stacktrace.h"

#include "dlbfd.h"

stacktrace::stacktrace ()
#if HAVE_BFD_H
  : mtx ()
  , syms (0)
  , dbfd ()
  , pc (0)
  , filename ("file")
  , funcname ("func")
  , line (0)
  , found (false)
#endif
{
#if HAVE_BFD_H
  bfd_init ();
  xassert (pthread_mutex_init (&mtx, NULL) == 0);
#endif
}
