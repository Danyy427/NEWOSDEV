#ifndef _KMAIN_H_
#define _KMAIN_H_

#define OS TARAOS
#define KERNEL
#define OSARCH BITS64

#include "../Math/math.h"
#include "../Types/types.h"
#include "../String/string.h"
#include "../Memory/memory.h"
#include "../Memory/pmm/pmm.h"

typedef struct
{
    uint16_t bpsl;
    uint16_t width;
    uint16_t height;
    uint8_t bpp;
    uint8_t rmasks;
    uint8_t rmaskp;
    uint8_t gmasks;
    uint8_t gmaskp;
    uint8_t bmasks;
    uint8_t bmaskp;
    uint16_t framebuffersegm;
    uint16_t framebufferoff;
    uint16_t memoryMap;
    uint16_t kernelSize;
    uint8_t cpuName[12];
} __attribute__((packed)) kernelInfoStructure_t;

#endif