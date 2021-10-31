#include "RTC.h"
#include "../../../IO/io.h"

void initRTC(uint64_t rate)
{
    asm volatile("cli");
    outb(0x70, 0x8A); // select Status Register A, and disable NMI (by setting the 0x80 bit)
    outb(0x71, 0x20); // write to CMOS/RTC RAM

    rate &= 0x0F;                     // rate must be above 2 and not over 15
    outb(0x70, 0x8A);                 // set index to register A, disable NMI
    char prev = inb(0x71);            // get initial value of register A
    outb(0x70, 0x8A);                 // reset index to A
    outb(0x71, (prev & 0xF0) | rate); //write only our rate to A. Note, rate is the bottom 4 bits.
    asm volatile("sti");
    startRTC();
}

void startRTC()
{
    asm volatile("cli");
    outb(0x70, 0x8B);        // select register B, and disable NMI
    char prev = inb(0x71);   // read the current value of register B
    outb(0x70, 0x8B);        // set the index again (a read will reset the index to register D)
    outb(0x71, prev | 0x40); // write the previous value ORed with 0x40. This turns on bit 6 of register B
    asm volatile("sti");

    outb(0x70, 0x0C); // select register C
    inb(0x71);        // just throw away contents
}