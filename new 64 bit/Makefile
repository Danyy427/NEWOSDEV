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

CCFLAGS := -mno-ms-bitfields -fno-PIC -m64 -ffreestanding -c

SRCDIR := Src
BINDIR := Bin
OBJDIR := Obj

BOOTASM	= $(shell $(FIND) $(SRCDIR)/Boot -name '*.asm')
KERNELASM = $(shell $(FIND) $(SRCDIR)/Kernel -name '*.asm')
KERNELC = $(shell $(FIND) $(SRCDIR)/Kernel -name '*.c')

BOOTBIN =  $(addsuffix .bin, $(basename $(notdir $(BOOTASM))))
KERNELASMO = $(addsuffix .o, $(basename $(notdir $(KERNELASM))))
KERNELCO = $(addsuffix .o, $(basename $(notdir $(KERNELC))))
KERNELCOW = $(addsuffix .o, $(basename $(notdir $(KERNELC))))

VPATH =  $(dir $(BOOTASM)) $(dir $(KERNELASM)) $(dir $(KERNELC))

build: compile 
	cat $(BINDIR)/MBR.bin $(BINDIR)/Bootloader.bin $(BINDIR)/Afterload.bin > $(BINDIR)/boot.bin
	x86_64-elf-ld -T link.ld -o $(BINDIR)/kernel.tmp $(OBJDIR)/kentry.o $(OBJDIR)/kmain.o Obj/math.o
	x86_64-elf-objcopy -O binary $(BINDIR)/kernel.tmp $(BINDIR)/kernel.bin
	cat $(BINDIR)/boot.bin $(BINDIR)/kernel.bin > $(BINDIR)/os-image.img
	qemu-system-x86_64 -monitor stdio $(BINDIR)/os-image.img
	
buildwin: compile
	
	copy /b "$(BINDIR)\MBR.bin" + "$(BINDIR)\Bootloader.bin" + "$(BINDIR)\Afterload.bin" /b ".\$(BINDIR)\boot.bin"
	ld -T link.ld -o $(BINDIR)\kernel.tmp $(OBJDIR)\kentry.o $(OBJDIR)\kmain.o $(OBJDIR)\math.o
	objcopy -O binary $(BINDIR)\kernel.tmp $(BINDIR)\kernel.bin
	copy /b $(BINDIR)\boot.bin + $(BINDIR)\kernel.bin /b $(BINDIR)\os-image.img
	qemu-system-x86_64 -monitor stdio $(BINDIR)\os-image.img
	
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


