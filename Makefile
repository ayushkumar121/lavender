PNAME=lavender
ARCH=x86
IDIR=src/include
SDIR=src
ODIR=bin
CC=gcc

DIRS=$(ODIR) $(ODIR)/$(ARCH) $(ODIR)/kernel $(ODIR)/kernel \
	$(ODIR)/arch/$(ARCH)/dev $(ODIR)/arch/$(ARCH)/gfx $(ODIR)/arch/$(ARCH)/lib $(ODIR)/arch/$(ARCH)/sys


DEPS   := $(shell find $(IDIR) -name "*.h")
SFILES := $(shell find $(SDIR) -name "*.c")
OBJ=$(patsubst $(SDIR)/%.c, $(ODIR)/%.o, $(SFILES))

CFLAGS=-std=gnu17 -Wall -nostdlib -fno-stack-protector -ffreestanding -m64 -masm=intel -mgeneral-regs-only -I$(IDIR)

$(shell mkdir -p $(DIRS))

$(ODIR)/%.o: $(SDIR)/%.c $(DEPS)
	$(CC) -c $< -o $@ $(CFLAGS)

build: $(OBJ)
	nasm -f elf64 $(SDIR)/kernel/kernel_entry.asm -o $(ODIR)/kernel/kernel_entry.o
	ld  -o $(ODIR)/$(ARCH)/full_kernel.bin -Ttext 0x1000 $(ODIR)/kernel/kernel_entry.o $(OBJ) --oformat binary 

	nasm -f bin $(SDIR)/boot/boot.asm -o $(ODIR)/$(ARCH)/boot.bin
	nasm -f bin $(SDIR)/boot/padding.asm -o $(ODIR)/$(ARCH)/padding.bin
	cat  $(ODIR)/$(ARCH)/boot.bin $(ODIR)/$(ARCH)/full_kernel.bin $(ODIR)/$(ARCH)/padding.bin \
	> $(ODIR)/$(ARCH)/$(PNAME).bin

	# dd if=/dev/zero of=$(ODIR)/$(ARCH)/$(PNAME).bin bs=512 count=2880 >/dev/null
	# dd if=$(ODIR)/$(ARCH)/osimage.bin of=$(ODIR)/$(ARCH)/$(PNAME).bin conv=notrunc >/dev/null

clean:
	rm -rdf bin

run:
	qemu-system-x86_64 -drive file=$(ODIR)/$(ARCH)/$(PNAME).bin,format=raw,index=0,if=floppy, \
	 -m 128M