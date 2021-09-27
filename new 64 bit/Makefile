AS := nasm
ASFLAGS_BIN := -fbin
ASFLAGS_O := -felf64 
CC := x86_64-elf-gcc
WCC := gcc
CCFLAGS := -mno-ms-bitfields -fno-PIC -m64 -ffreestanding -c

SRCDIR := Src
BINDIR := Bin
OBJDIR := Obj

BOOTASM	= $(shell gfind $(SRCDIR)/Boot -name '*.asm')
KERNELASM = $(shell gfind $(SRCDIR)/Kernel -name '*.asm')
KERNELC = $(shell gfind $(SRCDIR)/Kernel -name '*.c')

BOOTBIN =  $(addsuffix .bin, $(basename $(notdir $(BOOTASM))))
KERNELASMO = $(addsuffix .o, $(basename $(notdir $(KERNELASM))))
KERNELCO = $(addsuffix .o, $(basename $(notdir $(KERNELC))))

VPATH =  $(dir $(BOOTASM)) $(dir $(KERNELASM)) $(dir $(KERNELCO))

build: compile 
	cat Bin/MBR.bin Bin/Bootloader.bin Bin/Afterload.bin > Bin/boot.bin
	x86_64-elf-ld -T link.ld -o $(BINDIR)/kernel.tmp Obj/kentry.o Obj/kmain.o
	x86_64-elf-objcopy -O binary $(BINDIR)/kernel.tmp $(BINDIR)/kernel.bin
	cat $(BINDIR)/boot.bin $(BINDIR)/kernel.bin > $(BINDIR)/os-image.img
	qemu-system-x86_64 -monitor stdio $(BINDIR)/os-image.img
	
buildwin: compilewin 
	copy /b "Bin\MBR.bin" + "Bin\Bootloader.bin" + "Bin\Afterload.bin" /b "Bin\boot.bin"
	ld -T link.ld -o $(BINDIR)\kernel.tmp Obj\kentry.o Obj\kmain.o
	objcopy -O binary $(BINDIR)\kernel.tmp $(BINDIR)\kernel.bin
	copy /b $(BINDIR)\boot.bin + $(BINDIR)\kernel.bin + $(BINDIR)\os-image.img
	qemu-system-x86_64 -monitor stdio $(BINDIR)\os-image.img
	
rebuild: clean compile

compile: $(BOOTBIN) $(KERNELASMO) $(KERNELCO)
compilewin: $(BOOTBIN) $(KERNELASMO) $(KERNELCOW)

$(BOOTBIN) : %.bin : %.asm
	$(AS) $(ASFLAGS_BIN) $< -o $(BINDIR)/$@

$(KERNELASMO): %.o : %.asm
	$(AS) $(ASFLAGS_O) $< -o $(OBJDIR)/$@

$(KERNELCO): %.o : %.c
	$(CC) $(CCFLAGS) $< -o $(OBJDIR)/$@

$(KERNELCOW): %.o : %.c
	$(WCC) $(CCFLAGS) $< -o $(OBJDIR)/$@

clean:
	rm -rf $(BINDIR)/%.bin


