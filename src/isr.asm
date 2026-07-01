global isr_stub_0
global isr_stub_1
global isr_stub_2
global isr_stub_3
global isr_stub_4
global isr_stub_5
global isr_stub_6
global isr_stub_7
global isr_stub_8
global isr_stub_9
global isr_stub_10
global isr_stub_11
global isr_stub_12
global isr_stub_13
global isr_stub_14
global isr_stub_15
global isr_stub_16
global isr_stub_17
global isr_stub_18
global isr_stub_19
global isr_stub_20
global isr_stub_21
global isr_stub_22
global isr_stub_23
global isr_stub_24
global isr_stub_25
global isr_stub_26
global isr_stub_27
global isr_stub_28
global isr_stub_29
global isr_stub_30
global isr_stub_31

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
