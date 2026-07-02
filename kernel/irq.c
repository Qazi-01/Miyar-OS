#include "irq.h"
#include "pic.h"
#include "io.h"
#include "keyboard.h"
#include "terminal.h"

static const char scancode_table[128] =
{
    0, 27,'1','2','3','4','5','6','7','8','9','0','-','=', '\b','\t',
    'q','w','e','r','t','y','u','i','o','p','[',']','\n',0,
    'a','s','d','f','g','h','j','k','l',';','\'','`',0,'\\',
    'z','x','c','v','b','n','m',',','.','/',0,'*',0,' ',
};

void irq_handler(struct registers *r)
{
    unsigned char irq = r->int_no - 32;

    if (irq == 1)
    {
        terminal_write("Keyboard IRQ\n");
    }

    pic_send_eoi(irq);
}
