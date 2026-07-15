#include "panic.h"
#include "terminal.h"

void kernel_panic(const char *message, struct registers *r)
{
    terminal_writeIn("");
    terminal_writeIn("========================");
    terminal_writeIn("      KERNEL PANIC");
    terminal_writeIn("========================");
    terminal_writeIn(message);
    terminal_writeIn("");
    terminal_writeIn("System Halted.");

    __asm__ volatile("cli");

    for (;;)
    {
        __asm__ volatile("hlt");
    }
}