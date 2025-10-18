#ifndef THREADS_SYNCH_H
#define THREADS_SYNCH_H

#include <stdbool.h>
#include <list.h>

/* lock structure */
struct lock {
    struct thread *holder;      /* thread holding lock (for debugging) */
    struct list waiters;        /* list of waiting threads */
};

/* semaphore structure */
struct semaphore {
    unsigned value;             /* current value */
    struct list waiters;        /* list of waiting threads */
};

/* condition variable structure */
struct condition {
    struct list waiters;        /* list of waiting threads */
};

/* lock functions */
void lock_init(struct lock *lock);
void lock_acquire(struct lock *lock);
void lock_release(struct lock *lock);
bool lock_held_by_current_thread(const struct lock *lock);

/* semaphore functions */
void sema_init(struct semaphore *sema, unsigned value);
void sema_down(struct semaphore *sema);
bool sema_try_down(struct semaphore *sema);
void sema_up(struct semaphore *sema);

/* condition variable functions */
void cond_init(struct condition *cond);
void cond_wait(struct condition *cond, struct lock *lock);
void cond_signal(struct condition *cond, struct lock *lock);
void cond_broadcast(struct condition *cond, struct lock *lock);

#endif /* THREADS_SYNCH_H */