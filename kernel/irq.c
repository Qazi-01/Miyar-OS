#include "irq.h"
#include "pic.h"
#include "io.h"

void irq_handler(struct registers *r)
{
    unsigned char irq = (unsigned char)(r->int_no - 32);

    switch (irq)
    {
        case 1:
            /* Keyboard IRQ: read the scancode to clear the controller */
            (void)inb(0x60);
            break;

        default:
            break;
    }

    pic_send_eoi(irq);
}
