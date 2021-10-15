AS := nasm
ASFLAGS_BIN := -fbin
ASFLAGS_O := -felf64 


ifdef OS
   CC = gcc
   FIND = gfind
else
   ifeq ($(shell uname), Darwin)
      CC = x86_64-elf-gcc
	  FIND = find
   endif
endif

CCFLAGS := -mno-ms-bitfields -g -fno-PIC -m64 -mabi=sysv -ffreestanding -c -fno-stack-check -fno-stack-protector -mno-stack-arg-probe

SRCDIR := Src
BINDIR := Bin
OBJDIR := Obj

BOOTASM	= $(shell $(FIND) $(SRCDIR)/Boot -name '*.asm')
KERNELASM = $(shell $(FIND) $(SRCDIR)/Kernel -name '*.asm')
KERNELC = $(shell $(FIND) $(SRCDIR)/Kernel -name '*.c')

BOOTBIN =  $(addsuffix .bin, $(basename $(notdir $(BOOTASM))))
KERNELASMO = $(addsuffix .o, $(basename $(notdir $(KERNELASM))))
KERNELCO = $(addsuffix .o, $(basename $(notdir $(KERNELC))))

VPATH =  $(dir $(BOOTASM)) $(dir $(KERNELASM)) $(dir $(KERNELC))

build: compile 
	cat $(BINDIR)/MBR.bin $(BINDIR)/Bootloader.bin $(BINDIR)/Afterload.bin > Bin/boot.bin
	x86_64-elf-ld -T link.ld -o $(BINDIR)/kernel.tmp $(OBJDIR)/kentry.o \
										  			 $(OBJDIR)/kmain.o \
										 			 $(OBJDIR)/math.o \
										 			 $(OBJDIR)/string.o \
										 			 $(OBJDIR)/memory.o \
													 $(OBJDIR)/bitmap.o \
													 $(OBJDIR)/pmm.o \
													 $(OBJDIR)/gdtasm.o \
													 $(OBJDIR)/gdt.o \
										  			 $(OBJDIR)/paging.o \
													 $(OBJDIR)/paginga.o \
										  			 $(OBJDIR)/heap.o \
										  			 $(OBJDIR)/io.o \
										  			 $(OBJDIR)/idt.o \
													 $(OBJDIR)/idta.o \
										  			 $(OBJDIR)/isr.o \
													 $(OBJDIR)/irq.o \
													 $(OBJDIR)/video.o 

	x86_64-elf-objcopy -O binary $(BINDIR)/kernel.tmp $(BINDIR)/kernel.bin
	cat $(BINDIR)/boot.bin $(BINDIR)/kernel.bin > $(BINDIR)/os-image.img
	qemu-system-x86_64 -m 256M -monitor stdio $(BINDIR)/os-image.img
	
buildwin: compile
	
	copy /b "$(BINDIR)\MBR.bin" + "$(BINDIR)\Bootloader.bin" + "$(BINDIR)\Afterload.bin" /b ".\$(BINDIR)\boot.bin"
	ld -T link.ld -o $(BINDIR)\kernel.tmp $(OBJDIR)\kentry.o \
										  $(OBJDIR)\kmain.o \
										  $(OBJDIR)\math.o \
										  $(OBJDIR)\string.o \
										  $(OBJDIR)\memory.o \
										  $(OBJDIR)\bitmap.o \
										  $(OBJDIR)\pmm.o \
										  $(OBJDIR)\gdtasm.o \
										  $(OBJDIR)\gdt.o \
										  $(OBJDIR)\paging.o \
										  $(OBJDIR)\paginga.o \
										  $(OBJDIR)\heap.o \
										  $(OBJDIR)\io.o \
										  $(OBJDIR)\idt.o \
										  $(OBJDIR)\idta.o \
										  $(OBJDIR)\isr.o \
										  $(OBJDIR)\irq.o \
										  $(OBJDIR)\video.o 

	objcopy -O binary $(BINDIR)\kernel.tmp $(BINDIR)\kernel.bin
	copy /b $(BINDIR)\boot.bin + $(BINDIR)\kernel.bin /b $(BINDIR)\os-image.img
	qemu-system-x86_64 -m 256M -monitor stdio $(BINDIR)\os-image.img
	
rebuild: clean compile

compile: $(BOOTBIN) $(KERNELASMO) $(KERNELCO)

$(BOOTBIN) : %.bin : %.asm
	$(AS) $(ASFLAGS_BIN) $< -o $(BINDIR)/$@
 
$(KERNELASMO): %.o : %.asm
	$(AS) $(ASFLAGS_O) $< -o $(OBJDIR)/$@

$(KERNELCO): %.o : %.c
	$(CC) $(CCFLAGS) $< -o $(OBJDIR)/$@


clean:
	rm -rf $(BINDIR)/%.bin


