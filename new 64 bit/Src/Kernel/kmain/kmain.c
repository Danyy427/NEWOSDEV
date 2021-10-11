#include "kmain.h"

#include "../String/string.h"
#include "../Memory/memory.h"
#include "../Memory/pmm/pmm.h"
#include "../GDT/gdt.h"
#include "../Memory/vmm/paging.h"
#include "../Memory/vmm/heap.h"
#include "../Math/math.h"

kernelInfoStructure_t kernelInfo;

int kmain(kernelInfoStructure_t *infostruct)
{
	memcpy(&kernelInfo, infostruct, sizeof(kernelInfo));
	initMemoryMap((memoryMap_t *)kernelInfo.memoryMap);
	initPhysicalPageFrameAllocation(kernelInfo.kernelSize);
	initGDT();
	initPaging();
	initHeap(0x20000, 6);

	uint64_t ptr = kmalloc(24);

	uint64_t *ww = 0x1f000;
	*ww = ptr;
	while (1)
		;
	return 0;
}