#include "irq.h"
#include "timer.h"
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

void irq_handler(struct registers *r) {
    if (r->int_no == 33) {
        unsigned char scancode = inb(0x60);

        if ((scancode & 0x80) == 0 && scancode < 128) {
            keyboard_buffer_put(scancode_table[scancode]);
        }
    }

    if (r->int_no == 32) {
        timer_tick();
    }

    pic_send_eoi(r->int_no - 32);
}