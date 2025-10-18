#ifndef LIB_DEBUG_H
#define LIB_DEBUG_H

#include <stdbool.h>
#include <stdio.h>

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

#endif /* LIB_DEBUG_H */
