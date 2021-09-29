#include "kmain.h"

int kmain(kernelInfoStructure_t *infostruct)
{
	uint64_t memory = getSystemMemory((memoryMap_t *)infostruct->memoryMap);
	uint64_t *ww = 0x1f000;
	*ww = memory;

	while (1)
		;
	return 0;
}