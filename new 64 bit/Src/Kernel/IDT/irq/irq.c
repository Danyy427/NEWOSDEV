#include "irq.h"

void default_irq_handler(interrupt_frame_t *frame)
{
    /* If IRQ # on slave PIC send EOI to slave */
    if (frame->int_no >= 8)
        outb(0xA0, 0x20);

    outb(0x20, 0x20);
}

void irq0_handler(interrupt_frame_t *frame)
{

    default_irq_handler(frame);
}

void irq1_handler(interrupt_frame_t *frame)
{

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

void irq8_handler(interrupt_frame_t *frame)
{

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