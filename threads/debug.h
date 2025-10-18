#ifndef THREADS_DEBUG_H
#define THREADS_DEBUG_H

#include <stdbool.h>
#include <stdio.h>
#include <stddef.h>

/* Macro for functions that never return */
#define NO_RETURN __attribute__ ((noreturn))
#define NOT_REACHED() abort()
#define UNUSED __attribute__ ((unused))

/* Debug functions */
extern void put_debug(const char *, ...);
void debug_init(void);
bool debug_enabled(void);
void debug_set_enabled(bool);

#define ASSERT(condition)                     \
  if (!(condition))                           \
  {                                           \
      put_debug("assertion failed: %s:%d: %s\n", __FILE__, __LINE__, #condition); \
      for (;;) ;                               \
  }

#endif /* THREADS_DEBUG_H */
