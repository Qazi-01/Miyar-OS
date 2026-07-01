global isr0

section .text

isr0:
cli

.hang:
hlt
jmp .hang