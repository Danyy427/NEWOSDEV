
#include "memory.h"

uint16_t memoryMapFirstEntry;

void initMemoryMap(uint16_t firstEntry)
{
    memoryMapFirstEntry = firstEntry;
}

uint64_t getSystemMemory()
{
    uint64_t total = 0;
    uint32_t count = *(uint32_t *)(memoryMapFirstEntry - 4);
    for (uint64_t i = 0; i < count; i++)
    {
        memoryEntry_t *entry = (memoryEntry_t *)(memoryMapFirstEntry + (sizeof(memoryEntry_t) * i));
        total += entry->length;
    }

    return total;
}

uint64_t getUsableSystemMemory()
{
    uint64_t total = 0;
    uint32_t count = *(uint32_t *)(memoryMapFirstEntry - 4);
    for (uint64_t i = 0; i < count; i++)
    {
        memoryEntry_t *entry = (memoryEntry_t *)(memoryMapFirstEntry + (sizeof(memoryEntry_t) * i));
        if (entry->type != 2 && entry->type != 4 && entry->type != 5 && entry->acpi3 != 0 && entry->acpi3 != 2)
            total += entry->length;
    }

    return total;
}

uint32_t isMemoryEntryUsable(memoryEntry_t *entry)
{
    return (entry->type != 2 && entry->type != 4 && entry->type != 5 && entry->acpi3 != 0 && entry->acpi3 != 2) ? 1 : 0;
}
