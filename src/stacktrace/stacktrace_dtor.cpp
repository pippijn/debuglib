/* Copyright © 2010 Pippijn van Steenhoven
 * See COPYING.AGPL for licence information.
 */
#include "xassert.h"

#include "stacktrace.h"

stacktrace::~stacktrace ()
{
#if HAVE_BFD_H
  xassert (pthread_mutex_destroy (&mtx) == 0);
#endif
}
