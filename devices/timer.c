#include "timer.h"
#include <debug.h>
#include <inttypes.h>
#include <round.h>
#include <stdio.h>
#include "../threads/interrupt.h"
#include "../threads/synch.h"
#include "../threads/thread.h"

static int64_t ticks;
static struct list sleep_list;
static struct lock sleep_list_lock;

static void timer_interrupt (struct intr_frame *args UNUSED);

void timer_init (void)
{
  intr_register_int (0x20, timer_interrupt, "timer", TIMER_IRQ_PRIORITY);
  lock_init (&sleep_list_lock);
  list_init (&sleep_list);
}

/* it returns here the number of timer ticks */
int64_t timer_ticks (void)
{
  enum intr_level old_level = intr_disable ();
  int64_t t = ticks;
  intr_set_level (old_level);
  return t;
}

int64_t timer_elapsed (int64_t start_tick)
{
  return timer_ticks () - start_tick;
}

void timer_sleep (int64_t ticks)
{
  int64_t start_ticks = timer_ticks ();
  struct thread *current_thread = thread_current ();
  enum intr_level old_level;

  ASSERT (intr_get_level () == INTR_ON);
  if (ticks <= 0) return; /* no need to sleep for non-positive ticks. */

  old_level = intr_disable ();

  current_thread->wakeup_tick = start_ticks + ticks;
  lock_acquire (&sleep_list_lock);
  
  /* insert thread into sleep list sorted by wakeup time */
  struct list_elem *e;
  for (e = list_begin (&sleep_list); e != list_end (&sleep_list); e = list_next (e))
    {
      struct thread *t = list_entry (e, struct thread, elem);
      if (t->wakeup_tick > current_thread->wakeup_tick)
        break;
    }
  list_insert (e, &current_thread->elem);
  
  lock_release (&sleep_list_lock);
  thread_block ();

  intr_set_level (old_level);
}

static void thread_wakeup (void)
{
  struct list_elem *e;
  struct thread *t;
  int64_t current_ticks = timer_ticks ();

  lock_acquire (&sleep_list_lock);

  /* since list is sorted by wakeup time, we can stop at first thread not ready to wake */
  e = list_begin (&sleep_list);
  while (e != list_end (&sleep_list))
    {
      t = list_entry (e, struct thread, elem);
      if (t->wakeup_tick <= current_ticks)
        {
          e = list_remove (e);
          thread_unblock (t);
        }
      else
        {
          /* since list is sorted, no more threads need to be woken */
          break;
        }
    }

  lock_release (&sleep_list_lock);
}

void timer_mdelay (int64_t msec)
{
  int64_t ticks = msec * TIMER_FREQ / 1000;
  timer_sleep (ticks);
}

void timer_udelay (int64_t usec)
{
  int64_t ticks = usec * TIMER_FREQ / 1000000;
  timer_sleep (ticks);
}

void timer_ndelay (int64_t nsec)
{
  int64_t ticks = nsec * TIMER_FREQ / 1000000000;
  timer_sleep (ticks);
}

void timer_print_stats (void)
{
  printf ("Timer: %"PRId64" ticks\n", timer_ticks ());
}

static void timer_interrupt (struct intr_frame *args UNUSED)
{
  ticks++;
  thread_wakeup (); /* check for sleeping threads to wake up. */
  thread_yield ();
}