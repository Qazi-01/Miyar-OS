#include "panic.h"
#include "terminal.h"

void panic(const char *message)
{
    terminal_writeIn("");
    terminal_writeIn("========================");
    terminal_writeIn("KERNEL PANIC");
    terminal_writeIn("========================");
    terminal_writeIn(message);
    terminal_writeIn("");
    terminal_writeIn("System Halted.");

    while (1)
    {
        __asm__ volatile("hlt");
    }
}