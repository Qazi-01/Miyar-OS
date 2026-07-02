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