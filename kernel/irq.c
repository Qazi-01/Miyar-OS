#include "irq.h"
#include "pic.h"
#include "terminal.h"

void irq_handler(void)
{
    terminal_write("IRQ\n");

    pic_send_eoi(0);
}