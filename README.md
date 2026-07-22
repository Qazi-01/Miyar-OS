# MiyarOS

MiyarOS is a hobby operating system for x86 that boots with GRUB and follows the Multiboot specification. The kernel is written in C and x86 Assembly, and now includes a basic memory management subsystem alongside a text-mode terminal, keyboard-driven shell, interrupt handling, paging, and serial debugging.

## Why I Built It

I built MiyarOS to understand how a kernel comes together from the first boot instruction through to a usable command line. It is a hands-on project for learning low-level x86 development, memory management, interrupt handling, and operating system design without relying on existing kernels or frameworks.

## Current Version

**v0.2 – Memory Management**

A bootable release ISO is available in the `release/` directory and on the project's GitHub Releases page.

## Releases

Each tagged release includes a bootable ISO that can be downloaded from the GitHub Releases page.

Alternatively, you can clone the repository and build MiyarOS from source using the provided Makefile. This allows you to experiment with the codebase, modify the kernel, and generate your own bootable ISO.

## Features

- Multiboot-compliant 32-bit x86 boot flow through GRUB.
- Kernel written in C and x86 Assembly.
- Global Descriptor Table (GDT).
- Interrupt Descriptor Table (IDT).
- PIC remapping and IRQ handling.
- CPU exception handling.
- Dedicated kernel panic screen with diagnostic information.
- Dedicated page fault handler.
- Multiboot memory map detection.
- Physical Memory Manager (PMM).
- Bitmap-based physical frame allocator.
- Kernel heap (`kmalloc` / `kfree`).
- Paging support.
- Virtual Memory Manager (VMM).
- Dynamic page table creation.
- Read-only kernel memory protection.
- PS/2 keyboard driver with Shift support.
- VGA text-mode terminal with scrolling and backspace.
- Serial debugging output.
- PIT timer and uptime reporting.
- Table-driven interactive shell.

## Built-In Shell Commands

| Command | Description |
| --- | --- |
| `help` | Shows the built-in command list. |
| `about` | Displays project information. |
| `echo <msg>` | Prints the provided message. |
| `uptime` | Shows the number of seconds since boot. |
| `clear` | Clears the VGA text screen. |
| `reboot` | Reboots the machine. |
| `shutdown` | Attempts to power off the machine. |
| `exception` | Triggers a divide-by-zero exception for testing. |
| `pagefault` | Triggers a page fault for testing the memory subsystem. |

## Repository Structure(v0.3, in progress)

```text
Miyar-OS/
├── Makefile
├── README.md
├── LICENSE
├── .gitignore
├── miyaros.iso
│   (after compilation or in release)
├── build/
│   (after compilation)
│
├── iso/
│   └── boot/
│       └── grub/
│           └── grub.cfg
│
├── src/
│   └── arch/
│       └── x86/
│           ├── boot.asm
│           ├── gdt.asm
│           ├── isr.asm
│           └── linker.ld
│
└── kernel/
    ├── arch/
    │   └── x86/
    │       ├── gdt.c
    │       ├── gdt.h
    │       ├── idt.c
    │       ├── idt.h
    │       ├── irq.c
    │       ├── irq.h
    │       ├── exceptions.c
    │       └── exceptions.h
    │
    ├── drivers/
    │   ├── ata.c
    │   ├── ata.h
    │   ├── disk.c
    │   ├── disk.h
    │   ├── keyboard.c
    │   ├── keyboard.h
    │   ├── pic.c
    │   ├── pic.h
    │   ├── serial.c
    │   ├── serial.h
    │   ├── timer.c
    │   ├── timer.h
    │   ├── vga.c
    │   ├── vga.h
    │   └── io.h
    │
    ├── memory/
    │   ├── multiboot.c
    │   ├── multiboot.h
    │   ├── memory_map.c
    │   ├── memory_map.h
    │   ├── pmm.c
    │   ├── pmm.h
    │   ├── heap.c
    │   ├── heap.h
    │   ├── paging.c
    │   ├── paging.h
    │   ├── vmm.c
    │   ├── vmm.h
    │   ├── page_fault.c
    │   └── page_fault.h
    │
    ├── fs/
    │   ├── directory.c
    │   ├── directory.h
    │   ├── fat32.c
    │   ├── fat32.h
    │   ├── file.c
    │   ├── file.h
    │   ├── fs.c
    │   ├── fs.h
    │   ├── path.c
    │   └── path.h
    │
    ├── kernel.c
    ├── panic.c
    ├── panic.h
    ├── shell.c
    ├── shell.h
    ├── terminal.c
    └── terminal.h
```

## Build Requirements

- GNU Make
- NASM
- GCC with 32-bit multilib support
- GNU binutils (`ld`)
- GRUB (`grub-mkrescue`)
- xorriso
- QEMU (recommended for testing)

## Build Instructions

From the project root:

```bash
make clean
make
```

This builds the kernel and generates a bootable ISO.

## Run Instructions

The easiest way to boot the operating system is:

```bash
make run
```

Or run the generated ISO manually:

```bash
qemu-system-i386 -cdrom release/MiyarOS-v0.2.iso
```

For terminal-only environments:

```bash
qemu-system-i386 -curses -cdrom release/MiyarOS-v0.2.iso
```

## Roadmap

### Completed

- **v0.1 — Core Kernel**
  - Boot process
  - Interrupts
  - Drivers
  - Terminal
  - Shell

- **v0.2 — Memory Management**
  - Physical Memory Manager
  - Kernel heap
  - Paging
  - Virtual Memory Manager
  - Page fault handling
  - Read-only kernel memory protection

### Planned

- Filesystem
- Process management
- User mode
- System calls
- Virtual File System (VFS)
- Improved hardware support
- Networking
- Graphical user interface

## License

MiyarOS is released under the GNU General Public License v3.0. See [LICENSE](LICENSE) for the full license text.
