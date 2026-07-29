# Miyar-OS

MiyarOS is a hobby operating system for 32-bit x86 that boots with GRUB and follows the Multiboot specification. The kernel is written entirely in C and x86 Assembly and is developed from scratch as a learning project. It currently features memory management, a FAT32 filesystem, a command-line shell, device drivers, interrupt handling, and virtual memory.

## Why I Built It

MiyarOS was created to understand how an operating system works from the ground up, from the very first boot instruction to a functional kernel capable of managing memory, storage, and user interaction. Rather than building on an existing kernel, every subsystem is implemented from scratch to gain practical experience with low-level systems programming and operating system design.

## Current Version

## v0.3 - Filesystem

A bootable release ISO is available in the `release/` directory and on the project's GitHub Releases page.

## Releases

Each tagged release includes a bootable ISO that can be downloaded from the GitHub Releases page.

Alternatively, clone the repository and build MiyarOS from source using the provided Makefile.

## Features

### Kernel

* Multiboot-compliant 32-bit x86 boot flow through GRUB.
* Kernel written entirely in C and x86 Assembly.
* Global Descriptor Table (GDT).
* Interrupt Descriptor Table (IDT).
* PIC remapping and IRQ handling.
* CPU exception handling.
* Kernel panic screen with diagnostic information.
* Dedicated page fault handler.

### Memory Management

* Multiboot memory map detection.
* Physical Memory Manager (PMM).
* Bitmap-based physical frame allocator.
* Kernel heap (`kmalloc` / `kfree`).
* Paging support.
* Virtual Memory Manager (VMM).
* Dynamic page table creation.
* Read-only kernel memory protection.

### Storage & Filesystem

* ATA PIO disk driver.
* Disk abstraction layer.
* FAT32 filesystem detection.
* FAT table management.
* Cluster allocation and deallocation.
* Multi-cluster file reading.
* Multi-cluster file writing.
* File appending.
* FAT32 directory traversal.
* File and directory lookup.
* File creation and deletion.
* Directory creation and deletion.
* File copy.
* File rename (move).
* Path resolution.

### Drivers

* PS/2 keyboard driver with Shift support.
* PIT timer and uptime reporting.
* VGA text-mode terminal with scrolling and backspace.
* Serial debugging output.

### Shell

* Table-driven interactive shell.
* Filesystem integration.
* Output redirection using `>` and `>>`.

## Built-In Shell Commands

| Command          | Description                             |
| ---------------- | --------------------------------------- |
| `help`           | Shows the built-in command list.        |
| `about`          | Displays project information.           |
| `echo <msg>`     | Prints text or redirects it to a file.  |
| `ls`             | Lists files and directories.            |
| `cat <file>`     | Displays a file's contents.             |
| `touch <file>`   | Creates an empty file.                  |
| `mkdir <dir>`    | Creates a directory.                    |
| `rm <file>`      | Deletes a file.                         |
| `rmdir <dir>`    | Deletes an empty directory.             |
| `cp <src> <dst>` | Copies a file.                          |
| `mv <old> <new>` | Renames a file.                         |
| `uptime`         | Shows the number of seconds since boot. |
| `clear`          | Clears the VGA text screen.             |
| `reboot`         | Reboots the machine.                    |
| `shutdown`       | Attempts to power off the machine.      |
| `exception`      | Triggers a divide-by-zero exception.    |
| `pagefault`      | Triggers a page fault.                  |

## Repository Structure

```text
Miyar-OS/
├── Makefile
├── README.md
├── LICENSE
├── linker.ld
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
│           └── isr.asm
│
└── kernel/
    ├── arch/
    │   └── x86/
    │       ├── gdt.c
    │       ├── idt.c
    │       ├── irq.c
    │       └── exceptions.c
    │
    ├── drivers/
    │   ├── ata.c
    │   ├── disk.c
    │   ├── keyboard.c
    │   ├── pic.c
    │   ├── serial.c
    │   ├── timer.c
    │   └── vga.c
    │
    ├── memory/
    │   ├── multiboot.c
    │   ├── memory_map.c
    │   ├── pmm.c
    │   ├── heap.c
    │   ├── paging.c
    │   ├── vmm.c
    │   └── page_fault.c
    │
    ├── fs/
    │   ├── directory.c
    │   ├── fat32.c
    │   ├── file.c
    │   ├── fs.c
    │   └── path.c
    │
    ├── lib/
    │   └── string.c
    │
    ├── include/
    │   └── ...
    │   (all header files)
    │
    ├── kernel.c
    ├── panic.c
    ├── shell.c
    └── terminal.c
```

## Build Requirements

* GNU Make
* NASM
* GCC with 32-bit multilib support
* GNU Binutils (`ld`)
* GRUB (`grub-mkrescue`)
* xorriso
* QEMU (recommended)

## Build Instructions

```bash
make clean
make
```

This builds the kernel and generates a bootable ISO.

## Run

```bash
make run
```

Or manually:

```bash
qemu-system-i386 -cdrom release/MiyarOS-v0.3.iso
```

Terminal-only:

```bash
qemu-system-i386 -curses -cdrom release/MiyarOS-v0.3.iso
```

## Roadmap

### Completed

#### v0.1 - Core Kernel

* Boot process
* Interrupt handling
* Drivers
* VGA terminal
* Interactive shell

#### v0.2 - Memory Management

* Physical Memory Manager
* Kernel heap
* Paging
* Virtual Memory Manager
* Page fault handling
* Read-only kernel memory protection

#### v0.3 - Filesystem

* ATA disk driver
* FAT32 filesystem
* File management
* Directory management
* Path resolution
* Multi-cluster file I/O
* Filesystem shell commands

### Planned

#### v0.4 - Process Management & Multitasking

* Scheduler
* Context switching
* Kernel threads
* Processes

#### v0.5 - User Mode & System Calls

* Ring 3 execution
* ELF program loading
* System call interface

#### v0.6 - Virtual File System (VFS)

* Generic filesystem layer
* Multiple filesystem support

#### Future

* USB drivers
* Audio
* Networking
* Security improvements
* Graphical user interface

## License

MiyarOS is released under the GNU General Public License v3.0. See the [LICENSE](LICENSE) file for details.