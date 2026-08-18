global x86_context_restore

section .text

; ---------------------------------------------------------
; void x86_context_restore(uint32_t saved_esp)
;
; Restores a prepared kernel-thread context.
;
; Stack at saved_esp:
;
;   gs
;   fs
;   es
;   ds
;
;   edi
;   esi
;   ebp
;   esp
;   ebx
;   edx
;   ecx
;   eax
;
;   int_no
;   err_code
;
;   eip
;   cs
;   eflags
;
; ---------------------------------------------------------

x86_context_restore:

    mov esp, [esp + 4]

    pop gs
    pop fs
    pop es
    pop ds

    popa

    add esp, 8

    iretd

section .note.GNU-stack noalloc noexec nowrite progbits