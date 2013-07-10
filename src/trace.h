#pragma once

#include "common.h"

#include <signal.h>

BEGIN_DECLS

#ifdef __i386__
extern VISIBLE long stack_end;
#else
extern VISIBLE unsigned long long stack_end;
#endif

VISIBLE bool fork_trace ();
VISIBLE void fork_abort (char const *msg);

END_DECLS

#if __WORDSIZE == 64
# define init_debug()    \
   asm ("movq %%rbp, %0" : "=a" (stack_end));
#else
# define init_debug()    \
   asm ("movl %%ebp, %0" : "=a" (stack_end));
#endif
