#ifndef _PS2_H_
#define _PS2_H_

#define PS2DATA 0x60
#define PS2STATUS 0x64
#define PS2COMMAND 0x64
#include "../../Types/types.h"

uint8_t initPS2();

void controllerWait(unsigned char type);

unsigned char getPS2ControllerStatus();

void setPS2ControllerStatus(unsigned char status);

void disableSecondPS2Port();

void enableSecondPS2Port();

void disableFirstPS2Port();

void enableFirstPS2Port();

void commandPS2Controller(unsigned char command);

void writeToFirstPS2Port(unsigned char byte);

void writeToSecondPS2Port(unsigned char byte);

unsigned char readPS2DataPort();

enum PS2WRITEREAD
{
    PS2READ,
    PS2WRITE
};

extern uint8_t firstPortDevice[3];
extern uint8_t secondPortDevice[3];

#endif