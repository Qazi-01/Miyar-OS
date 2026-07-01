global isr_stub_0
extern exception_handler

section .text

isr_stub_0:
    cli

    pusha                ; Save EAX, ECX, EDX, EBX, ESP, EBP, ESI, EDI

    call exception_handler

    popa                 ; Restore registers

    iret
