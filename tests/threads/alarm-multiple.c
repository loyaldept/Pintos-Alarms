#include "../../threads/thread.h"
#include "../../devices/timer.h"
#include <stdio.h>

void test_alarm_multiple(void) {
    int64_t start = timer_ticks();
    timer_sleep(3);
    timer_sleep(5);
    int64_t end = timer_ticks();
    if (end - start >= 8) {
        printf("alarm-multiple: PASS\n");
    } else {
        printf("alarm-multiple: FAIL\n");
    }
}

int main() {
    timer_init();
    test_alarm_multiple();
    return 0;
}