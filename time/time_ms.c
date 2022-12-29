#include <stdio.h>
#include <sys/time.h>

void gettimeofday_test_func()
{
    struct timeval start, end;
    double delta;

    gettimeofday(&start, NULL);
    printf("Hello world!\n");
    gettimeofday(&end, NULL);
    delta = (end.tv_sec + end.tv_usec / 1000000.0) - (start.tv_sec + start.tv_usec / 1000000.0);

    printf("start sec: %ld usec %ld\n", start.tv_sec, start.tv_usec);
    printf("end sec: %ld usec %ld\n", end.tv_sec, end.tv_usec);
    printf("run time: %f s\n", delta);
}

int main()
{
    gettimeofday_test_func();
    return 0;
}
