#ifndef _PAGING_H_
#define _PAGING_H_

#include "../../Types/types.h"

extern void enablePaging(uint64_t *);

typedef struct
{
    uint64_t present : 1;
    uint64_t rw : 1;
    uint64_t privilege : 1;
    uint64_t pwt : 1;
    uint64_t pcd : 1;
    uint64_t accessed : 1;
    uint64_t dirty : 1;
    uint64_t type : 1;
    uint64_t global : 1;
    uint64_t ignored0 : 3;
    uint64_t address : 28;
    uint64_t reserved0 : 12;
    uint64_t ignored1 : 7;
    uint64_t protection : 4;
    uint64_t execdisable : 1;
} __attribute__((packed)) pageTableEntry_t;

typedef struct
{
    pageTableEntry_t entries[512];
} __attribute__((packed)) pageTable_t;

typedef struct
{
    uint64_t present : 1;
    uint64_t rw : 1;
    uint64_t privilege : 1;
    uint64_t pwt : 1;
    uint64_t pcd : 1;
    uint64_t accessed : 1;
    uint64_t ignored0 : 1;
    uint64_t zero : 1;
    uint64_t ignored1 : 4;
    uint64_t address : 28;
    uint64_t reserved0 : 12;
    uint64_t ignored2 : 11;
    uint64_t execdisable : 1;
} __attribute__((packed)) pageDirectoryEntry_t;

typedef struct
{
    pageDirectoryEntry_t entries[512];
} __attribute__((packed)) pageDirectory_t;

typedef struct
{
    uint64_t present : 1;
    uint64_t rw : 1;
    uint64_t privilege : 1;
    uint64_t pwt : 1;
    uint64_t pcd : 1;
    uint64_t accessed : 1;
    uint64_t ignored0 : 1;
    uint64_t zero : 1;
    uint64_t ignored1 : 4;
    uint64_t address : 28;
    uint64_t reserved0 : 12;
    uint64_t ignored2 : 11;
    uint64_t execdisable : 1;
} __attribute__((packed)) pageDirectoryPointerTableEntry_t;

typedef struct
{
    pageDirectoryPointerTableEntry_t entries[512];
} __attribute__((packed)) pageDirectoryPointerTable_t;

typedef struct
{
    uint64_t present : 1;
    uint64_t rw : 1;
    uint64_t privilege : 1;
    uint64_t pwt : 1;
    uint64_t pcd : 1;
    uint64_t accessed : 1;
    uint64_t ignored0 : 1;
    uint64_t zero : 1;
    uint64_t ignored1 : 4;
    uint64_t address : 28;
    uint64_t reserved0 : 12;
    uint64_t ignored2 : 11;
    uint64_t execdisable : 1;
} __attribute__((packed)) PML4Entry_t;

typedef struct
{
    PML4Entry_t entries[512];
} __attribute__((packed)) PML4_t;

extern PML4_t *currentPML4;

void initPaging();
void mapPhysicalToVirtualAddr(PML4_t *pml4, uint64_t paddr, uint64_t vaddr, uint32_t flags);
uint64_t getPhysicalAddress(PML4_t *pml4, uint64_t vaddr);
#endif