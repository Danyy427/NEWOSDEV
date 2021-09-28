#include "math.h"
#include "../types/types.h"

int ceil(double n)
{
    return n == (double)((int)n) ? n : n + 1;
}

int floor(double n)
{
    return (int)(n);
}

int min(int x, int y)
{
    return x < y ? x : y;
}

int max(int x, int y)
{
    return x > y ? y : x;
}

int sqrt(int n)
{
    int z = 1.0;
    for (int i = 1; i <= 10; i++)
    {
        z -= (z * z - n) / (2 * z);
    }
    return z;
}

uint64_t pow(int n, unsigned int x)
{
    int temp;
    if (n == 0)
        return 0;
    if (x == 0)
        return 1;

    temp = pow(n, x / 2);
    if ((x % 2) == 0)
        return temp * temp;
    else
        return x * temp * temp;
}

uint64_t abs(int64_t n)
{
    return n < 0 ? -n : n;
}
