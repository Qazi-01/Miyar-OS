global isr0
extern isr0_handler

section .text

isr0:
    cli

    pusha

    call isr0_handler

.hang:
    hlt
    jmp .hang
