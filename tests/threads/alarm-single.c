#include "../../threads/thread.h"
#include "../../devices/timer.h"
#include <stdio.h>

void test_alarm_single(void) {
    int64_t start = timer_ticks();
    timer_sleep(5);  // sleep 5 ticks
    int64_t end = timer_ticks();
    if (end - start >= 5) {
        printf("alarm-single: PASS\n");
    } else {
        printf("alarm-single: FAIL\n");
    }
}

int main() {
    timer_init();
    test_alarm_single();
    return 0;
}