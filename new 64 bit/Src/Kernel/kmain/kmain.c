#include "kmain.h"

int kmain(kernelInfoStructure_t *infostruct)
{
	char * str1 = "Hello world";
	char * str2 = "world";
	char * sss = strcat(str1, str2);
	unsigned char * ww = 0x1f000;
	*ww = *(sss+11);
    while (1)
        ;
    return 0;
}