#include "irq.h"

#include "../../Hardware/Keyboard/keyboard.h"
#include "../../Printf/printf.h"

void default_irq_handler(interrupt_frame_t *frame)
{
    /* If IRQ # on slave PIC send EOI to slave */

    outb(0x20, 0x20);
    outb(0xa0, 0x20);
}

// PIT TIMER

uint64_t environmentTickCount;

void irq0_handler(interrupt_frame_t *frame)
{
    environmentTickCount++;
    default_irq_handler(frame);
}

void irq1_handler(interrupt_frame_t *frame)
{
    uint8_t abc = inb(0x60);
    printk("%x", abc);

    default_irq_handler(frame);
}

void irq2_handler(interrupt_frame_t *frame)
{

    default_irq_handler(frame);
}

void irq3_handler(interrupt_frame_t *frame)
{

    default_irq_handler(frame);
}

void irq4_handler(interrupt_frame_t *frame)
{

    default_irq_handler(frame);
}

void irq5_handler(interrupt_frame_t *frame)
{

    default_irq_handler(frame);
}

void irq6_handler(interrupt_frame_t *frame)
{

    default_irq_handler(frame);
}

void irq7_handler(interrupt_frame_t *frame)
{

    default_irq_handler(frame);
}

// RTC Real Time Clock Timer

uint64_t secondsElapsed;

void irq8_handler(interrupt_frame_t *frame)
{
    secondsElapsed++;
    if (secondsElapsed % 2 == 0)
        *((uint64_t *)0x1f000) = secondsElapsed / 2;
    outb(0x70, 0x0C); // select register C
    inb(0x71);        // just throw away contents
    default_irq_handler(frame);
}

void irq9_handler(interrupt_frame_t *frame)
{

    default_irq_handler(frame);
}

void irq10_handler(interrupt_frame_t *frame)
{

    default_irq_handler(frame);
}

void irq11_handler(interrupt_frame_t *frame)
{

    default_irq_handler(frame);
}

void irq12_handler(interrupt_frame_t *frame)
{

    default_irq_handler(frame);
}

void irq13_handler(interrupt_frame_t *frame)
{

    default_irq_handler(frame);
}

void irq14_handler(interrupt_frame_t *frame)
{

    default_irq_handler(frame);
}

void irq15_handler(interrupt_frame_t *frame)
{

    default_irq_handler(frame);
}