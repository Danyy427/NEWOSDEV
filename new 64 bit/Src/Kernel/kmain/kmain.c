#include "kmain.h"

int kmain(kernelInfoStructure_t *infostruct)
{

	initMemoryMap((memoryMap_t *)infostruct->memoryMap);
	initPhysicalPageFrameAllocation(infostruct->kernelSize);
	uint64_t memory = RequestPhysicalPage();
	uint64_t *ww = 0x1f000;
	*ww = memory;

	while (1)
		;
	return 0;
}