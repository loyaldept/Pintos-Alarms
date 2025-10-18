#include "../../threads/thread.h"
#include "../../devices/timer.h"
#include <stdio.h>

void test_alarm_negative(void) {
    int64_t start = timer_ticks();
    timer_sleep(-5);  // should not sleep
    int64_t end = timer_ticks();
    if (end - start == 0) {
        printf("alarm-negative: PASS\n");
    } else {
        printf("alarm-negative: FAIL\n");
    }
}

int main() {
    timer_init();
    test_alarm_negative();
    return 0;
}