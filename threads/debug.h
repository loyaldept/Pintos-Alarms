/* threads/debug.h - Debugging utilities. */

#ifndef THREADS_DEBUG_H
#define THREADS_DEBUG_H

/* external function to print to the console. */
extern void put_debug (const char *, ...);

/* enables or disables debugging output. */
void debug_init (void);
bool debug_enabled (void);
void debug_set_enabled (bool);

/* asserts that condition is true. if false, prints message and halts. */
#define ASSERT(condition)                                                  \
  if (!(condition))                                                        \
    {                                                                      \
      put_debug ("assertion failed: %s:%d: %s\n", __FILE__, __LINE__,     \
                 #condition);                                             \
      for (;;)                                                             \
        ;                                                                  \
    }

#endif /* threads/debug.h */

