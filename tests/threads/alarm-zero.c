#include "../../threads/thread.h"
#include "../../devices/timer.h"
#include <stdio.h>

void test_alarm_zero(void) {
    int64_t start = timer_ticks();
    timer_sleep(0);  // should not sleep
    int64_t end = timer_ticks();
    if (end - start == 0) {
        printf("alarm-zero: PASS\n");
    } else {
        printf("alarm-zero: FAIL\n");
    }
}

int main() {
    timer_init();
    test_alarm_zero();
    return 0;
}