#ifndef _IRQ_H_
#define _IRQ_H_

#include "../idt.h"

void default_irq_handler(interrupt_frame_t *frame);

extern uint64_t environmentTickCount;

void irq0_handler(interrupt_frame_t *frame);

void irq1_handler(interrupt_frame_t *frame);
void irq2_handler(interrupt_frame_t *frame);
void irq3_handler(interrupt_frame_t *frame);
void irq4_handler(interrupt_frame_t *frame);
void irq5_handler(interrupt_frame_t *frame);
void irq6_handler(interrupt_frame_t *frame);
void irq7_handler(interrupt_frame_t *frame);
void irq8_handler(interrupt_frame_t *frame);
void irq9_handler(interrupt_frame_t *frame);
void irq10_handler(interrupt_frame_t *frame);
void irq11_handler(interrupt_frame_t *frame);
void irq12_handler(interrupt_frame_t *frame);
void irq13_handler(interrupt_frame_t *frame);
void irq14_handler(interrupt_frame_t *frame);
void irq15_handler(interrupt_frame_t *frame);

#endif