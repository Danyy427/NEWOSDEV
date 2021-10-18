#include "pmm.h"
#include "../memory.h"
#include "../../kmain/kmain.h"

bitmap_t physicalPageBitmap;
uint32_t isPhysicalPageFrameAllocationInitialized;

void initPhysicalPageFrameAllocation(uint64_t kernelSize)
{
    physicalPageBitmap.buffer = 0x10000 + (kernelSize * 512) + 0x100;
    uint64_t count = getSystemMemory();
    count /= 0x1000;
    count /= 64;
    count += 1;
    physicalPageBitmap.size = count;

    memset(physicalPageBitmap.buffer, 0, physicalPageBitmap.size);

    isPhysicalPageFrameAllocationInitialized = 1;

    reservePages(0, getSystemMemory() / 0x1000 + 1);

    for (uint64_t i = 0; i < *(uint32_t *)(memoryMapFirstEntry - 4); i++)
    {
        if (isMemoryEntryUsable(memoryMapFirstEntry))
        {
            memoryEntry_t *entry = (memoryEntry_t *)(memoryMapFirstEntry + (sizeof(memoryEntry_t) * i));
            unreservePages((void *)entry->addr, entry->length / 0x1000);
        }
    }

    reservePages(0x0000, 0x100);
}

void reservePages(uint64_t *address, uint64_t pageCount)
{
    if (isPhysicalPageFrameAllocationInitialized == 0)
        return;

    for (uint64_t i = 0; i < pageCount; i++)
    {
        uint64_t addr = (uint64_t)address + i * 0x1000;

        bitmapSet(physicalPageBitmap, addr / 0x1000, 1);
    }
}

void unreservePages(uint64_t *address, uint64_t pageCount)
{
    if (isPhysicalPageFrameAllocationInitialized == 0)
        return;

    for (uint64_t i = 0; i < pageCount; i++)
    {
        uint64_t addr = (uint64_t)address + i * 0x1000;

        bitmapSet(physicalPageBitmap, addr / 0x1000, 0);
    }
}

uint32_t isPageReserved(uint64_t pageNumber)
{
    if (isPhysicalPageFrameAllocationInitialized == 0)
        return 0;

    return bitmapGet(physicalPageBitmap, pageNumber);
}

uint64_t firstUnreservedPage()
{
    if (isPhysicalPageFrameAllocationInitialized == 0)
        return 0;

    return bitmapFirstZeriBit(physicalPageBitmap);
}

void *RequestPhysicalPage()
{
    if (isPhysicalPageFrameAllocationInitialized == 0)
        return 0;

    uint64_t index = firstUnreservedPage();

    if (index == 0xFFFFFFFFFFFFFFFF)
    {
        while (1)
            ;
    }

    reservePages((void *)(index * 0x1000), 1);
    return (void *)((index * 0x1000));
}