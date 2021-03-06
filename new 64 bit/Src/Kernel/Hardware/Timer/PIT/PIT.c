#include "PIT.h"
#include "../../../IO/io.h"

void initPIT(uint64_t freq)
{
    SetPITSpeed(freq);
}

void SetPITSpeed(int hz)
{
    int divisor = 1193180 / hz; /* Calculate our divisor */
    outb(0x43, 0x36);           /* Set our command byte 0x36 */
    outb(0x40, divisor & 0xFF); /* Set low byte of divisor */
    outb(0x40, divisor >> 8);   /* Set high byte of divisor */
}