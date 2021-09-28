#ifndef _MATH_H_
#define _MATH_H_

#include "../Types/types.h"

int ceil(double n);
int floor(double n);
int min(int x, int y);
int max(int x, int y);

double sqrt(double n);
uint64_t pow(int n, unsigned int x);
uint64_t abs(int64_t n);

double hypot(double a, double b);

#endif