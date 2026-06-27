; MiyarOS boot entry - 32-bit multiboot1
section .multiboot
align 4

MB_MAGIC equ 0x1BADBOO2
MB_FLAGS equ 0x3 ; page-align + provide memory map
MB_CHECKSUM equ -(MB_MAGIC + MB_FLAGS)

dd MB_MAGIC
dd MB_FLAGS
dd MB_CHECKSUM

section .text
global _start
extern kernel_main

_start:
mov esp, stack_top  ; set stack pointer
push ebx  ; push multiboot info pointer (2nd arg)
push eax  ; push magic number (1st arg)
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