#include <stdio.h>

#define NONE                 "\e[0m"
#define RED                  "\e[0;31m"
#define L_RED                "\e[1;31m"
#define GREEN                "\e[0;32m"
#define L_GREEN              "\e[1;32m"
#define BLUE                 "\e[0;34m"
 
#define printr(format, arg...) do{printf(RED format NONE,## arg);}while(0)
#define printg(format, arg...) do{printf(GREEN format NONE,## arg);}while(0)
#define printb(format, arg...) do{printf(BLUE format NONE,## arg);}while(0)
#define printk(format, arg...) do{printf(BLACK format NONE,## arg);}while(0)
	
#define printlr(format, arg...) do{printf(L_RED format NONE,## arg);}while(0)
#define printlg(format, arg...) do{printf(L_GREEN format NONE,## arg);}while(0)
#define printlb(format, arg...) do{printf(L_BLUE format NONE,## arg);}while(0)

int main()
{
	printf("普通字体\n");
	printr("红色字体\n");
	printg("绿色字体\n");
	printb("蓝色字体\n");

	return 0;
}

