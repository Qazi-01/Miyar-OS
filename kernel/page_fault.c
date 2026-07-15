#include <stdint.h>

#include "page_fault.h"
#include "terminal.h"

static const char *page_fault_reason(uint32_t error_code)
{
    if (!(error_code & 0x1))
    {
        return "Page not present";
    }

    if (error_code & 0x2)
    {
        return "Write access";
    }

    return "Read access";
}

void page_fault_handler(uint32_t error_code)
{
    uint32_t fault_address;

    __asm__ volatile("mov %%cr2, %0" : "=r"(fault_address));

    (void)error_code;
    (void)fault_address;
    
    terminal_writeIn("===PAGE FAULT!===");
    terminal_writeIn(page_fault_reason(error_code));

    __asm__ volatile ("cli");
    
    for (;;)
    {
        __asm__ volatile ("hlt");
    }
}