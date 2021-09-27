OUTPUT(ELF64-x86-64);
ENTRY(_kentry)
. = 0x10000;
SECTIONS {
	.top_of_kernel = 0x10000;
	.kernel :
	{
		*(.text*)
		*(.rodata*)
		*(.rdata*)
		*(.data)
	_bss_start = .;
		*(.bss)
		*(COMMON)
		_bss_end = .;
	}
	/DISCARD/ : {
		*(.eh_frame)
		*(.comment)
	}
}