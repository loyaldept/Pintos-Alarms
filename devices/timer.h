/* devices/timer.h - System timer. */

#ifndef DEVICES_TIMER_H
#define DEVICES_TIMER_H

#include <round.h>
#include <stdint.h>

/* number of timer interrupts per second. */
#define TIMER_FREQ 100

void timer_init (void);
void timer_calibrate (void);

int64_t timer_ticks (void);
int64_t timer_elapsed (int64_t);

void timer_sleep (int64_t ticks);
void timer_msleep (int64_t msec);
void timer_usleep (int64_t usec);
void timer_nsleep (int64_t nsec);

void timer_print_stats (void);

#endif /* devices/timer.h */
