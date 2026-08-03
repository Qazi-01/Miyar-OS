#include <stdint.h>

#include "drivers/io.h"
#include "drivers/serial.h"

void serial_init(void) {
    outb(0x3F8 + 1, 0x00);
    outb(0x3F8 + 3, 0x80);
    outb(0x3F8 + 0, 0x01);
    outb(0x3F8 + 1, 0x00);
    outb(0x3F8 + 3, 0x03);
    outb(0x3F8 + 2, 0xC7);
    outb(0x3F8 + 4, 0x0B);
}

void serial_putc(char c) {
    outb(0x3F8, c);
}

void kputs(const char *s) {
    if (!s)
        return;
    
    while (*s) {
        serial_putc(*s++);
    }
}
