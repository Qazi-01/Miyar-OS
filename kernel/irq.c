#include "irq.h"
#include "pic.h"
#include "terminal.h"

void irq_handler(struct registers *r)
{
    (void)r;

    terminal_write("IRQ\n");

    pic_send_eoi(0);
}