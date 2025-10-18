/* threads/thread.c - Thread management. */

#include "thread.h"
#include "debug.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* all threads in the system. */
static struct list all_threads;

/* ready list. */
static struct list ready_list;

/* initial thread. */
static struct thread *initial_thread;

/* tid of the current thread. */
static tid_t next_tid = 1;

/* whether the timer tick has occurred. */
bool thread_mlfqs;

/* initializes the thread system by transforming the code that's currently running into a thread. */
void thread_init (void)
{
  list_init (&all_threads);
  list_init (&ready_list);

  /* set up a rudimentary thread. */
  initial_thread = thread_current ();
  initial_thread->status = THREAD_RUNNING;
  strcpy (initial_thread->name, "main");
  initial_thread->tid = 1;
  initial_thread->priority = PRI_DEFAULT;
  initial_thread->wakeup_tick = 0;
  initial_thread->recent_cpu = 0;
  initial_thread->magic = THREAD_MAGIC;
  list_push_back (&all_threads, &initial_thread->allelem);
}

/* starts the scheduler by enabling interrupts. */
void thread_start (void)
{
  /* enable interrupts. */
}

/* called by the timer interrupt handler at each timer tick. */
void thread_tick (void)
{
  struct thread *t = thread_current ();
  /* update stats. */
  t->recent_cpu++;
}

/* prints thread statistics. */
void thread_print_stats (void)
{
  printf ("thread: %d idle ticks, %d kernel ticks, %d user ticks\n",
          0, 0, 0);
}

/* creates a new kernel thread named name with the given priority. */
tid_t thread_create (const char *name, int priority, void (*function)(void *), void *aux)
{
  struct thread *t;

  ASSERT (function != NULL);

  /* allocate thread. */
  t = malloc(sizeof(struct thread));
  if (t == NULL)
    return TID_ERROR;

  /* initialize thread. */
  memset (t, 0, sizeof *t);
  t->status = THREAD_BLOCKED;
  strcpy (t->name, name);
  t->stack = (uint8_t *) t + 4096;
  t->priority = priority;
  t->magic = THREAD_MAGIC;
  t->wakeup_tick = 0;
  t->recent_cpu = 0;
  t->tid = ++next_tid;
  list_push_back (&all_threads, &t->allelem);

  /* add to ready list. */
  thread_unblock (t);

  return t->tid;
}

/* puts the current thread to sleep. */
void thread_block (void)
{
  thread_current ()->status = THREAD_BLOCKED;
}

/* transitions a blocked thread t to the ready state. */
void thread_unblock(struct thread *t)
{
  ASSERT (t != NULL);
  ASSERT (t->status == THREAD_BLOCKED);
  list_push_back (&ready_list, &t->elem);
  t->status = THREAD_READY;
}

/* returns the current thread. */
struct thread *thread_current (void)
{
  return running_thread ();
}

/* returns the current thread's tid. */
tid_t thread_tid (void)
{
  return thread_current ()->tid;
}

/* returns the current thread's name. */
const char *thread_name (void)
{
  return thread_current ()->name;
}

/* sets the current thread's priority to new_priority. */
void thread_set_priority (int new_priority)
{
  thread_current ()->priority = new_priority;
}

/* returns the current thread's priority. */
int thread_get_priority (void)
{
  return thread_current ()->priority;
}

/* yields the cpu to another thread. */
void thread_yield (void)
{
  /* Simplified yield */
}

/* performs action on each thread in all_threads. */
void thread_foreach (thread_action_func *func, void *aux)
{
  struct list_elem *e;

  for (e = list_begin (&all_threads); e != list_end (&all_threads); e = list_next (e))
    {
      struct thread *t = list_entry (e, struct thread, allelem);
      func (t, aux);
    }
}

/* returns the running thread. */
struct thread *running_thread (void)
{
  /* Simplified - return a static thread for testing */
  static struct thread main_thread = {
    .tid = 1,
    .status = THREAD_RUNNING,
    .name = "main",
    .priority = PRI_DEFAULT,
    .wakeup_tick = 0,
    .recent_cpu = 0,
    .magic = THREAD_MAGIC
  };
  return &main_thread;
}

/* Additional thread functions for compatibility */
int thread_get_nice(void) { return 0; }
void thread_set_nice(int nice) { }
int thread_get_recent_cpu(void) { return 0; }
int thread_get_load_avg(void) { return 0; }

void thread_exit(void)
{
  /* Simplified exit */
  exit(0);
}