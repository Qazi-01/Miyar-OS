#include "exceptions.h"
#include "terminal.h"

static const char *exception_names[] = {
    "Divide Error",
    "Debug",
    "NMI",
    "Breakpoint",
    "Overflow",
    "BOUND",
    "Invalid Opcode",
    "Device Not Available",
    "Double Fault",
    "Coprocessor Segment",
    "Invalid TSS",
    "Segment Not Present",
    "Stack Fault",
    "General Protection Fault",
    "Page Fault",
    "Reserved",
    "x87 Floating Point",
    "Alignment Check",
    "Machine Check",
    "SIMD Floating Point",
    "Virtualization"
};

void exception_handler(struct registers *r)
{
    terminal_write("\nEXCEPTION!\n");

    if (r->int_no < 21)
    {
    terminal_write((char *)exception_names[r->int_no]);
    terminal_write("\n");
    }
    
    while (1)
        __asm__ volatile("cli; hlt");
}
