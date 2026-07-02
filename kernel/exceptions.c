#include "exceptions.h"
#include "io.h"

void exception_handler(struct registers *r)
{
    (void)r;

    while (1)
    {
        __asm__ volatile("hlt");
    }
}

void irq_handler(struct registers *r)
{
    if (r->int_no >= 40)
    {
        outb(0xA0, 0x20);
    }

    outb(0x20, 0x20);
}
