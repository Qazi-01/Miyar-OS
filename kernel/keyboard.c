#include <stdint.h>
#include "keyboard.h"

char keyboard_getchar(void)
{
    return 0;
}

uint8_t inb(uint16_t port)
{
    uint8_t value;

    __asm__ volatile ("inb %1, %0" : "=a"(value) : "Nd"(port));

    return value;
}

void outb(uint16_t port, uint8_t value)
{
    __asm__ volatile ("outb %0, %1" :: "a"(value), "Nd"(port));
}

void keyboard_init(void)
{
}