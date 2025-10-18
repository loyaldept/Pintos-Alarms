#ifndef DEVICES_TIMER_H
#define DEVICES_TIMER_H

#include <stdint.h>

/* number of timer interrupts per second. */
#define TIMER_FREQ 100

void timer_init(void);
int64_t timer_ticks(void);
int64_t timer_elapsed(int64_t start_tick);
void timer_sleep(int64_t ticks);
void timer_print_stats(void);

#endif /* DEVICES_TIMER_H */
