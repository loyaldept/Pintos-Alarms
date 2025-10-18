/* threads/thread.c - Thread management. */

#include "threads/thread.h"
#include <debug.h>
#include <stddef.h>
#include <random.h>
#include <stdio.h>
#include <string.h>
#include "threads/flags.h"
#include "threads/interrupt.h"
#include "threads/intr-stubs.h"
#include "threads/palloc.h"
#include "threads/switch.h"
#include "threads/synch.h"
#include "threads/vaddr.h"
#ifdef USERPROG
#include "userprog/process.h"
#endif

/* an array of 4k pages. */
#define THREAD_STACK_PAGES 1

/* all threads in the system. */
static struct list all_threads;

/* ready list. */
static struct list ready_list;

/* idle thread. */
static struct thread *idle_thread;

/* initial thread. */
static struct thread *initial_thread;

/* tid of the current thread. */
static tid_t next_tid = 1;

/* whether the timer tick has occurred. */
bool thread_mlfqs;

/* a function to compare thread priorities. */
static bool cmp_priority (const struct list_elem *a, const struct list_elem *b, void *aux UNUSED);

/* helper function for thread_create. */
static void kernel_thread (thread_func *, void *aux);

/* initializes the thread system by transforming the code that's currently running into a thread. */
void thread_init (void)
{
  ASSERT (intr_get_level () == INTR_OFF);

  list_init (&all_threads);
  list_init (&ready_list);

  /* set up a rudimentary thread. */
  initial_thread = thread_current ();
  initial_thread->status = THREAD_RUNNING;
  strcpy (initial_thread->name, "main");
  initial_thread->tid = allocate_tid ();
  initial_thread->priority = PRI_DEFAULT;
  initial_thread->wakeup_tick = 0; /* not sleeping initially. */
  list_push_back (&all_threads, &initial_thread->allelem);
}

/* starts the scheduler by enabling interrupts. */
void thread_start (void)
{
  /* enable interrupts. */
  intr_enable ();
}

/* called by the timer interrupt handler at each timer tick. */
void thread_tick (void)
{
  struct thread *t = thread_current ();

  /* update stats. */
  if (t != idle_thread)
    t->recent_cpu++;

  if (thread_mlfqs)
    thread_yield ();
}

/* prints thread statistics. */
void thread_print_stats (void)
{
  printf ("thread: %ld idle ticks, %ld kernel ticks, %ld user ticks\n",
          idle_thread->total_ticks,
          kernel_thread->total_ticks,
          user_thread->total_ticks);
}

/* creates a new kernel thread named name with the given priority. */
tid_t thread_create (const char *name, int priority, thread_func *function, void *aux)
{
  struct thread *t;
  struct intr_frame *if_;
  bool success;

  ASSERT (function != NULL);

  /* allocate thread. */
  t = palloc_get_page (PAL_ZERO);
  if (t == NULL)
    return TID_ERROR;

  /* initialize thread. */
  init_thread (t, name, priority);
  t->wakeup_tick = 0; /* not sleeping initially. */
  if_ = (struct intr_frame *) ((uint8_t *) t + PGSIZE - sizeof *if_ - sizeof (void *));
  if_->gs = if_->fs = if_->es = if_->ds = SELECTOR_KDSEG;
  if_->cs = SELECTOR_KCODE | RPL_0;
  if_->eflags = FLAG_IF | FLAG_MBS;
  if_->esp = (void *) if_;
  if_->eip = (void *) kernel_thread;
  if_->aux = aux;
  if_->func = function;

  /* add to ready list. */
  thread_unblock (t);

  return t->tid;
}

/* puts the current thread to sleep. */
void thread_block (void)
{
  ASSERT (!intr_context ());
  ASSERT (intr_get_level () == INTR_OFF);

  thread_current ()->status = THREAD_BLOCKED;
  schedule ();
}

/* transitions a blocked thread t to the ready state. */
void thread_unblock (struct thread *t)
{
  enum intr_level old_level;

  ASSERT (is_thread (t));

  old_level = intr_disable ();
  ASSERT (t->status == THREAD_BLOCKED);
  list_insert_ordered (&ready_list, &t->elem, cmp_priority, NULL);
  t->status = THREAD_READY;
  intr_set_level (old_level);
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
  thread_yield ();
}

/* returns the current thread's priority. */
int thread_get_priority (void)
{
  return thread_current ()->priority;
}

/* yields the cpu to another thread. */
void thread_yield (void)
{
  struct thread *cur = thread_current ();
  enum intr_level old_level;

  ASSERT (!intr_context ());

  old_level = intr_disable ();
  if (cur != idle_thread)
    list_insert_ordered (&ready_list, &cur->elem, cmp_priority, NULL);
  cur->status = THREAD_READY;
  schedule ();
  intr_set_level (old_level);
}

/* compares the priorities of two threads. */
static bool cmp_priority (const struct list_elem *a, const struct list_elem *b, void *aux UNUSED)
{
  return list_entry (a, struct thread, elem)->priority >
         list_entry (b, struct thread, elem)->priority;
}

/* idle thread. */
static void idle (void *idle_started_ UNUSED)
{
  for (;;)
    {
      intr_disable ();
      thread_block ();
      asm volatile ("sti; hlt" : : : "memory");
    }
}

/* initializes a thread. */
static void init_thread (struct thread *t, const char *name, int priority)
{
  ASSERT (t != NULL);
  ASSERT (priority >= PRI_MIN && priority <= PRI_MAX);
  ASSERT (name != NULL);

  memset (t, 0, sizeof *t);
  t->status = THREAD_BLOCKED;
  strlcpy (t->name, name, sizeof t->name);
  t->stack = (uint8_t *) t + PGSIZE;
  t->priority = priority;
  t->magic = THREAD_MAGIC;
  list_push_back (&all_threads, &t->allelem);
}

/* allocates a tid. */
static tid_t allocate_tid (void)
{
  static tid_t tid = 0;
  return tid++;
}

/* returns true if t appears to be a valid thread. */
static bool is_thread (struct thread *t)
{
  return t != NULL && t->magic == THREAD_MAGIC;
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

/* switch to the next thread. */
static void schedule (void)
{
  struct thread *cur = thread_current ();
  struct thread *next = next_thread_to_run ();

  if (cur != next)
    switch_threads (cur, next);
}

/* returns the next thread to be scheduled. */
static struct thread *next_thread_to_run (void)
{
  if (!list_empty (&ready_list))
    return list_entry (list_pop_front (&ready_list), struct thread, elem);
  else
    return idle_thread;
}

/* returns the running thread. */
struct thread *running_thread (void)
{
  uint32_t *esp;

  /* kernel stacks are 4kb in size. */
  asm ("mov %%esp, %0" : "=g" (esp));
  return (struct thread *) ((uint32_t) esp & ~ (PGSIZE - 1));
}

/* thread magic number. */
#define THREAD_MAGIC 0xcd6abf4b

