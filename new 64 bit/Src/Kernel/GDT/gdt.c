#include "gdt.h"

extern void loadGDT(uint64_t *gdtr);

gdtEntry_t gdtentries[GDT_SIZE];

void initGDT()
{
    gdtr_t gdtr;
    gdtr.offset = gdtentries;
    gdtr.limit = sizeof(gdtEntry_t) * (GDT_SIZE + 1) - 1;

    gdtSetEntry(gdtr.offset, 0, 0, 0, 0, 0);
    gdtSetEntry(gdtr.offset, 1, 0, 0, 0x9A, 0xA0);
    gdtSetEntry(gdtr.offset, 2, 0, 0, 0x92, 0xA0);
    gdtSetEntry(gdtr.offset, 3, 0, 0, 0xF2, 0xA0);
    gdtSetEntry(gdtr.offset, 4, 0, 0, 0xFA, 0xA0);

    loadGDT(&gdtr);
}

/* limit has to be page aligned */
void gdtSetEntry(gdtEntry_t *offset, uint8_t number, uint64_t base, uint32_t limit, uint8_t access, uint8_t granularity)
{
    offset[number].base_low = (base & 0xFFFF);
    offset[number].base_middle = (base >> 16) & 0xFF;
    offset[number].base_high = (base >> 24) & 0xFF;

    offset[number].limit_low = (limit & 0xFFFF);
    offset[number].limit_high = (limit >> 16) & 0x0F;

    offset[number].limit_high |= granularity & 0xF0;
    offset[number].access = access;
}