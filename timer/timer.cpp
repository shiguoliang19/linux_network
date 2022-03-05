#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

void timer_notify_funcation(union sigval v) {
    printf("hello world\n");
}

void start_timer(timer_t& timer_id) {
    struct sigevent sevp;
    struct itimerspec it;

    bzero(&sevp, sizeof(struct sigevent));
    sevp.sigev_value.sival_int = 10;
    sevp.sigev_notify = SIGEV_THREAD;
    sevp.sigev_notify_function = timer_notify_funcation;

    int err = timer_create(CLOCK_REALTIME, &sevp, &timer_id);
    if(err < 0) {
        printf("timer_create exit code: %d\n", err);
        return;
    }

    bzero(&it, sizeof(struct itimerspec));
    it.it_interval.tv_sec = 1;
    it.it_interval.tv_nsec = 0;
    it.it_value.tv_sec = 3;
    it.it_value.tv_nsec = 0;

    int err2 = timer_settime(timer_id, 0, &it, NULL);
    if(err2 < 0) {
        printf("timer_settime exit code: %d\n", err2);
        return;
    }
}

void stop_timer(timer_t& timer_id) {
    int err = timer_delete(timer_id);
    if(err < 0) {
        printf("timer_delete exit code: %d\n", err);
        return;
    }
}

int main() {
    timer_t timer_id;

    start_timer(timer_id);

    int number = 0;
    while (++number) {
        sleep(1);

        if(number > 10) {
            stop_timer(timer_id);
            break;
        }
    }
    
    return 1;
}