#ifndef TERMINAL_H
#define TERMINAL_H

#include <stdint.h>

void terminal_clear(void);
void terminal_init(void);
void terminal_write(const char *str);
void terminal_writeIn(const char *str);
void terminal_write_hex(uint32_t value);
void terminal_prompt(void);
void terminal_putchar(char c);
void terminal_backspace(void);

#endif