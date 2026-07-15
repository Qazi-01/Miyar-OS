#include "panic.h"
#include "terminal.h"

void kernel_panic(const char *message, struct registers *r)
{
    terminal_writeIn("");
    terminal_writeIn("========================");
    terminal_writeIn("      KERNEL PANIC");
    terminal_writeIn("========================");
    terminal_writeIn("");
    terminal_writeIn(message);
    terminal_write("EIP: ");
    terminal_write_hex(r->eip);
    terminal_writeIn("");
    terminal_write("Error Code: ");
    terminal_write_hex(r->err_code);
    terminal_writeIn("");
    terminal_write("EFLAGS: ");
    terminal_write_hex(r->eflags);
    terminal_writeIn("");
    terminal_write("Interrupt Number: ");
    terminal_write_hex(r->int_no);
    terminal_writeIn("");
    terminal_writeIn("System Halted.");

    __asm__ volatile("cli");

    for (;;)
    {
        __asm__ volatile("hlt");
    }
}