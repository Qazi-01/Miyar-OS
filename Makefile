ASM=nasm
CC=gcc
LD=ld

ASMFLAGS=-f elf32
CFLAGS=-m32 -ffreestanding -fno-pic -fno-stack-protector -nostdlib -Wall -Wextra -MMD -MP
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
$(BUILDDIR)/keyboard.o \
$(BUILDDIR)/shell.o \
$(BUILDDIR)/timer.o \
$(BUILDDIR)/idt.o \
$(BUILDDIR)/isr.o \
$(BUILDDIR)/panic.o \
$(BUILDDIR)/exceptions.o \
$(BUILDDIR)/pic.o \
$(BUILDDIR)/irq.o \
$(BUILDDIR)/gdt.o \
$(BUILDDIR)/gdtasm.o \
$(BUILDDIR)/multiboot.o \
$(BUILDDIR)/memory_map.o \
$(BUILDDIR)/pmm.o \
$(BUILDDIR)/heap.o \
$(BUILDDIR)/paging.o \
$(BUILDDIR)/vmm.o \
$(BUILDDIR)/page_fault.o

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

$(BUILDDIR)/shell.o: $(KERNELDIR)/shell.c | $(BUILDDIR)
> $(CC) $(CFLAGS) -c $< -o $@

$(BUILDDIR)/timer.o: $(KERNELDIR)/timer.c | $(BUILDDIR)
> $(CC) $(CFLAGS) -c $< -o $@

$(BUILDDIR)/idt.o: $(KERNELDIR)/idt.c | $(BUILDDIR)
> $(CC) $(CFLAGS) -c $< -o $@

$(BUILDDIR)/panic.o: $(KERNELDIR)/panic.c | $(BUILDDIR)
> $(CC) $(CFLAGS) -c $< -o $@

$(BUILDDIR)/isr.o: $(SRCDIR)/isr.asm | $(BUILDDIR)
> $(ASM) $(ASMFLAGS) $< -o $@

$(BUILDDIR)/exceptions.o: $(KERNELDIR)/exceptions.c | $(BUILDDIR)
> $(CC) $(CFLAGS) -c $< -o $@

$(BUILDDIR)/pic.o: $(KERNELDIR)/pic.c | $(BUILDDIR)
> $(CC) $(CFLAGS) -c $< -o $@

$(BUILDDIR)/irq.o: $(KERNELDIR)/irq.c | $(BUILDDIR)
> $(CC) $(CFLAGS) -c $< -o $@

$(BUILDDIR)/gdt.o: $(KERNELDIR)/gdt.c | $(BUILDDIR)
> $(CC) $(CFLAGS) -c $< -o $@

$(BUILDDIR)/multiboot.o: $(KERNELDIR)/multiboot.c | $(BUILDDIR)
> $(CC) $(CFLAGS) -c $< -o $@

$(BUILDDIR)/memory_map.o: $(KERNELDIR)/memory_map.c | $(BUILDDIR)
> $(CC) $(CFLAGS) -c $< -o $@

$(BUILDDIR)/pmm.o: $(KERNELDIR)/pmm.c | $(BUILDDIR)
> $(CC) $(CFLAGS) -c $< -o $@

$(BUILDDIR)/heap.o: $(KERNELDIR)/heap.c | $(BUILDDIR)
> $(CC) $(CFLAGS) -c $< -o $@

$(BUILDDIR)/paging.o: $(KERNELDIR)/paging.c | $(BUILDDIR)
> $(CC) $(CFLAGS) -c $< -o $@

$(BUILDDIR)/vmm.o: $(KERNELDIR)/vmm.c | $(BUILDDIR)
> $(CC) $(CFLAGS) -c $< -o $@

$(BUILDDIR)/page_fault.o: $(KERNELDIR)/page_fault.c | $(BUILDDIR)
> $(CC) $(CFLAGS) -c $< -o $@

$(BUILDDIR)/gdtasm.o: $(SRCDIR)/gdt.asm | $(BUILDDIR)
> $(ASM) $(ASMFLAGS) $< -o $@

$(KERNEL_ELF): $(BOOT_OBJ) $(KERNEL_OBJS)
> $(LD) $(LDFLAGS) -o $@ $^

$(ISO): $(KERNEL_ELF)
> cp $(KERNEL_ELF) $(ISODIR)/boot/
> grub-mkrescue -o $@ $(ISODIR)

run: $(ISO)
> qemu-system-i386 -cdrom $(ISO) -nographic

clean:
> rm -rf $(BUILDDIR) $(ISO)

-include $(wildcard $(BUILDDIR)/*.d)