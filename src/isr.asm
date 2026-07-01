global isr0
global isr_common_stub

extern isr0_handler

section .text

isr0:
    cli
    push byte 0
    push byte 0
    jmp isr_common_stub

isr_common_stub:
    pusha
    
    mov eax, esp
    push eax

    call isr0_handler

    add esp, 4

    popa

    add esp, 8

    iretd