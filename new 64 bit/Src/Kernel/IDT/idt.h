#ifndef _IDT_H_
#define _IDT_H_

#include "../Types/types.h"

typedef struct
{
    uint16_t offset0_15;
    uint16_t selector;
    uint8_t zero;
    uint8_t attributes;
    uint16_t offset16_31;
    uint32_t offset32_63;
    uint32_t zero1;
} __attribute__((packed)) ideEntry_t;

typedef struct
{
    uint16_t size;
    uint64_t baddr;
} __attribute__((packed)) idt_t;

typedef enum
{
    TASK32 = 0x5,
    INT16 = 0x8,
    TRAP16 = 0x7,
    INT32 = 0xE,
    TRAP32 = 0xF
} __attribute__((packed)) gateTypes_t;

typedef struct
{
    uint64_t rdi, rsi, rbp, useless, rbx, rdx, rcx, rax;
    uint64_t r8, r9, r10, r11, r12, r13, r14, r15;
    uint64_t int_no, err_code;
    uint64_t rip, cs, rflags, rsp, ss;
} __attribute__((packed)) interrupt_frame_t;

extern idt_t currentIDT;

extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();

/* IRQ definitions */
extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();

void initIDT();

#endif