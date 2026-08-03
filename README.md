# Miyar-OS

MiyarOS is a hobby operating system for 32-bit x86 that boots with GRUB and follows the Multiboot specification. The kernel is written entirely in C and x86 Assembly and is developed from scratch as a learning project. It currently features physical and virtual memory management, a fully functional FAT32 filesystem, an interactive command-line shell, hardware device drivers, interrupt handling, and paging.

## Why I Built It

MiyarOS was created to understand how an operating system works from the ground up, from the very first boot instruction to a functional kernel capable of managing memory, storage, and user interaction. Rather than building on an existing kernel, every subsystem is implemented from scratch to gain practical experience with low-level systems programming and operating system design.

## Current Version

**v0.3 - Filesystem**:
A bootable release ISO is available in the `release/` directory and on the project's GitHub Releases page.

## Highlights

* 32-bit x86 kernel written from scratch in C and x86 Assembly.
* GRUB Multiboot compliant.
* Physical and virtual memory management.
* Fully functional FAT32 filesystem.
* Unix-like shell with filesystem commands.
* Absolute and relative path resolution.
* Multi-cluster file support.
* Open-source and built as a learning project.

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
* FAT32 filesystem detection and mounting.
* FAT table management.
* Cluster allocation and deallocation.
* Multi-cluster file reading and writing.
* File appending.
* FAT32 directory traversal.
* File and directory lookup.
* File creation, deletion, copying, and renaming.
* Directory creation and deletion.
* Absolute and relative path resolution.
* Current working directory support.
* Path normalization (`.`, `..`, `/`).
* FAT32 8.3 filename support.

### Drivers

* PS/2 keyboard driver with Shift support.
* PIT timer and uptime reporting.
* VGA text-mode terminal with scrolling and backspace.
* Serial debugging output.

### Shell

* Interactive command-line shell.
* Table-driven command dispatcher.
* FAT32 filesystem integration.
* Absolute and relative path support.
* Output redirection using `>` and `>>`.

## Built-In Shell Commands

| Command                 | Description                                    |
| ----------------------- | ---------------------------------------------- |
| `help`                  | Shows the list of available commands.          |
| `about`                 | Displays information about MiyarOS.            |
| `clear`                 | Clears the VGA text screen.                    |
| `uptime`                | Displays the time elapsed since boot.          |
| `pwd`                   | Prints the current working directory.          |
| `cd <dir>`              | Changes the current working directory.         |
| `ls [dir]`              | Lists the contents of a directory.             |
| `cat <file>`            | Displays the contents of a file.               |
| `touch <file>`          | Creates an empty file.                         |
| `mkdir <dir>`           | Creates a new directory.                       |
| `rm <file>`             | Deletes a file.                                |
| `rmdir <dir>`           | Deletes an empty directory.                    |
| `cp <src> <dst>`        | Copies a file.                                 |
| `mv <old> <new>`        | Renames or moves a file.                       |
| `echo <text>`           | Prints text to the terminal.                   |
| `echo <text> > <file>`  | Writes text to a file, replacing its contents. |
| `echo <text> >> <file>` | Appends text to the end of a file.             |
| `reboot`                | Reboots the machine.                           |
| `shutdown`              | Attempts to power off the machine.             |
| `exception`             | Triggers a divide-by-zero exception.           |
| `pagefault`             | Triggers a page fault.                         |

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
qemu-system-i386 \
    -cdrom release/MiyarOS-v0.3.iso \
    -drive file=disk.img,format=raw
```

Terminal-only:

```bash
qemu-system-i386 \
    -curses \
    -cdrom release/MiyarOS-v0.3.iso \
    -drive file=disk.img,format=raw
```

## Disk Image

MiyarOS requires a FAT32-formatted disk image for filesystem support.

Create a 64 MB disk image:

```bash
dd if=/dev/zero of=disk.img bs=1M count=64
```

Format it as FAT32:

```bash
mkfs.fat -F 32 disk.img
```

Place `disk.img` in the project root (alongside the `Makefile`).

> **Note:** The disk image is not included in the repository and must be created before using the filesystem.

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

* ATA PIO disk driver
* FAT32 filesystem implementation
* File and directory management
* Multi-cluster file I/O
* Absolute and relative path resolution
* Current working directory
* Integrated filesystem shell commands

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
