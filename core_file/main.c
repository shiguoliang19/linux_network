#include <stdio.h>

void func()
{
	char* p = NULL;
	p[0] = 'A';
}

int main()
{
    func();
	printf("Hello world!");
	return 0;
}
