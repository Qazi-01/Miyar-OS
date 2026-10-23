global enter_user_mode

section .text

enter_user_mode;
    cli

    mov ax, 0x23

    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    push dword 0x23
    push dword 0x00800000
    push dword 0x202
    push dword 0x1B
    push dword 0x00400000

    iretd

section .note.GNU-stack noalloc noexec nowrite progbits