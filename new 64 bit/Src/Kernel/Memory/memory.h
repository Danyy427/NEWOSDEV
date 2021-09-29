#ifndef _MEMORY_H_
#define _MEMORY_H_

#include "../Types/types.h"

typedef struct
{
    uint64_t addr;
    uint64_t length;
    uint32_t type;
    uint32_t acpi3;
} __attribute__((packed)) memoryEntry_t;

typedef struct
{
    uint32_t length;
    memoryEntry_t *entries;
} __attribute__((packed)) memoryMap_t;

uint64_t getSystemMemory(uint16_t memoryMap);

#endif
