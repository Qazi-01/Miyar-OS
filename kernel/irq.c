#include "irq.h"
#include "terminal.h"

void irq_handler(void)
{
    terminal_write("IRQ\n");
}