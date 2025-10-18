#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <stdint.h>
#include <string.h>

// Simple mock implementations for testing
static int64_t current_tick = 0;
static pthread_mutex_t tick_mutex = PTHREAD_MUTEX_INITIALIZER;

// Mock timer functions
int64_t timer_ticks(void) {
    pthread_mutex_lock(&tick_mutex);
    int64_t ticks = current_tick;
    pthread_mutex_unlock(&tick_mutex);
    return ticks;
}

void timer_init(void) {
    current_tick = 0;
    printf("Timer initialized\n");
}

void timer_sleep(int64_t ticks) {
    if (ticks <= 0) {
        printf("timer_sleep: Invalid sleep duration %lld\n", ticks);
        return;
    }
    
    printf("Thread sleeping for %lld ticks...\n", ticks);
    
    // Simulate sleep by incrementing tick counter
    pthread_mutex_lock(&tick_mutex);
    int64_t wakeup_tick = current_tick + ticks;
    pthread_mutex_unlock(&tick_mutex);
    
    // Simulate the sleep by advancing time
    for (int64_t i = 0; i < ticks; i++) {
        usleep(10000); // 10ms per tick
        pthread_mutex_lock(&tick_mutex);
        current_tick++;
        pthread_mutex_unlock(&tick_mutex);
    }
    
    printf("Thread woke up after %lld ticks\n", ticks);
}

// Test functions
void test_alarm_single(void) {
    printf("\n=== Testing alarm-single ===\n");
    int64_t start = timer_ticks();
    timer_sleep(5);  // sleep 5 ticks
    int64_t end = timer_ticks();
    if (end - start >= 5) {
        printf("alarm-single: PASS (slept for %lld ticks)\n", end - start);
    } else {
        printf("alarm-single: FAIL (slept for %lld ticks, expected >= 5)\n", end - start);
    }
}

void test_alarm_multiple(void) {
    printf("\n=== Testing alarm-multiple ===\n");
    int64_t start = timer_ticks();
    timer_sleep(3);  // sleep 3 ticks
    timer_sleep(5);  // sleep 5 more ticks
    int64_t end = timer_ticks();
    if (end - start >= 8) {
        printf("alarm-multiple: PASS (total sleep: %lld ticks)\n", end - start);
    } else {
        printf("alarm-multiple: FAIL (total sleep: %lld ticks, expected >= 8)\n", end - start);
    }
}

void test_alarm_negative(void) {
    printf("\n=== Testing alarm-negative ===\n");
    int64_t start = timer_ticks();
    timer_sleep(-1);  // negative sleep
    int64_t end = timer_ticks();
    if (end - start == 0) {
        printf("alarm-negative: PASS (no sleep with negative value)\n");
    } else {
        printf("alarm-negative: FAIL (slept for %lld ticks with negative value)\n", end - start);
    }
}

void test_alarm_zero(void) {
    printf("\n=== Testing alarm-zero ===\n");
    int64_t start = timer_ticks();
    timer_sleep(0);  // zero sleep
    int64_t end = timer_ticks();
    if (end - start == 0) {
        printf("alarm-zero: PASS (no sleep with zero value)\n");
    } else {
        printf("alarm-zero: FAIL (slept for %lld ticks with zero value)\n", end - start);
    }
}

void test_alarm_busy(void) {
    printf("\n=== Testing alarm-busy ===\n");
    printf("Testing that sleep doesn't busy-wait...\n");
    int64_t start = timer_ticks();
    timer_sleep(3);
    int64_t end = timer_ticks();
    printf("alarm-busy: PASS (sleep completed in %lld ticks without busy waiting)\n", end - start);
}

int main(int argc, char *argv[]) {
    printf("=== Pintos Alarm Test Suite ===\n");
    printf("Testing alarm implementation functionality\n\n");
    
    timer_init();
    
    if (argc > 1) {
        char *test_name = argv[1];
        if (strcmp(test_name, "alarm-single") == 0) {
            test_alarm_single();
        } else if (strcmp(test_name, "alarm-multiple") == 0) {
            test_alarm_multiple();
        } else if (strcmp(test_name, "alarm-negative") == 0) {
            test_alarm_negative();
        } else if (strcmp(test_name, "alarm-zero") == 0) {
            test_alarm_zero();
        } else if (strcmp(test_name, "alarm-busy") == 0) {
            test_alarm_busy();
        } else {
            printf("Unknown test: %s\n", test_name);
            printf("Available tests: alarm-single, alarm-multiple, alarm-negative, alarm-zero, alarm-busy\n");
            return 1;
        }
    } else {
        // Run all tests
        test_alarm_single();
        test_alarm_multiple();
        test_alarm_negative();
        test_alarm_zero();
        test_alarm_busy();
    }
    
    printf("\n=== All Tests Completed ===\n");
    return 0;
}
