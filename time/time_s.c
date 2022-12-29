#include <stdio.h>
#include <time.h>

void time_test_func()
{
    time_t seconds;
    seconds = time(NULL);

    printf("从1970-01-01 00:00:00到现在的秒数: [%ld]\n", seconds);
}

void difftime_test_func()
{
    time_t start, end;
    double delta;

    start = time(NULL);
    printf("Hello world!\n");
    end = time(NULL);
    delta = difftime(end, start);

    printf("相差秒数 : %0.2f\n", delta);
}

void strftime_test_func()
{
    time_t seconds;
    struct tm* now;
    char buf[1024];

    seconds = time(NULL);
    now = localtime(&seconds);
    strftime(buf, 1024, "%Y-%m-%d %H:%M:%S", now);

    printf("格式化的日期 & 时间 : [%s]\n", buf);
}

int main()
{
    time_test_func();
    difftime_test_func();
    strftime_test_func();
    return 0;
}
