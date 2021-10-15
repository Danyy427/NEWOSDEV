#include "kmain.h"

#include "../String/string.h"
#include "../Memory/memory.h"
#include "../Memory/pmm/pmm.h"
#include "../GDT/gdt.h"
#include "../Memory/vmm/paging.h"
#include "../Memory/vmm/heap.h"
#include "../Math/math.h"
#include "../IDT/idt.h"
#include "../Video/video.h"
#include "../Video/cursor.h"
#include "../Printf/printf.h"

kernelInfoStructure_t kernelInfo;

int kmain(kernelInfoStructure_t *infostruct)
{

	memcpy(&kernelInfo, infostruct, sizeof(kernelInfo));
	initMemoryMap((memoryMap_t *)kernelInfo.memoryMap);
	initPhysicalPageFrameAllocation(kernelInfo.kernelSize);
	initGDT();
	initPaging();
	initIDT();
	initCursor();
	initDefaultFont();

	char *str = "World";
	printk("Hello, %s!", str);

	while (1)
		;
	return 0;
}