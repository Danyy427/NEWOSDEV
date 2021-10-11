#ifndef _HEAP_H_
#define _HEAP_H_

#include "../../Types/types.h"

/*
Attributes byte

Bit 0            1           2 3            4 5 6 7 8
    is used      root entry  privileges    reserved


*/

typedef struct HeapEntry
{
    uint8_t attributes;
    uint32_t size;
    struct HeapEntry *next;
} __attribute__((packed)) heapEntry_t;

typedef struct
{
    heapEntry_t *startEntry;
    uint64_t free;
    uint64_t size;
} __attribute__((packed)) heap_t;

extern heap_t *currentHeap;
void initHeap(uint64_t baddr, uint64_t size);
void *kmalloc(uint64_t size);

#endif