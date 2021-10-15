#include "idt.h"
#include "../String/string.h"
#include "../IO/io.h"
#include "../Memory/pmm/pmm.h"
#include "./irq/irq.h"
#include "../Memory/vmm/paging.h"

idt_t currentIDT;

extern void idt_flush(idt_t *idt);

ideEntry_t idtEntries[256];

void createIdtGate(int vecNum, uint64_t offset, uint8_t selector, uint8_t attr)
{

    idtEntries[vecNum].attributes = attr;

    idtEntries[vecNum].offset0_15 = (uint16_t)(offset & 0xFFFF);
    idtEntries[vecNum].offset16_31 = (uint16_t)((offset & 0xFFFF0000) >> 16);
    idtEntries[vecNum].offset32_63 = (uint32_t)((offset & 0xFFFFFFFF00000000) >> 32);

    idtEntries[vecNum].selector = 0x08;

    idtEntries[vecNum].zero = 0;
    idtEntries[vecNum].zero1 = 0;
}

void init_pic()
{
    unsigned char a1, a2;
    a1 = inb(PIC1_DATA);
    io_wait();
    a2 = inb(PIC2_DATA);
    io_wait();

    outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
    io_wait();
    outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
    io_wait();

    outb(PIC1_DATA, 0x20);
    io_wait();
    outb(PIC2_DATA, 0x28);
    io_wait();

    outb(PIC1_DATA, 4);
    io_wait();
    outb(PIC2_DATA, 2);
    io_wait();

    outb(PIC1_DATA, ICW4_8086);
    io_wait();
    outb(PIC2_DATA, ICW4_8086);
    io_wait();

    outb(PIC1_DATA, a1);
    io_wait();
    outb(PIC2_DATA, a2);
}

void initIDT()
{

    currentIDT.size = (16 * 256) - 1;
    currentIDT.baddr = idtEntries;

    memset(idtEntries, 0, 256 * 16);

    createIdtGate(0, (uint64_t)&isr0, 0x08, 0x8E);
    createIdtGate(1, (uint64_t)&isr1, 0x08, 0x8E);
    createIdtGate(2, (uint64_t)&isr2, 0x08, 0x8E);
    createIdtGate(3, (uint64_t)&isr3, 0x08, 0x8E);
    createIdtGate(4, (uint64_t)&isr4, 0x08, 0x8E);
    createIdtGate(5, (uint64_t)&isr5, 0x08, 0x8E);
    createIdtGate(6, (uint64_t)&isr6, 0x08, 0x8E);
    createIdtGate(7, (uint64_t)&isr7, 0x08, 0x8E);
    createIdtGate(8, (uint64_t)&isr8, 0x08, 0x8E);
    createIdtGate(9, (uint64_t)&isr9, 0x08, 0x8E);
    createIdtGate(10, (uint64_t)&isr10, 0x08, 0x8E);
    createIdtGate(11, (uint64_t)&isr11, 0x08, 0x8E);
    createIdtGate(12, (uint64_t)&isr12, 0x08, 0x8E);
    createIdtGate(13, (uint64_t)&isr13, 0x08, 0x8E);
    createIdtGate(14, (uint64_t)&isr14, 0x08, 0x8E);
    createIdtGate(15, (uint64_t)&isr14, 0x08, 0x8E);
    createIdtGate(16, (uint64_t)&isr16, 0x08, 0x8E);
    createIdtGate(17, (uint64_t)&isr17, 0x08, 0x8E);
    createIdtGate(18, (uint64_t)&isr18, 0x08, 0x8E);
    createIdtGate(19, (uint64_t)&isr19, 0x08, 0x8E);
    createIdtGate(20, (uint64_t)&isr20, 0x08, 0x8E);
    createIdtGate(21, (uint64_t)&isr21, 0x08, 0x8E);
    createIdtGate(22, (uint64_t)&isr22, 0x08, 0x8E);
    createIdtGate(23, (uint64_t)&isr23, 0x08, 0x8E);
    createIdtGate(24, (uint64_t)&isr24, 0x08, 0x8E);
    createIdtGate(25, (uint64_t)&isr25, 0x08, 0x8E);
    createIdtGate(26, (uint64_t)&isr26, 0x08, 0x8E);
    createIdtGate(27, (uint64_t)&isr27, 0x08, 0x8E);
    createIdtGate(28, (uint64_t)&isr28, 0x08, 0x8E);
    createIdtGate(29, (uint64_t)&isr29, 0x08, 0x8E);
    createIdtGate(30, (uint64_t)&isr30, 0x08, 0x8E);
    createIdtGate(31, (uint64_t)&isr31, 0x08, 0x8E);

    createIdtGate(32, (uint64_t)&irq0, 0x08, 0x8E);
    createIdtGate(33, (uint64_t)&irq1, 0x08, 0x8E);
    createIdtGate(34, (uint64_t)&irq2, 0x08, 0x8E);
    createIdtGate(35, (uint64_t)&irq3, 0x08, 0x8E);
    createIdtGate(36, (uint64_t)&irq4, 0x08, 0x8E);
    createIdtGate(37, (uint64_t)&irq5, 0x08, 0x8E);
    createIdtGate(38, (uint64_t)&irq6, 0x08, 0x8E);
    createIdtGate(39, (uint64_t)&irq7, 0x08, 0x8E);
    createIdtGate(40, (uint64_t)&irq8, 0x08, 0x8E);
    createIdtGate(41, (uint64_t)&irq9, 0x08, 0x8E);
    createIdtGate(42, (uint64_t)&irq10, 0x08, 0x8E);
    createIdtGate(43, (uint64_t)&irq11, 0x08, 0x8E);
    createIdtGate(44, (uint64_t)&irq12, 0x08, 0x8E);
    createIdtGate(45, (uint64_t)&irq13, 0x08, 0x8E);
    createIdtGate(46, (uint64_t)&irq14, 0x08, 0x8E);
    createIdtGate(47, (uint64_t)&irq15, 0x08, 0x8E);

    init_pic();

    idt_flush(&currentIDT);

    outb(PIC1_DATA, 0);
    outb(PIC2_DATA, 0);

    //int d = 5 / 0;
    asm volatile("sti");
}
