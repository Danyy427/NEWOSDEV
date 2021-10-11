#ifndef _KMAIN_H_
#define _KMAIN_H_

#define OS TARAOS
#define KERNEL
#define OSARCH BITS64

#include "../Types/types.h"

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
    uint32_t framebuffer;
    uint16_t memoryMap;
    uint16_t kernelSize;
    uint8_t cpuName[12];
} __attribute__((packed)) kernelInfoStructure_t;

extern kernelInfoStructure_t kernelInfo;

#endif

/*


Annen sj zbab ivjjj 31 kernal anan -NPX


if(entrysize>6 inch){

    get.pregnant_anan;

}

else{
    visit info@sjzbab3169.com
}

*/