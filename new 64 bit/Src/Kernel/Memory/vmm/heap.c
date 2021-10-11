#include "heap.h"
#include "paging.h"
#include "../../String/string.h"
#include "../pmm/pmm.h"

heap_t *currentHeap;

void initHeap(uint64_t baddr, uint64_t size)
{

    for (int i = 0; i < size; i++)
    {
        mapPhysicalToVirtualAddr(currentPML4, RequestPhysicalPage(), baddr + i * 0x1000, 0x003);
    }

    heap_t *kernelHeap = baddr;
    kernelHeap->size = size * 0x1000;
    kernelHeap->free = size * 0x1000 - sizeof(kernelHeap);

    heapEntry_t *rootEntry = baddr + sizeof(heap_t);
    rootEntry->attributes = 0x03;
    rootEntry->size = 0;
    rootEntry->next = 0;
    kernelHeap->startEntry = rootEntry;

    currentHeap = kernelHeap;
}

void expandHeap(heap_t *heap, uint32_t size)
{
    uint64_t heapLast = heap->startEntry + heap->size;
    mapPhysicalToVirtualAddr(currentPML4, RequestPhysicalPage(), heapLast, 0x03);
    heap->size += 0x1000;
}

void *kmalloc(uint64_t size)
{

    if (currentHeap == 0)
        return 0;

    for (heapEntry_t *i = currentHeap->startEntry; i; i = i->next)
    {
        if (i->attributes & 0x1 == 1 || (i->attributes & 0x1 == 0 && i->size < size)) // is the space used? yes?
        {

            if (i->next == 0x00) // we dont have a block after this?
            {

                uint64_t spaceNeeded = size + sizeof(heapEntry_t); // the total space we need
                if (currentHeap->free >= spaceNeeded)              // do we have that space yes?
                {

                    currentHeap->free -= spaceNeeded; // decrease the free space

                    heapEntry_t *newEntry = (uint64_t)i + sizeof(heapEntry_t) + i->size; // create a new entry
                    newEntry->size = size;                                               // set the new entry
                    newEntry->attributes |= 0x01;
                    memset((uint64_t)newEntry + sizeof(heapEntry_t), 0, newEntry->size);

                    i->next = newEntry; // set the next to be the next block

                    return (void *)((uint64_t)newEntry + sizeof(heapEntry_t)); // we have allocated the block its time to return
                }
                else // there is no space so lets get some
                {
                    for (int z = 0; z < ((spaceNeeded - (spaceNeeded % 4096)) / 4096); z++)
                        expandHeap(currentHeap, 1);

                    currentHeap->free -= spaceNeeded; // decrease the free space

                    heapEntry_t *newEntry = (uint64_t)i + sizeof(heapEntry_t) + i->size; // create a new entry
                    newEntry->size = size;                                               // set the new entry
                    newEntry->attributes |= 0x01;                                        // set it to be used

                    memset((uint64_t)newEntry + sizeof(heapEntry_t), 0, newEntry->size);

                    i->next = newEntry; // set the next to be the next block

                    return (void *)((uint64_t)newEntry + sizeof(heapEntry_t)); // we have allocated the block its time to return
                }

                continue;
            }
            else // we do?
            {
                // go on
                continue;
            }
        }
        else // no?
        {
            if (i->size == size) // we can use this space to store our thing if it is already equal in size
            {
                i->attributes |= 0x1;

                memset((uint64_t)i + sizeof(heapEntry_t), 0, i->size);
                return (uint64_t)i + sizeof(heapEntry_t);
            }
            else if (i->size >= size + sizeof(heapEntry_t) + 1) // if it is bigger in size that we can fit another header + 1 split that thing
            {
                heapEntry_t *newEntry = (uint64_t)i + size + sizeof(heapEntry_t);
                newEntry->attributes = 0x0;
                newEntry->size = (i->size) - size - sizeof(heapEntry_t);
                newEntry->next = i->next;
                i->next = (uint64_t)newEntry;
                i->size = size;
                i->attributes |= 0x01;

                memset((uint64_t)newEntry + sizeof(heapEntry_t), 0, newEntry->size);
                memset(i + sizeof(heapEntry_t), 0, i->size);
                return (void *)((uint64_t)newEntry + sizeof(heapEntry_t));
            }
            else // we have a space that is bigger but doesnt have enough space to include a header for split, just die
            {
                continue;
            }
        }
    }
    return 0;
}

void kfree(void *addr)
{
    if (currentHeap == 0 || addr == 0)
        return;

    heapEntry_t *before;
    int beforefull = 1, afterfull = 1;
    for (heapEntry_t *i = currentHeap->startEntry; i; i = i->next)
    {

        if (i->attributes & 0x1 == 0x1 && i + sizeof(heapEntry_t) == addr)
        {
            if (before != 0 && before->attributes & 0x1 == 0x0)
                beforefull = 0;
            if (i->next != 0 && i->next->attributes & 0x1 == 0x0)
                afterfull = 0;

            if (beforefull == 0 && afterfull == 0)
            {
                before->size += i->size + sizeof(heapEntry_t) + i->next->size + sizeof(heapEntry_t);
                before->next = i->next->next;
                memset(i, 0, i->size + sizeof(heapEntry_t) + i->next->size + sizeof(heapEntry_t));
            }
            else if (beforefull == 0)
            {
                before->size += i->size + sizeof(heapEntry_t);
                before->next = i->next;
                memset(i, 0, i->size + sizeof(heapEntry_t));
            }
            else if (afterfull == 0)
            {
                i->size += i->next->size + sizeof(heapEntry_t);
                i->next = i->next->next;
                memset(i->next, 0, i->next->size + sizeof(heapEntry_t));
            }
            else
            {
                i->attributes &= 0xE;
                memset(addr, 0, i->size);
            }
        }
        else
        {
            continue;
        }
        before = i;
    }
}