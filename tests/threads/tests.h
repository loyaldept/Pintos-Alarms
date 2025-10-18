#ifndef TESTS_THREADS_TESTS_H
#define TESTS_THREADS_TESTS_H

#include <debug.h>
#include <stdio.h>

/* Test framework functions */
void test_name (const char *name);
void fail (const char *format, ...);
void pass (void);

/* Test functions */
void test_alarm_single (void);
void test_alarm_multiple (void);
void test_alarm_negative (void);
void test_alarm_zero (void);
void test_alarm_busy (void);

#endif /* TESTS_THREADS_TESTS_H */

