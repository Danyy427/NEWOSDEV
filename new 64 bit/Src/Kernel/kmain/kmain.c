#include "kmain.h"

int kmain(kernelInfoStructure_t *infostruct)
{
    char *str = "Hello World";
    char *str1 = "Hello World";
    int n = strcmp(str1, str);
    int *ww = (int *)0x1f000;
    *ww = n;
    while (1)
        ;
    return 0;
}