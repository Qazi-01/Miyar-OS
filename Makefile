ASM=nasm
CC=gcc
LD=ld

ASMFLAGS=-f elf32
CFLAGS=-m32 -ffreestanding -fno-pic -nostdlib -Wall -Wextra
LDFLAGS=-m elf_i386 -T linker.ld -z max-page-size=0x1000

SRCDIR=src
KERNELDIR=kernel
BUILDDIR=build
ISODIR=iso

KERNEL_BIN=$(BUILDDIR)/kernel.bin
KERNEL_ELF=$(BUILDDIR)/kernel.elf
ISO=miyaros.iso

.PHONY: all clean run iso

all: $(ISO)

$(BUILDDIR)/boot.o: $(SRCDIR)/boot.asm | $(BUILDDIR)
       $(ASM) $(ASMFLAGS) $< -o $@

$(BUILDDIR)/kernel.o: $(KERNELDIR)/kernel.c | $(BUILDDIR)
       $(CC) $(CFLAGS) -c $< -o $@

$(KERNEL_ELF): $(BUILDDIR)/boot.o $(BUILDDIR)/kernel.o
       $(LD) $(LDFLAGS) -o $@ $^

$(KERNEL_BIN): $(KERNEL_ELF)
       objcopy -O binary $< $@

$(ISO): $(KERNEL_ELF)
       cp $(KERNEL_ELF) $(ISODIR)/boot/
	 grub-mkrescue -o $@ $(ISODIR)

$(BUILDDIR):
       mkdir -p $@

run: $(ISO)
       qemu-system-i386 -cdrom $(ISO) -nographic -no-reboot

clean:
       rm -rf $(BUILDDIR) $(ISO)