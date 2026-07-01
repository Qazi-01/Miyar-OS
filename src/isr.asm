global isr_stub_0
global isr_stub_1
global isr_stub_2
global isr_stub_3

extern exception_handler

section .text

%macro ISR_NOERR 1
isr_stub_%1:
    cli
    pusha
    call exception_handler
    popa
    iret
%endmacro

ISR_NOERR 0
ISR_NOERR 1
ISR_NOERR 2
ISR_NOERR 3
