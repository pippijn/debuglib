/* Copyright © 2010 Pippijn van Steenhoven
 * See COPYING.AGPL for licence information.
 */
#pragma once

#include "common.h"

#include <signal.h>

BEGIN_DECLS

/*
 * Sets up signal handlers for our debugging aid with backtrace
 * and sigaction coolness.
 */
extern VISIBLE void init_signals ();
/*
 * Resets the signal handlers to SIG_DFL.
 * XXX: init_signals does not save the old signal handlers. Maybe it should.
 */
extern VISIBLE void uninit_signals ();

extern VISIBLE void print_siginfo (siginfo_t* si, int pid);

/* Controls whether a caught signal should spawn gdb and attach it to our PID. */
extern VISIBLE bool spawn_gdb;

END_DECLS
