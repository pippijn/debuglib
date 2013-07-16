#pragma once

#include "common.h"

#include <signal.h>

BEGIN_DECLS

#ifdef __i386__
typedef long stack_word_t;
#elif defined (__x86_64__)
typedef unsigned long stack_word_t;
#endif

extern VISIBLE stack_word_t stack_end;

VISIBLE bool fork_trace ();
VISIBLE void fork_abort (char const *msg);

END_DECLS

#ifdef __i386__
# define init_debug()    \
   __asm__ ("movq %%rbp, %0" : "=a" (stack_end))
#elif defined (__x86_64__)
# define init_debug()    \
   __asm__ ("movl %%ebp, %0" : "=a" (stack_end))
#endif
