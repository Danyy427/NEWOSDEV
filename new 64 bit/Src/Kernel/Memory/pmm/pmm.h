#ifndef _PMM_H_
#define _PMM_H_

#include "../../Types/types.h"

#include "../../DataStructs/bitmap.h"

typedef struct
{
    uint64_t base_addr;
} __attribute__((packed)) physicalPageFrame;

static bitmap_t physicalPageBitmap;
static uint32_t isPhysicalPageFrameAllocationInitialized;

void initPhysicalPageFrameAllocation(uint64_t kernelSize);

void reservePages(uint64_t *address, uint64_t pageCount);

void unreservePages(uint64_t *address, uint64_t pageCount);

uint32_t isPageReserved(uint64_t pageNumber);

uint64_t firstUnreservedPage();

void *RequestPhysicalPage();

#endif