#include "paging.h"
#include "../memory.h"
#include "../pmm/pmm.h"
#include "../../kmain/kmain.h"
#include "../../DataStructs/bitmap.h"

PML4_t *currentPML4;

void initPaging()
{

    PML4_t *kernelPML4 = (PML4_t *)RequestPhysicalPage();
    setAsCurrentPML4(kernelPML4);

    uint64_t from = 0x00000, size = 0x300000, zz = 0;

    for (; size > 0; from += 4096, size -= 4096, zz++)
    {
        //first_page_table[zz] = from | 1;     // mark page present.
        mapPhysicalToVirtualAddr(kernelPML4, from, from, 0x003);
    }

    from = kernelInfo.framebuffer, size = kernelInfo.width * kernelInfo.height * kernelInfo.bpp / 8, zz = 0;

    for (; size > 0; from += 4096, size -= 4096, zz++)
    {
        //first_page_table[zz] = from | 1;     // mark page present.
        mapPhysicalToVirtualAddr(kernelPML4, from, from, 0x003);
    }

    loadCurrentPML4(kernelPML4);

    setAsCurrentPML4(kernelPML4);
}

void mapPhysicalToVirtualAddr(PML4_t *pml4, uint64_t paddr, uint64_t vaddr, uint32_t flags)
{

    // Make sure that both addresses are page-aligned.

    uint64_t pdindex;
    uint64_t ptindex;
    uint64_t pdptindex;
    uint64_t pml4index;

    vaddr >>= 12;
    ptindex = vaddr & 0x1ff;
    vaddr >>= 9;
    pdindex = vaddr & 0x1ff;
    vaddr >>= 9;
    pdptindex = vaddr & 0x1ff;
    vaddr >>= 9;
    pml4index = vaddr & 0x1ff;

    pageDirectoryPointerTable_t *pdpt;
    if (pml4->entries[pml4index].present == 0)
    {
        pdpt = RequestPhysicalPage();
        memset(pdpt, 0, 4096);
        pml4->entries[pml4index].present = 1;
        pml4->entries[pml4index].rw = 1;
        pml4->entries[pml4index].address = ((uint64_t)pdpt >> 12);
    }
    else
    {

        pdpt = (pml4->entries[pml4index].address << 12);
    }

    pageDirectory_t *directory;
    if (pdpt->entries[pdptindex].present == 0)
    {
        directory = RequestPhysicalPage();
        memset(directory, 0, 4096);
        pdpt->entries[pdptindex].present = 1;
        pdpt->entries[pdptindex].rw = 1;
        pdpt->entries[pdptindex].address = ((uint64_t)directory >> 12);
    }
    else
    {

        directory = (pdpt->entries[pdptindex].address << 12);
    }

    pageTable_t *pt;
    if (directory->entries[pdindex].present == 0)
    {
        pt = RequestPhysicalPage();
        memset(pt, 0, 4096);
        directory->entries[pdindex].present = 1;
        directory->entries[pdindex].rw = 1;
        directory->entries[pdindex].address = ((uint64_t)pt >> 12);
    }
    else
    {

        pt = (directory->entries[pdindex].address << 12);
    }

    pt->entries[ptindex].present = flags & 1;
    pt->entries[ptindex].rw = (flags & 2) >> 1;
    pt->entries[ptindex].privilege = (flags & 4) >> 2;
    pt->entries[ptindex].pwt = (flags & 8) >> 3;
    pt->entries[ptindex].pcd = (flags & 16) >> 4;      // 000
    pt->entries[ptindex].accessed = (flags & 32) >> 5; // 0000 0000 0000
    pt->entries[ptindex].dirty = (flags & 64) >> 6;
    pt->entries[ptindex].type = 0;
    pt->entries[ptindex].global = (flags & 256) >> 8;
    pt->entries[ptindex].ignored1 = 0b000;
    pt->entries[ptindex].address = ((uint64_t)paddr >> 12);

    __native_flush_tlb_single((uint64_t)pt->entries[ptindex].address << 12);
}

uint64_t getPhysicalAddress(PML4_t *pml4, uint64_t vaddr)
{
    uint64_t pdindex;
    uint64_t ptindex;
    uint64_t pdptindex;
    uint64_t pml4index;

    vaddr >>= 12;
    ptindex = vaddr & 0x1ff;
    vaddr >>= 9;
    pdindex = vaddr & 0x1ff;
    vaddr >>= 9;
    pdptindex = vaddr & 0x1ff;
    vaddr >>= 9;
    pml4index = vaddr & 0x1ff;

    pageDirectoryPointerTable_t *pdpt = (pml4->entries[pml4index].address << 12);
    pageDirectory_t *directory = (pdpt->entries[pdptindex].address << 12);
    pageTable_t *pt = (directory->entries[pdindex].address << 12);

    return (pt->entries[ptindex].address);
}

void __native_flush_tlb_single(uint64_t addr)
{
    asm volatile("invlpg (%0)" ::"r"(addr)
                 : "memory");
}

void setAsCurrentPML4(PML4_t *pml4)
{
    currentPML4 = pml4;
}
PML4_t *getCurrentPML4()
{
    return currentPML4;
}

void loadCurrentPML4(PML4_t *pml4)
{
    //while(1);
    asm volatile("mov %0, %%cr3"
                 :
                 : "r"(pml4));

    //currentPML4 = pml4;
}
