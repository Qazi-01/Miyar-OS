#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdint.h>

char keyboard_getchar(void);
void keyboard_init(void);

uint8_t inb(uint16_t port);
void outb(uint16_t port, uint8_t value);

#endif