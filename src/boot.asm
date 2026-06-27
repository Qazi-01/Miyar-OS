; MiyarOS boot entry – 32-bit multiboot1
section .multiboot
align 4

MB_MAGIC    equ 0x1BADB002
MB_FLAGS    equ 0x3
MB_CHECKSUM equ -(MB_MAGIC + MB_FLAGS)

dd MB_MAGIC
dd MB_FLAGS
dd MB_CHECKSUM

section .text
global _start
extern kernel_main

_start:
    mov esp, stack_top
    push ebx
    push eax
    call kernel_main
    cli
.hang:
    hlt
    jmp .hang

section .bss
align 16
stack_bottom:
    resb 16384
stack_top: