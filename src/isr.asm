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

%assign i 0
%rep 32
ISR_NOERR i
%assign i i + 1
%endrep