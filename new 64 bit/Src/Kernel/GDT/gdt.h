#ifndef _GDT_H_
#define _GDT_H_

#include "../Types/types.h"

#define GDT_SIZE 5

typedef struct
{
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t base_middle;
    uint8_t access;
    uint8_t limit_high;
    uint8_t base_high;
} __attribute__((packed)) gdtEntry_t;

typedef struct
{
    uint16_t limit;
    gdtEntry_t *offset;
} __attribute__((packed)) gdtr_t;

void initGDT();

#endif