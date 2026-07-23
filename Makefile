ASM=nasm
CC=gcc
LD=ld

ASMFLAGS=-f elf32

CFLAGS=-m32 -ffreestanding -fno-pic -fno-stack-protector -nostdlib -Wall -Wextra -MMD -MP \
-I$(KERNELDIR) \
-I$(KERNELDIR)/arch/x86 \
-I$(KERNELDIR)/drivers \
-I$(KERNELDIR)/memory \
-I$(KERNELDIR)/fs

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
$(BUILDDIR)/page_fault.o \
$(BUILDDIR)/ata.o \
$(BUILDDIR)/string.o

KERNEL_ELF=$(BUILDDIR)/kernel.elf
ISO=miyaros.iso

.RECIPEPREFIX = >

.PHONY: all clean run

all: $(ISO)

$(BUILDDIR):
> mkdir -p $(BUILDDIR)

$(BOOT_OBJ): $(SRCDIR)/arch/x86/boot.asm | $(BUILDDIR)
> $(ASM) $(ASMFLAGS) $< -o $@

$(BUILDDIR)/kernel.o: $(KERNELDIR)/kernel.c | $(BUILDDIR)
> $(CC) $(CFLAGS) -c $< -o $@

$(BUILDDIR)/serial.o: $(KERNELDIR)/drivers/serial.c | $(BUILDDIR)
> $(CC) $(CFLAGS) -c $< -o $@

$(BUILDDIR)/vga.o: $(KERNELDIR)/drivers/vga.c | $(BUILDDIR)
> $(CC) $(CFLAGS) -c $< -o $@

$(BUILDDIR)/terminal.o: $(KERNELDIR)/terminal.c | $(BUILDDIR)
> $(CC) $(CFLAGS) -c $< -o $@

$(BUILDDIR)/keyboard.o: $(KERNELDIR)/drivers/keyboard.c | $(BUILDDIR)
> $(CC) $(CFLAGS) -c $< -o $@

$(BUILDDIR)/shell.o: $(KERNELDIR)/shell.c | $(BUILDDIR)
> $(CC) $(CFLAGS) -c $< -o $@

$(BUILDDIR)/timer.o: $(KERNELDIR)/drivers/timer.c | $(BUILDDIR)
> $(CC) $(CFLAGS) -c $< -o $@

$(BUILDDIR)/idt.o: $(KERNELDIR)/arch/x86/idt.c | $(BUILDDIR)
> $(CC) $(CFLAGS) -c $< -o $@

$(BUILDDIR)/panic.o: $(KERNELDIR)/panic.c | $(BUILDDIR)
> $(CC) $(CFLAGS) -c $< -o $@

$(BUILDDIR)/isr.o: $(SRCDIR)/arch/x86/isr.asm | $(BUILDDIR)
> $(ASM) $(ASMFLAGS) $< -o $@

$(BUILDDIR)/exceptions.o: $(KERNELDIR)/arch/x86/exceptions.c | $(BUILDDIR)
> $(CC) $(CFLAGS) -c $< -o $@

$(BUILDDIR)/pic.o: $(KERNELDIR)/drivers/pic.c | $(BUILDDIR)
> $(CC) $(CFLAGS) -c $< -o $@

$(BUILDDIR)/irq.o: $(KERNELDIR)/arch/x86/irq.c | $(BUILDDIR)
> $(CC) $(CFLAGS) -c $< -o $@

$(BUILDDIR)/gdt.o: $(KERNELDIR)/arch/x86/gdt.c | $(BUILDDIR)
> $(CC) $(CFLAGS) -c $< -o $@

$(BUILDDIR)/multiboot.o: $(KERNELDIR)/memory/multiboot.c | $(BUILDDIR)
> $(CC) $(CFLAGS) -c $< -o $@

$(BUILDDIR)/memory_map.o: $(KERNELDIR)/memory/memory_map.c | $(BUILDDIR)
> $(CC) $(CFLAGS) -c $< -o $@

$(BUILDDIR)/pmm.o: $(KERNELDIR)/memory/pmm.c | $(BUILDDIR)
> $(CC) $(CFLAGS) -c $< -o $@

$(BUILDDIR)/heap.o: $(KERNELDIR)/memory/heap.c | $(BUILDDIR)
> $(CC) $(CFLAGS) -c $< -o $@

$(BUILDDIR)/paging.o: $(KERNELDIR)/memory/paging.c | $(BUILDDIR)
> $(CC) $(CFLAGS) -c $< -o $@

$(BUILDDIR)/vmm.o: $(KERNELDIR)/memory/vmm.c | $(BUILDDIR)
> $(CC) $(CFLAGS) -c $< -o $@

$(BUILDDIR)/page_fault.o: $(KERNELDIR)/memory/page_fault.c | $(BUILDDIR)
> $(CC) $(CFLAGS) -c $< -o $@

$(BUILDDIR)/ata.o: $(KERNELDIR)/drivers/ata.c | $(BUILDDIR)
> $(CC) $(CFLAGS) -c $< -o $@

$(BUILDDIR)/string.o: $(KERNELDIR)/lib/string.c | $(BUILDDIR)
> $(CC) $(CFLAGS) -c $< -o $@

$(BUILDDIR)/gdtasm.o: $(SRCDIR)/arch/x86/gdt.asm | $(BUILDDIR)
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
