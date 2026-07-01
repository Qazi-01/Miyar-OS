global isr0
extern isr0_handler

section .text

isr0:
    cli

    pusha

    push esp
    call isr0_handler
    add esp, 4

    popa

    iretd

