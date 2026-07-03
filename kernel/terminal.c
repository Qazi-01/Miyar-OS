#include "serial.h"
#include "terminal.h"
#include "vga.h"

void terminal_init(void)
{
    vga_set_color(VGA_BLACK, VGA_WHITE);

    vga_clear();
}

void terminal_write(const char *str)
{
    vga_write(str);
    kputs(str);
}

void terminal_writeIn(const char *str)
{
    terminal_write(str);
    terminal_write("\n");
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