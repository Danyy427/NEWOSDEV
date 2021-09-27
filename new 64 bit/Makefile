AS := nasm
ASFLAGS_BIN := -fbin
ASFLAGS_O := -felf64 
CC := x86_64-elf-gcc
CCFLAGS := -mno-ms-bitfields -fno-PIC -m64 -ffreestanding -c

SRCDIR := Src
BINDIR := Bin
OBJDIR := Obj

BOOTASM	= $(shell find $(SRCDIR)/Boot -name '*.asm')
KERNELASM = $(shell find $(SRCDIR)/Kernel -name '*.asm')
KERNELC = $(shell find $(SRCDIR)/Kernel -name '*.c')

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


