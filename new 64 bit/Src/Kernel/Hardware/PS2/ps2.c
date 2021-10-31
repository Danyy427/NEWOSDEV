#include "ps2.h"
#include "../../IO/io.h"

uint8_t firstPortDevice[3];
uint8_t secondPortDevice[3];

void controllerWait(unsigned char type) //unsigned char
{
    int _time_out = 100000; //unsigned int
    if (type == 0)
    {
        while (_time_out--) //Data
        {
            if ((inb(PS2STATUS) & 1) == 1)
            {
                return;
            }
        }
        return;
    }
    else
    {
        while (_time_out--) //Signal
        {
            if ((inb(PS2STATUS) & 2) == 0)
            {
                return;
            }
        }
        return;
    }
}

unsigned char getPS2ControllerStatus()
{
    controllerWait(PS2WRITE);
    outb(PS2COMMAND, 0x20);
    controllerWait(PS2READ);
    return inb(0x60);
}

void setPS2ControllerStatus(unsigned char status)
{
    controllerWait(PS2WRITE);
    outb(PS2COMMAND, 0x60);
    outb(PS2DATA, status);
}

void disableSecondPS2Port()
{
    controllerWait(PS2WRITE);
    outb(PS2COMMAND, 0xA7);
}

void enableSecondPS2Port()
{
    controllerWait(PS2WRITE);
    outb(PS2COMMAND, 0xA8);
}

void disableFirstPS2Port()
{
    controllerWait(PS2WRITE);
    outb(PS2COMMAND, 0xAD);
}

void enableFirstPS2Port()
{
    controllerWait(PS2WRITE);
    outb(PS2COMMAND, 0xAE);
}

void commandPS2Controller(unsigned char command)
{
    controllerWait(PS2WRITE);
    outb(PS2COMMAND, command);
}

void writeToFirstPS2Port(unsigned char byte)
{
    controllerWait(PS2WRITE);
    outb(PS2COMMAND, byte);
}

void writeToSecondPS2Port(unsigned char byte)
{
    controllerWait(PS2WRITE);
    outb(PS2COMMAND, 0xD4);
    controllerWait(PS2WRITE);
    outb(PS2DATA, byte);
}

unsigned char readPS2DataPort()
{
    controllerWait(PS2READ);
    return inb(PS2DATA);
}

uint8_t initPS2()
{
    // Start by disabling devices

    disableFirstPS2Port();
    disableSecondPS2Port();

    // Flush the input buffer

    inb(PS2DATA);

    // Set the state of the status register.

    outb(PS2COMMAND, 0x20);
    uint8_t status = inb(PS2DATA);
    uint8_t dual0 = status & 0b00100000 == 0b00100000; // if dual = 0, there is no second channel
    status |= 0b01000011;
    outb(PS2COMMAND, 0x60);
    outb(PS2DATA, status);

    // Perform self test

    outb(PS2COMMAND, 0xAA);
    uint8_t response = inb(PS2DATA);
    if (response == 0xFC)
        return 0xFC;
    outb(PS2COMMAND, 0x60);
    outb(PS2DATA, status);

    // test dual channel controller
    uint8_t dual = 0xFF;

    if (dual0 == 1)
    {
        outb(PS2COMMAND, 0xAE);
        outb(PS2COMMAND, 0x20);
        status = inb(PS2DATA);
        uint8_t dual1 = status & 0b00100000 == 0b00100000;
        // if dual1 == 0, dual channel exists
        outb(PS2COMMAND, 0xAD);
        dual = !dual1;
    }
    else
    {
        dual = 0;
    }

    // Perform interface tests

    outb(PS2COMMAND, 0xAB);
    uint8_t responseIT1 = inb(PS2DATA);
    uint8_t responseIT2;
    if (dual == 1)
    {
        outb(PS2COMMAND, 0xA9);
        responseIT2 = inb(PS2DATA);
    }

    if (responseIT1 != 0x0)
        return responseIT1;
    if (responseIT2 != 0x0 && dual == 1)
        return responseIT2;

    // Enable devices

    outb(PS2COMMAND, 0xAE);
    if (dual == 1)
        outb(PS2COMMAND, 0xA8);

    return 0;
}