#include <iostream>
#include <chrono>
#include <ctime>
#include <unistd.h>

using namespace std;

void system_clock_test_func()
{
    auto now = chrono::system_clock::now();
    time_t time = chrono::system_clock::to_time_t(now);
    cout << "now: " << ctime(&time) << endl;
}

void steady_clock_test_func()
{
    auto start = chrono::steady_clock::now();
    sleep(1);
    auto end = chrono::steady_clock::now();

    auto diff = end - start;
    auto sec = chrono::duration_cast<chrono::seconds>(diff);
    auto ms = chrono::duration_cast<chrono::milliseconds>(diff);

    cout << "duration: " << sec.count() << " s" << endl;
    cout << "duration: " << ms.count() << " ms" << endl;
    cout << "duration: " << diff.count() << " us" << endl;
}

#define TIMERSTART(tag)  auto tag##_start = std::chrono::steady_clock::now(),tag##_end = tag##_start
#define TIMEREND(tag)  tag##_end =  std::chrono::steady_clock::now()
#define DURATION_s(tag) printf("%s costs %d s\n",#tag,std::chrono::duration_cast<std::chrono::seconds>(tag##_end - tag##_start).count())
#define DURATION_ms(tag) printf("%s costs %d ms\n",#tag,std::chrono::duration_cast<std::chrono::milliseconds>(tag##_end - tag##_start).count());
#define DURATION_us(tag) printf("%s costs %d us\n",#tag,std::chrono::duration_cast<std::chrono::microseconds>(tag##_end - tag##_start).count());
#define DURATION_ns(tag) printf("%s costs %d ns\n",#tag,std::chrono::duration_cast<std::chrono::nanoseconds>(tag##_end - tag##_start).count());

int macro_test()
{
    TIMERSTART(for_loop);
    sleep(1);
    TIMEREND(for_loop);

    DURATION_ms(for_loop);
}

int main()
{
    system_clock_test_func();
    steady_clock_test_func();
    macro_test();
    return 0;
}