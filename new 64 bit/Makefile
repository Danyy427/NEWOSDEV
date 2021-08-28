AS := nasm
ASFLAGS_BIN := -fbin

SRCDIR := Src
BINDIR := Bin

BOOTASM	= $(shell find $(SRCDIR) -name '*.asm')
BOOTBIN =  $(addsuffix .bin, $(basename $(notdir $(BOOTASM))))
VPATH =  $(dir $(BOOTASM))


build: compile 
	cat Bin/MBR.bin Bin/Bootloader.bin Bin/Afterload.bin > Bin/os-image.img
	qemu-system-x86_64 -monitor stdio Bin/os-image.img

rebuild: clean compile

compile: $(BOOTBIN)

$(BOOTBIN) : %.bin : %.asm
	$(AS) $(ASFLAGS_BIN) $< -o $(BINDIR)/$@

clean:
	rm -rf $(BINDIR)/%.bin


