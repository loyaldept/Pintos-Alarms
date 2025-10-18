/* threads/synch.h - Synchronization primitives. */

#ifndef THREADS_SYNCH_H
#define THREADS_SYNCH_H

#include <list.h>
#include <stdbool.h>

/* a counting semaphore. */
struct semaphore
  {
    unsigned value;             /* current value. */
    struct list waiters;        /* list of waiting threads. */
  };

void sema_init (struct semaphore *, unsigned value);
void sema_down (struct semaphore *);
bool sema_try_down (struct semaphore *);
void sema_up (struct semaphore *);
void sema_self_test (void);

/* a lock. */
struct lock
  {
    struct thread *holder;      /* thread holding lock (for debugging). */
    struct semaphore semaphore; /* binary semaphore controlling access. */
  };

void lock_init (struct lock *);
void lock_acquire (struct lock *);
bool lock_try_acquire (struct lock *);
void lock_release (struct lock *);
bool lock_held_by_current_thread (const struct lock *);

/* condition variable. */
struct condition
  {
    struct list waiters;        /* list of waiting threads. */
  };

void cond_init (struct condition *);
void cond_wait (struct condition *, struct lock *);
void cond_signal (struct condition *, struct lock *);
void cond_broadcast (struct condition *, struct lock *);

/* optimization barrier. */
#define barrier() __asm volatile ("" : : : "memory")

#endif /* threads/synch.h */

