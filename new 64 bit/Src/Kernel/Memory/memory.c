
#include "memory.h"

uint64_t getSystemMemory(uint16_t firstEntry)
{
    uint64_t total = 0;
    uint32_t count = *(uint32_t *)(firstEntry - 4);
    for (uint64_t i = 0; i < count; i++)
    {
        memoryEntry_t *entry = (memoryEntry_t *)(firstEntry + (sizeof(memoryEntry_t) * i));
        total += entry->length;
    }

    return total;
}
