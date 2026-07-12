# MiyarOS

MiyarOS is a small hobby operating system for x86 that boots with GRUB and follows the Multiboot specification. The kernel is written in C and x86 Assembly, and it currently provides a text-mode boot experience with basic hardware setup, a keyboard-driven shell, VGA output, and serial debugging.

## Why I Built It

I built MiyarOS to understand how a kernel comes together from the first boot instruction through to a usable command line. It is a hands-on project for learning low-level x86 development, interrupt handling, and simple device drivers without hiding the hardware behind a larger framework.

## Current Version

v0.1

The bootable ISO is built locally as [miyaros.iso](miyaros.iso). The checked-in copy stays on the v0.1 baseline until v0.2 is ready for its final refresh.

## Features

- Multiboot-compliant 32-bit x86 boot flow through GRUB.
- Kernel code split between C and x86 Assembly.
- Global Descriptor Table setup.
- Interrupt Descriptor Table setup for CPU exceptions and hardware IRQs.
- PIC remapping and end-of-interrupt handling.
- Basic exception handling for CPU faults.
- PS/2 keyboard input with scancode translation and Shift support.
- VGA text mode terminal output.
- Terminal scrolling and backspace handling.
- Serial output for debugging and mirrored console text.
- PIT timer initialization and uptime reporting.
- A small interactive shell with a handful of built-in commands.

## Built-In Shell Commands

| Command | Description |
| --- | --- |
| `help` | Shows the built-in command list. |
| `about` | Prints project information, author, and license. |
| `version` | Displays the kernel and system version. |
| `echo <msg>` | Prints the provided message back to the screen. |
| `uptime` | Shows the number of seconds since boot. |
| `clear` | Clears the VGA text screen. |
| `reboot` | Reboots the machine. |
| `shutdown` | Attempts to power off the machine through the shutdown port. |
| `exception` | Triggers a divide-by-zero exception to test the exception handler. |

## Project Structure

```text
Miyar-OS/
├── Makefile
├── LICENSE
├── linker.ld
├── README.md
├── miyaros.iso
├── iso/
│   └── boot/
│       └── grub/
│           └── grub.cfg
├── src/
│   ├── boot.asm
│   ├── gdt.asm
│   └── isr.asm
└── kernel/
	├── kernel.c
	├── gdt.c/.h
	├── idt.c/.h
	├── pic.c/.h
	├── irq.c/.h
	├── exceptions.c/.h
	├── timer.c/.h
	├── keyboard.c/.h
	├── terminal.c/.h
	├── vga.c/.h
	├── serial.c/.h
	├── shell.c/.h
	├── panic.c/.h
	└── io.h
```

## Build Requirements

- GNU Make
- NASM
- GCC with 32-bit multilib support
- GNU binutils, including `ld`
- `grub-mkrescue`
- `xorriso`
- QEMU system emulator for i386, if you want to run it locally

On Debian or Ubuntu, the required packages are typically available through the standard development toolchain and GRUB packages, plus 32-bit GCC support.

## Build Instructions

From the project root, run:

```bash
make clean && make
```

This builds the kernel, links it into `build/kernel.elf`, and packages a bootable ISO as `miyaros.iso`.

Until v0.2 is complete and tested, the repository should avoid ISO-only updates on `main`. The release ISO will be replaced once the v0.2 build is validated.

## Run Instructions

The easiest way to boot the system is:

```bash
make run
```

This launches QEMU with the generated ISO in text mode. The project also boots through GRUB, so you can inspect `iso/boot/grub/grub.cfg` if you want to adjust the boot entry or use the ISO in another emulator.

## Roadmap

### Current State

- Bootable x86 kernel with GRUB and Multiboot.
- Text console, serial output, keyboard input, timer ticks, and interrupt handling.
- A small shell with basic system commands.

### Future Work

- Memory management.
- Multitasking.
- A filesystem.
- User programs and a richer command environment.
- Additional drivers and hardware support.

## License

MiyarOS is released under the GNU General Public License v3.0. See [LICENSE](LICENSE) for the full text.
