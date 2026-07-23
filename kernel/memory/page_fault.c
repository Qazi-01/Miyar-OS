#include <stdint.h>

#include "memory/page_fault.h"
#include "terminal.h"

void page_fault_handler(uint32_t error_code)
{
    uint32_t fault_address;

    __asm__ volatile("mov %%cr2, %0" : "=r"(fault_address));

    terminal_writeIn("");
    terminal_writeIn("========================");
    terminal_writeIn("      PAGE FAULT");
    terminal_writeIn("========================");

    terminal_write("Fault Address: ");
    terminal_write_hex(fault_address);
    terminal_writeIn("");

    terminal_write("Error Code: ");
    terminal_write_hex(error_code);
    terminal_writeIn("");
    terminal_writeIn("");

    terminal_write("Present: ");
    terminal_writeIn((error_code & 0x1) ? "Yes" : "No");

    terminal_write("Access: ");
    terminal_writeIn((error_code & 0x2) ? "Write" : "Read");

    terminal_write("Mode: ");
    terminal_writeIn((error_code & 0x4) ? "User" : "Kernel");

    terminal_write("Reserved Bit: ");
    terminal_writeIn((error_code & 0x8) ? "Yes" : "No");

    terminal_write("Instruction Fetch: ");
    terminal_writeIn((error_code & 0x10) ? "Yes" : "No");

    terminal_writeIn("");
    terminal_writeIn("System Halted.");

    __asm__ volatile("cli");

    for (;;)
    {
        __asm__ volatile("hlt");
    }
}
