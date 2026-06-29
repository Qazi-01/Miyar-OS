ASM=nasm
CC=gcc
LD=ld

ASMFLAGS=-f elf32
CFLAGS=-m32 -ffreestanding -fno-pic -fno-stack-protector -nostdlib -Wall -Wextra
LDFLAGS=-m elf_i386 -T linker.ld -z max-page-size=0x1000

SRCDIR=src
KERNELDIR=kernel
BUILDDIR=build
ISODIR=iso

BOOT_OBJ=$(BUILDDIR)/boot.o

KERNEL_OBJS=\
$(BUILDDIR)/kernel.o \
$(BUILDDIR)/serial.o \
$(BUILDDIR)/vga.o \
$(BUILDDIR)/terminal.o \
$(BUILDDIR)/keyboard.o

KERNEL_ELF=$(BUILDDIR)/kernel.elf
ISO=miyaros.iso

.RECIPEPREFIX = >

.PHONY: all clean run

all: $(ISO)

$(BUILDDIR):
> mkdir -p $(BUILDDIR)

$(BOOT_OBJ): $(SRCDIR)/boot.asm | $(BUILDDIR)
> $(ASM) $(ASMFLAGS) $< -o $@

$(BUILDDIR)/kernel.o: $(KERNELDIR)/kernel.c | $(BUILDDIR)
> $(CC) $(CFLAGS) -c $< -o $@

$(BUILDDIR)/serial.o: $(KERNELDIR)/serial.c | $(BUILDDIR)
> $(CC) $(CFLAGS) -c $< -o $@

$(BUILDDIR)/vga.o: $(KERNELDIR)/vga.c | $(BUILDDIR)
> $(CC) $(CFLAGS) -c $< -o $@

$(BUILDDIR)/terminal.o: $(KERNELDIR)/terminal.c | $(BUILDDIR)
> $(CC) $(CFLAGS) -c $< -o $@

$(BUILDDIR)/keyboard.o: $(KERNELDIR)/keyboard.c | $(BUILDDIR)
> $(CC) $(CFLAGS) -c $< -o $@

$(KERNEL_ELF): $(BOOT_OBJ) $(KERNEL_OBJS)
> $(LD) $(LDFLAGS) -o $@ $^

$(ISO): $(KERNEL_ELF)
> cp $(KERNEL_ELF) $(ISODIR)/boot/
> grub-mkrescue -o $@ $(ISODIR)

run: $(ISO)
> qemu-system-i386 -cdrom $(ISO) -nographic -no-reboot

clean:
> rm -rf $(BUILDDIR) $(ISO)
