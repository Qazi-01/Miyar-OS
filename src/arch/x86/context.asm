global x86_context_switch

section .text

; ---------------------------------------------------------
; void x86_context_switch(uint32_t *old_esp,
;                         uint32_t new_esp)
;
; Save the current thread's CPU context and switch to the
; new thread's saved context.
;
; Both contexts use the same interrupt-style stack frame.
; ---------------------------------------------------------

x86_context_switch:

    ; Preserve the original argument stack pointer.
    mov edx, esp

    ; Build an interrupt-style frame for the current thread. The
    ; resume stub consumes the original C return address correctly.
    push dword 0x202
    push dword 0x08
    push dword .resume
    push dword 0
    push dword 0

    ; Save the current CPU context.
    pusha

    push ds
    push es
    push fs
    push gs

    ; old_esp argument:
    ; [original ESP + 4]
    mov eax, [edx + 4]

    ; Save current context ESP.
    mov [eax], esp

    ; new_esp argument:
    ; [original ESP + 8]
    mov esp, [edx + 8]

    ; Restore new thread's segment registers.
    pop gs
    pop fs
    pop es
    pop ds

    ; Restore new thread's general registers.
    popa

    ; Skip interrupt number and error code.
    add esp, 8

    ; Restore EIP, CS and EFLAGS.
    iretd

.resume:
    ret

section .note.GNU-stack noalloc noexec nowrite progbits