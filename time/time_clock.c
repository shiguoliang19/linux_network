
#include <stdio.h>
#include <time.h>

void clock_test_func()
{
    clock_t start, end;
    double delta;

    start = clock();
    printf("Hello world!\n");
    end = clock();
    delta = (double)(end - start) / CLOCKS_PER_SEC;

    printf("CPU 占用时间：%f s\n", delta);
}

int main()
{
    clock_test_func();
    return 0;
}