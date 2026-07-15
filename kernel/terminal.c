#include "serial.h"
#include "terminal.h"
#include "vga.h"

#include <stdint.h>

void terminal_init(void)
{
    vga_set_color(VGA_BLACK, VGA_WHITE);

    vga_clear();
}

void terminal_putchar(char c)
{
    vga_putc(c);
    serial_putc(c);
}

void terminal_write(const char *str)
{
    while (*str)
    {
        terminal_putchar(*str++);
    }
}

void terminal_writeIn(const char *str)
{
    terminal_write(str);
    terminal_write("\n");
}

void terminal_write_hex(uint32_t value)
{
    const char hex[] = "0123456789ABCDEF";

    terminal_write("0x");
    for (int i = 28; i >= 0; i -= 4)
    {
        terminal_putchar(hex[(value >> i) & 0xF]);
    }
}

void terminal_prompt(void)
{
    terminal_write("> ");
}

void terminal_clear(void)
{
    vga_clear();
}

void terminal_backspace(void)
{
    vga_backspace();
}