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

static const char scancode_table_shift[128] =
{
    0, 27,'!','@','#','$','%','^','&','*','(',')','_','+', '\b','\t',
    'Q','W','E','R','T','Y','U','I','O','P','{','}','\n',0,
    'A','S','D','F','G','H','J','K','L',':','"','~',0,'|',
    'Z','X','C','V','B','N','M','<','>','?',0,'*',0,' ',
};

void irq_handler(struct registers *r) {
    static unsigned char shift_pressed = 0;

    if (r->int_no == 33) {
        unsigned char scancode = inb(0x60);

        if (scancode == 0x2A || scancode == 0x36) {
            shift_pressed = 1;
        }
        else if (scancode == 0xAA || scancode == 0xB6) {
            shift_pressed = 0;
        }
        else if ((scancode & 0x80) == 0 && scancode < 128) {
            /* Capture shift state to avoid potential race */
            unsigned char shifted = shift_pressed;
            char c = shifted ? scancode_table_shift[scancode] : scancode_table[scancode];

            if (c != 0) {
                keyboard_buffer_put(c);
            }
        }
    }

    if (r->int_no == 32) {
        timer_tick();
    }

    pic_send_eoi(r->int_no - 32);
}