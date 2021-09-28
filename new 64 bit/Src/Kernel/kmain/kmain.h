#ifndef _KMAIN_H_
#define _KMAIN_H_

#define OS TARAOS
#define KERNEL
#define OS BITS64

#include "../Math/math.c"

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
    uint8_t cpuName[12];
} __attribute__((packed)) kernelInfoStructure_t;

/*


kernelInfoStruct:
VBEINFOFORKERNEL:
	bpsl: resw 1
	width: resw 1
	height: resw 1
	bpp: resb 1	
	rmasks: resb 1
	rmaskp: resb 1
	gmasks: resb 1
	gmaskp: resb 1
	bmasks: resb 1
	bmaskp: resb 1
	physicalFramebuffersegm: resw 1
	physicalFramebufferoffs: resw 1
memoryMapLocation: 
	mmap_ss: dw 0x9f00
cpuInfo:
	cpuManufacturer: resb 12


*/
#endif