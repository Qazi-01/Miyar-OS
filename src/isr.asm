global isr_stub_0
extern exception_handler

section .text

isr_stub_0:
    cli
    call exception_handler
    iret