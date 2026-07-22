#include "arch/x86/exceptions.h"
#include "memory/page_fault.h"
#include "panic.h"
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
    if (r->int_no == 14)
    {
        page_fault_handler(r->err_code);
        return;
    }

    terminal_write("\nEXCEPTION!\n");

    if (r->int_no < 21)
    {
        kernel_panic(exception_names[r->int_no], r);
    }

    else
    {
        kernel_panic("Unknown Exception", r);
    }
}
