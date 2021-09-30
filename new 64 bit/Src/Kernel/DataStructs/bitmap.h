#ifndef _BITMAP_H_
#define _BITMAP_H_

#include "../Types/types.h"
#include "../String/string.h"

typedef struct
{
    uint64_t size;
    uint64_t *buffer;

} bitmap_t;

int bitmapGet(bitmap_t bitmap, unsigned long index);
void bitmapSet(bitmap_t bitmap, unsigned long index, int value);
uint64_t bitmapFirstZeriBit(bitmap_t bitmap);

#endif
