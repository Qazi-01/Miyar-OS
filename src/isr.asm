global isr0
extern isr0_handler

section .text

isr0:
cli
call isr0_handler

.hang:
hlt
jmp .hang