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
#include "../Hardware/Timer/PIT/PIT.h"
#include "../Hardware/Timer/RTC/RTC.h"
#include "../Hardware/PS2/ps2.h"

kernelInfoStructure_t kernelInfo;

int kmain(kernelInfoStructure_t *infostruct)
{

	memcpy(&kernelInfo, infostruct, sizeof(kernelInfo));

	initMemoryMap((memoryMap_t *)kernelInfo.memoryMap);
	initPhysicalPageFrameAllocation(kernelInfo.kernelSize);
	initGDT();
	initPaging();
	initCursor();
	initDefaultFont();
	printk("%d\n", infostruct->kernelSize);
	initIDT();
	initPIT(100);
	initRTC(15);

	return 0;
}